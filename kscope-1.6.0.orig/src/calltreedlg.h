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

#ifndef CALLTREEDLG_H
#define CALLTREEDLG_H

#include <qwidget.h>
#include <qlistview.h>
#include <calltreelayout.h>

/**
 * A multiple-view window showing function call information.
 * The available views are:
 * - Call graph, showing both calling and call functions
 * - Called functions tree
 * - Calling functions tree
 * NOTE: This is class is now derived from QWidget instead of QDialog. This 
 * means that call-trees are independent windows, which can be maximised or
 * minimised.
 * @author Elad Lahav
 */
class CallTreeDlg : public CallTreeLayout
{
   Q_OBJECT
   
public: 
	CallTreeDlg(QWidget* pParent = 0, const char* szName = 0);
	~CallTreeDlg();
	
	void setRoot(const QString&);
	bool load(const QString&, const QString&);
	void store(const QString&);
	
	/** Returns Call Tree filename */
	QString getFileName() { return m_sFileName; }

public slots:
	virtual void show();

signals:
	/**
	 * Emitted when the user makes a request to view the contents of a
	 * location in the source code.
	 * This can be the location of a call, the definition of a function,
	 * etc.
	 * @param	sPath	The full path of the file to show
	 * @param	nLine	The line number in this file
	 */
	void lineRequested(const QString& sPath, uint nLine);
	
	/**
	 * Emitted when the user closes the tree view.
	 */
	void closed(const CallTreeDlg*);

protected:
	virtual void closeEvent(QCloseEvent*);

protected slots:
	virtual void slotSaveClicked();
	virtual void slotZoomInClicked();
	virtual void slotZoomOutClicked();
	virtual void slotRotateClicked();
	virtual void slotPrefClicked();
	virtual void slotViewChanged(int);
	
private:
	/** The root function. */
	QString m_sRoot;
	
	/** A unique file name used for storing the call tree on a file.
		The name is a combination of the root function and an incremented
		index. */
	QString m_sFileName;
	
	/** The full path of the file on which the call tree was saved 
		(empty if this graph was never stored). */
	QString m_sFilePath;
		
	/** The view to show when the dialogue is first displayed. */
	int m_nDefView;
		
	/** An index for the generating unique file names. */
	static int s_nFileNameIndex;
};

#endif
