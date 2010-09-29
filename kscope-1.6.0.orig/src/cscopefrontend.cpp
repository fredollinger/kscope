/***************************************************************************
 *
 * Copyright (C) 2005 Elad Lahav (elad_lahav@users.sourceforge.net)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ***************************************************************************/

#include <qfileinfo.h>
#include <qtimer.h>
#include <kconfig.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kglobalsettings.h>
#include "cscopefrontend.h"
#include "kscopeconfig.h"
#include "configfrontend.h"

#define BUILD_STR		"Building symbol database %d of %d"
#define SEARCH_STR		"Search %d of %d"
#define INV_STR			"Possible references retrieved %d of %d"
#define REGEXP_STR		"Symbols matched %d of %d"
#define SEARCHEND_STR	"%d lines"

QString CscopeFrontend::s_sProjPath;
uint CscopeFrontend::s_nProjArgs;
uint CscopeFrontend::s_nSupArgs;

/**
 * Class constructor.
 * @param	bAutoDelete	true to delete the object once the process has
 *						terminated, false otherwise
 */
CscopeFrontend::CscopeFrontend(bool bAutoDelete) : 
	Frontend(CSCOPE_RECORD_SIZE, bAutoDelete),
	m_state(Unknown),
	m_sErrMsg(""),
	m_bRebuildOnExit(false)
{
}

/**
 * Class destructor.
 */
CscopeFrontend::~CscopeFrontend()
{
}

/**
 * Executes a Cscope process using the given command line arguments.
 * The full path to the Cscope executable should be set in the "Path" key
 * under the "Cscope" group.
 * @param	slArgs	Command line arguments for Cscope
 * @return	true if successful, false otherwise
 */
bool CscopeFrontend::run(const QStringList& slArgs)
{
	QStringList slCmdLine;

	// Set the command line arguments
	slCmdLine.append(Config().getCscopePath());
	slCmdLine += slArgs;
	
	// Use verbose mode, if supported
	if (s_nSupArgs & VerboseOut)
		slCmdLine << "-v";
		
	// Project-specific options
	if (s_nProjArgs & Kernel)
		slCmdLine << "-k";
	if (s_nProjArgs & InvIndex)
		slCmdLine << "-q";
	if (s_nProjArgs & NoCompression)
		slCmdLine << "-c";
	if (s_nProjArgs & s_nSupArgs & SlowPathDef)
		slCmdLine << "-D";
		
	// Run a new process
	if (!Frontend::run("cscope", slCmdLine, s_sProjPath)) {
		emit aborted();
		return false;
	}

	return true;
}

/**
 * Executes a Cscope query.
 * A query is composed of a numeric type and a query text, which are written
 * to the stndard input of the currently running Cscope process.
 * @param	nType		The type of query to run
 * @param	sText		The query's text
 * @param	nMaxRecords	The maximal number of records to return (abort if this
 *						number is exceeded)
 */
void CscopeFrontend::query(uint nType, const QString& sText, uint nMaxRecords)
{
	QString sQuery;
	QStringList slArgs;
	
	m_nMaxRecords = nMaxRecords;
	
	// Create the Cscope command line
	slArgs.append(QString("-L") + QString::number(nType));
	slArgs.append(sText);
	slArgs.append("-d");
	
	run(slArgs);
	
	// Initialise stdout parsing
	m_state = SearchSymbol;
	m_delim = WSpace;

	emit progress(0, 1);
}

/**
 * Rebuilds the symbol database of the current project.
 */
void CscopeFrontend::rebuild()
{
	QStringList slArgs;
	
	// If a process is already running, kill it start a new one
	if (isRunning()) {
		m_bRebuildOnExit = true;
		kill();
		return;
	}
	
	// Run the database building process
	slArgs.append("-b");
	run(slArgs);
	
	// Initialise output parsing
	m_state = BuildStart;
	m_delim = Newline;
	
	emit progress(0, 1);
}

/**
 * Sets default parameters for all CscopeFrontend projects based on the
 * current project.
 * @param	sProjPath	The full path of the project's directory
 * @param	nArgs		Project-specific command-line arguments
 */
void CscopeFrontend::init(const QString& sProjPath, uint nArgs)
{
	s_sProjPath = sProjPath;
	s_nProjArgs = nArgs;
}

/**
 * Stops a Cscope action.
 */
void CscopeFrontend::slotCancel()
{
	kill();
}

