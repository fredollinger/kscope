/***************************************************************************
 *
 * Copyright (C) 2006 Elad Lahav (elad_lahav@users.sf.net)
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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
#include <qpaintdevicemetrics.h>
#include <kmessagebox.h>
#include <klocale.h>
#include "dotfrontend.h"
#include "graphwidget.h"
#include "kscopeconfig.h"

/**
 * Class constructor.
 * @param	pGraph	The graph widget on which to draw the output
 */
DotFrontend::DotFrontend(GraphWidget* pGraph) : Frontend(1),
	m_pGraph(pGraph)
{
}

/**
 * Class destructor.
 */
DotFrontend::~DotFrontend()
{
}

/**
 * Executes dot on the goven input file.
 * @param	sFile	The path to a temporary file holding the graph's 
 *					description
 * @return	true if successful, false otherwise
 */
bool DotFrontend::run(const QString& sFile)
{
	QString sPath;
	QStringList slArgs;
	QPaintDeviceMetrics pdm(m_pGraph);
	
	// Set the horizontal and vertical DPI values
	m_dDpiX = (double)pdm.logicalDpiX();
	m_dDpiY = (double)pdm.logicalDpiY();

	// Make sure the executable exists
	sPath = Config().getDotPath();
	
	// Set the command line arguments
	slArgs.append(sPath);
	slArgs.append("-Tplain");
	slArgs.append(sFile);
	
	// Run a new process
	if (!Frontend::run("dot", slArgs))
		return false;

	// Initialize stdout parsing
	m_state = Graph;
	m_delim = All;

	return true;
}

/**
 * Tests that the given file path leads to an executable.
 * @param	sPath	The path to check
 * @return	true if the file in the given path exists and has executable
 *			permissions, false otherwise
 */
bool DotFrontend::verify(const QString& sPath)
{
	QFileInfo fi(sPath);

	if (!fi.exists() || !fi.isFile() || !fi.isExecutable() ||
		fi.fileName() != "dot") {
		KMessageBox::error(0, i18n("Dot cannot be found in the given "
			"path"));
		return false;
	}

	return true;
}

#define PAD	5

/**
 * Parses the output of a Dot process.
 * @param	sToken	The current token read (the token delimiter is determined
 *					by the current state)
 * @param	delim	The delimiter that ends this token
 * @return	A value indicating the way this token should be treated: dropped,
 *			added to the token queue, or finishes a new record
 */
Frontend::ParseResult DotFrontend::parseStdout(QString& sToken,
	ParserDelim delim)
{
	static int nWidth, nHeight, nXpos, nYpos, nCurveSize, nCurveCount;
	static QPointArray arrCurve;
	static QString sNode, sEdgeHead, sEdgeTail;
	ParseResult result = DiscardToken;
	double dVal;
	bool bOK;
	
	// Handle the token according to the current state
	switch (m_state) {
	case Graph:
		if (sToken == "graph")
			m_state = GraphScale;
		break;

	case GraphScale:
		sToken.toDouble(&bOK);
		if (bOK)
			m_state = GraphWidth;
		break;

	case GraphWidth:
		// Read and transform the graph's width
		dVal = sToken.toDouble(&bOK);
		if (bOK) {
			nWidth = (int)(dVal * m_dDpiX) + (PAD * 2);
			m_state = GraphHeight;
		}
		break;

	case GraphHeight:
		// Read and transform the graph's height
		dVal = sToken.toDouble(&bOK);
		if (bOK) {
			nHeight = (int)(dVal * m_dDpiY) + (PAD * 2);
			
			// Set the graph's size
			m_pGraph->resize(nWidth, nHeight);
			
			m_state = NodeEdgeStop;
		}
		break;
		
	case NodeEdgeStop:
		// "node" starts a new node
		// "edge" starts a new edge
		// "stop" ends this graph
		if (sToken == "node") {
			m_state = NodeName;
		}
		else if (sToken == "edge") {
			m_state = EdgeHead;
		}
		else if (sToken == "stop") {
			m_state = Graph;
		}
		break;

	case NodeName:
		// Get a node's name
		sNode = sToken;
		m_state = NodeCentreX;
		break;

	case NodeCentreX:
		// Read and transform the node's centre location (X coordinate)
		dVal = sToken.toDouble(&bOK);
		if (bOK) {
			nXpos = (int)(dVal * m_dDpiX) + PAD;
			m_state = NodeCentreY;
		}
		break;

	case NodeCentreY:
		// Read and transform the node's centre location (Y coordinate)
		dVal = sToken.toDouble(&bOK);
		if (bOK) {
			nYpos = (int)(dVal * m_dDpiY) + PAD;
			m_state = NodeWidth;
		}
		break;

	case NodeWidth:
		// Read and transform the node's width
		dVal = sToken.toDouble(&bOK);
		if (bOK) {
			nWidth = (int)(dVal * m_dDpiX);
			m_state = NodeHeight;
		}
		break;

	case NodeHeight:
		// Read and transform the node's height
		dVal = sToken.toDouble(&bOK);
		if (bOK) {
			nHeight = (int)(dVal * m_dDpiY);
			
			// Create the bounding rectangle of the node
			QRect rect;
			rect.setX(nXpos - (nWidth / 2));
			rect.setY(nYpos - (nHeight / 2));
			rect.setWidth(nWidth);
			rect.setHeight(nHeight);
			
			// Draw the node
			m_pGraph->drawNode(sNode, rect);
			
			m_state = EndNodeEdge;
		}
		break;

	case EdgeHead:
		// Get the edge's head node
		sEdgeHead = sToken;
		m_state = EdgeTail;
		break;

	case EdgeTail:
		// Get the edge's tail node
		sEdgeTail = sToken;
		m_state = EdgeCurveSize;
		break;

	case EdgeCurveSize:
		// Get the number of control points in the edge's spline
		nCurveSize = sToken.toInt(&bOK);
		if (bOK) {
			arrCurve.resize(nCurveSize);
			nCurveCount = 0;
			m_state = EdgeCurveX;
		}
		break;

	case EdgeCurveX:
		// Read and a control point (X coordinate)
		dVal = sToken.toDouble(&bOK);
		if (bOK) {
			nXpos = (int)(dVal * m_dDpiX) + PAD;
			m_state = EdgeCurveY;
		}
		break;

	case EdgeCurveY:
		// Read and a control point (Y coordinate)
		dVal = sToken.toDouble(&bOK);
		if (bOK) {
			nYpos = (int)(dVal * m_dDpiY) + PAD;
			
			// Add the control point to the spline array
			arrCurve.setPoint(nCurveCount++, nXpos, nYpos);
			
			// Check if this is the last control point
			if (nCurveCount == nCurveSize) {
				// Draw the edge
				m_pGraph->drawEdge(sEdgeHead, sEdgeTail, arrCurve);
				
				// Must detach from contents since a QPointArray shares data
				arrCurve.detach();
				
				m_state = EndNodeEdge;
			}
			else {
				// Another control point available
				m_state = EdgeCurveX;
			}
		}
		break;
		
	case EndNodeEdge:
		// Discard everything else on a node or edge line
		if (delim == Newline)
			m_state = NodeEdgeStop;
		break;
	}

	return result;
}

#include "dotfrontend.moc"