/**
 * Parses the output of a Cscope process.
 * Implements a state machine, where states correspond to the output of the
 * controlled Cscope process.
 * @param	sToken	The current token read (the token delimiter is determined
 *					by the current state)
 * @return	A value indicating the way this token should be treated: dropped,
 *			added to the token queue, or finishes a new record
 */
Frontend::ParseResult CscopeFrontend::parseStdout(QString& sToken,
	ParserDelim /* ignored */)
{
	int nFiles, nTotal, nRecords;
	ParseResult result = DiscardToken;
	ParserState stPrev;
	
	// Remember previous state
	stPrev = m_state;
	
	// Handle the token according to the current state
	switch (m_state) {
	case BuildStart:
		if (sToken == "Building cross-reference...") {
			m_state = BuildSymbol;
			m_delim = WSpace;
		}
		else if (sToken == "Building inverted index...") {
			emit buildInvIndex();
		}
		
		result = DiscardToken;
		break;

	case BuildSymbol:
		// A single angle bracket is the prefix of a progress indication,
		// while double brackets is Cscope's prompt for a new query
		if (sToken == ">") {
			m_state = Building;
			m_delim = Newline;
		}

		result = DiscardToken;
		break;

	case Building:
		// Try to get building progress
		if (sscanf(sToken.latin1(), BUILD_STR, &nFiles, &nTotal) == 2) {
			emit progress(nFiles, nTotal);
			
			// Check for last progress message
			if (nFiles == nTotal) {
				m_state = BuildStart;
				m_delim = Newline;
				
				result = DiscardToken;
				break;
			}
		}

		// Wait for another progress line or the "ready" symbol
		m_state = BuildSymbol;
		m_delim = WSpace;

		result = DiscardToken;
		break;

	case SearchSymbol:
		// Check for more search progress, or the end of the search,
		// designated by a line in the format of "cscope: X lines"
		if (sToken == ">") {
			m_state = Searching;
			m_delim = Newline;
			result = DiscardToken;
			break;
		}
		else if (sToken == "cscope:") {
			m_state = SearchEnd;
			m_delim = Newline;
			result = DiscardToken;
			break;
		}

	case File:
		// Is this the first entry? If so, signal that the query is complete
		if (stPrev != LineText)
			emit progress(1, 1);

		// Treat the token as the name of the file in this record
		m_state = Func;
		result = AcceptToken;
		break;

	case Searching:
		// Try to get the search progress value (ignore other messages)
		if ((sscanf(sToken.latin1(), SEARCH_STR, &nFiles, &nTotal) == 2) ||
			(sscanf(sToken.latin1(), INV_STR, &nFiles, &nTotal) == 2) ||
			(sscanf(sToken.latin1(), REGEXP_STR, &nFiles, &nTotal) == 2)) {
			emit progress(nFiles, nTotal);
		}

		m_state = SearchSymbol;
		m_delim = WSpace;
		result = DiscardToken;
		break;

	case SearchEnd:
		// Get the number of results found in this search
		if ((sscanf(sToken.latin1(), SEARCHEND_STR, &nRecords) == 1) &&
			(m_nMaxRecords > 0) &&
			(nRecords > m_nMaxRecords)) {
			result = Abort;
		}
		else {
			m_state = File;
			m_delim = WSpace;
			result = DiscardToken;
		}
		
		break;
		
	case Func:
		// Treat the token as the name of the function in this record
		if (sToken.toInt()) {
			// In case of a global definition, there is no function name, and
			// instead the line number is given immediately
			m_state = LineText;
			m_delim = Newline;
		}
		else {
			// Not a number, it is the name of the function
			m_state = Line;
		}
		
		result = AcceptToken;
		break;

	case Line:
		// Treat the token as the line number in this record
		m_state = LineText;
		m_delim = Newline;
		result = AcceptToken;
		break;

	case LineText:
		// Treat the token as the text of this record, and report a new
		// record
		m_state = File;
		m_delim = WSpace;
		result = RecordReady;
		break;

	default:
		// Do nothing (prevents a compilation warning for unused enum values)
		break;
	}

	return result;
}

/**
 * Handles Cscope messages sent to the standard error stream.
 * @param	sText	The error message text
 */
void CscopeFrontend::parseStderr(const QString& sText)
{
	// Wait for a complete line to arrive
	m_sErrMsg += sText;
	if (!sText.endsWith("\n"))
		return;
	
	// Display the error message
	emit error(m_sErrMsg);
		
	// Line displayed, reset the text accumulator
	m_sErrMsg = "";
}

/**
 * Called when the underlying process exits.
 * Checks if the rebuild flag was raised, and if so restarts the building
 * process.
 */
void CscopeFrontend::finalize()
{
	// Reset the parser state machine
	m_state = Unknown;
	
	// Restart the building process, if required
	if (m_bRebuildOnExit) {
		m_bRebuildOnExit = false;
		rebuild();
	}
}

/**
 * Class constructor.
 * @param	pMainWidget	The parent widget to use for the progress bar and
 * 						label
 */
CscopeProgress::CscopeProgress(QWidget* pMainWidget) : QObject(),
	m_pMainWidget(pMainWidget),
	m_pProgressBar(NULL),
	m_pLabel(NULL)
{
}

/**
 * Class destructor.
 */
CscopeProgress::~CscopeProgress()
{
	delete m_pProgressBar;
	delete m_pLabel;
}

/**
 * Displays query progress information.
 * If the progress value is below the expected final value, a progress bar is
 * used to show the advance of the query process. Otherwise, a label is
 * displayed asking the user to wait ahile the query output is processed.
 * @param	nProgress	The current progress value
 * @param	nTotal		The expected final value
 */
void CscopeProgress::setProgress(int nProgress, int nTotal)
{
	// Was the final value is reached?
	if (nProgress == nTotal) {
		// Destroy the progress bar
		if (m_pProgressBar != NULL) {
			delete m_pProgressBar;
			m_pProgressBar = NULL;
		}
		
		// Show the "Please wait..." label
		if (m_pLabel == NULL) {
			m_pLabel = new QLabel(i18n("Processing query results, "
				"please wait..."), m_pMainWidget);
			m_pLabel->setFrameStyle(QFrame::Box | QFrame::Plain);
			m_pLabel->setLineWidth(1);
			m_pLabel->adjustSize();
			
			m_pLabel->setPaletteBackgroundColor(
				KGlobalSettings::highlightColor());
			m_pLabel->setPaletteForegroundColor(
				KGlobalSettings::highlightedTextColor());
				
			QTimer::singleShot(1000, this, SLOT(slotShowLabel()));
		}
		
		return;
	}

	// Create the progress bar, if it does not exist.
	// Note that the progress bar will only be displayed one second after the
	// first progress signal is received. Thus the bar will not be displayed
	// on very short queries.
	if (m_pProgressBar == NULL) {
		m_pProgressBar = new QProgressBar(m_pMainWidget);
		QTimer::singleShot(1000, this, SLOT(slotShowProgressBar()));
	}
	
	// Set the current progress value
	m_pProgressBar->setProgress(nProgress, nTotal);
}

/**
 * detsroys any progress widgets when the process is terminated.
 */
void CscopeProgress::finished()
{
	// Destroy the progress bar
	if (m_pProgressBar != NULL) {
		delete m_pProgressBar;
		m_pProgressBar = NULL;
	}
	
	// Destroy the label
	if (m_pLabel != NULL) {
		delete m_pLabel;
		m_pLabel = NULL;
	}
}

/**
 * Shows the progress bar.
 * This slot is connected to a timer activated when the first progress signal
 * is received.
 */
void CscopeProgress::slotShowProgressBar()
{
	if (m_pProgressBar != NULL)
		m_pProgressBar->show();
}

/**
 * Shows the "Please wait...".
 * This slot is connected to a timer activated when the progress bar
 * reaches its final value.
 */
void CscopeProgress::slotShowLabel()
{
	if (m_pLabel != NULL)
		m_pLabel->show();
}

void CscopeVerifier::verify()
{
	ConfigFrontend* pConf;
	 
	pConf = new ConfigFrontend(true);
	connect(pConf, SIGNAL(result(uint, const QString&)), this,
		SLOT(slotConfigResult(uint, const QString&)));
	connect(pConf, SIGNAL(finished(uint)), this, SLOT(slotFinished()));
	
	pConf->run(Config().getCscopePath(), "", "", true);
}

void CscopeVerifier::slotConfigResult(uint nType, const QString& sResult)
{
	switch (nType) {
	case ConfigFrontend::CscopeVerbose:
		if (sResult == "Yes")
			m_nArgs |= CscopeFrontend::VerboseOut;
		break;
	
	case ConfigFrontend::CscopeSlowPath:
		if (sResult == "Yes")
			m_nArgs |= CscopeFrontend::SlowPathDef;
			
		// If we got this far, then Cscope is configured properly
		m_bResult = true;
		break;
	}
}

void CscopeVerifier::slotFinished()
{
	emit done(m_bResult, m_nArgs);
	delete this;
}

#include "cscopefrontend.moc"
