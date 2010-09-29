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

#ifndef SYMBOLCOMPLETION_H
#define SYMBOLCOMPLETION_H

#include <qobject.h>
#include <qregexp.h>
#include <ktexteditor/codecompletioninterface.h>
#include <ktexteditor/view.h>
#include "cscopefrontend.h"

/**
 * This class executes symbol definition queries based on symbol prefixes.
 * The results can then be displayed as symbol completion lists.
 * @author Albert Yosher
 */
class SymbolCompletion : public QObject
{
	Q_OBJECT

public:
	/**
	 * A pure-virtual class that allows a SymbolCompletion object access to
	 * text-editing objects.
	 * Classes that wish to utilise SymbolCompletion need to inplement this
	 * interface.
	 * @author Albert Yosher
	 */
	struct Interface
	{
		/**
	 	 * Class destructor.
	 	 * NOTE: A virtual destructor is required by GCC 4.0
		 */
		virtual ~Interface() {}

		/**
		 * Returns the word currently under the editing cursor.
		 * Symbol completion will be provided for this word only if the cursor
		 * is positioned at the end of this word.
		 * @param	pPosInWord	Set this value to the offset in the word on
		 * 						which the cursor is positioned
		 */
		virtual QString getWordUnderCursor(uint* pPosInWord) = 0;
		
		/**
		 * Returns a target object for displaying the completion list.
		 * @return	A pointer to an object implementing
		 *			KTextEditor::CodeCompletionInterface, or NULL if the
		 *			implementation does not support this interface.
		 */
		virtual QObject* getCCObject() = 0;
	};
	
    SymbolCompletion(SymbolCompletion::Interface*, QObject* pParent = 0, 
		const char* szName = 0);
    ~SymbolCompletion();

	void abort();
	
	static void initAutoCompletion(bool, uint, uint, uint);
	
public slots:
	void slotComplete();
	void slotAutoComplete();
		
private:
	/**
	 * Symbol completion entry object, used in the completion list.
	 * Implements operators required for sorting the completion list.
	 * @author Albert Yosher
	 */
	class Entry : public KTextEditor::CompletionEntry
	{
	public:
		/**
		 * Determines whether a given entry is smaller than this one.
		 * @param	entry	The entry to compare with
		 * @return	true if the given entry is smaller, false otherwise
		 */
		bool operator < (const SymbolCompletion::Entry& entry) const {
			return (text < entry.text);
		}
			
		/**
		 * Determines whether a given entry is equal to this one.
		 * @param	entry	The entry to compare with
		 * @return	true if the given entry equals this one, false otherwise
		 */
		bool operator == (const SymbolCompletion::Entry& entry) const {
			return (text == entry.text);
		}
	};
	
	/**
	 * A sortable version of the value list used by CodeCompletionInterface.
	 * @author Albert Yosher
	 */
	class EntryList : public QValueList<Entry>
	{
	public:
		/** 
		 * Sorts completion list.
		 */
		void sort() { qHeapSort(*this); }
		 
		 /**
		  * Type casting support required for calling showCompletionBox().
		  * @return	A casted reference to this object
		  */
		 operator QValueList<KTextEditor::CompletionEntry>() 
		 	{ return *(QValueList<KTextEditor::CompletionEntry>*)this; }
	};
	
	/** Editor object for which symbol completion is provided. */
	Interface* m_pEditor;
	
	/** An object that supports KTextEditor::CodeCompletionInterface, as
		supplied by the editor. */
	QObject* m_pCCObject;
	
	/** Cscope process used to run completion queries. */
	CscopeFrontend* m_pCscope;
	
	/** The prefix used for the current query. */
	QString m_sPrefix;
	
	/** A list of possible completions for the given prefix. */
	EntryList m_elEntries;
	
	/** The maximal number of completions to accept. */
	uint m_nMaxEntries;
	
	/** Regular expression for extracting a symbol out of Cscope's text field.
		NOTE: This member is required due to a bug in Cscope that renders the
		symbol field useless. */
	QRegExp m_reSymbol;
	
	/** Auto-completion timer. */
	QTimer* m_pAutoCompTimer;
	
	/** Auto-completion flag */
	bool m_bAutoCompletion;
	
	void complete(const QString&, int nMaxEntries = 1000);
	void filterEntries();
	void makeErrMsg(const QString&);
	
	/** true if auto-completion is enabled, false otherwise. */
	static bool s_bACEnabled;
	
	/** The minimum number of characters a symbol must have for
		auto-completion. */
	static uint s_nACMinChars;
	
	/** The interval between the time slotAutoComplete() is called and the
		time the completion process begins (in milliseconds). */
	static uint s_nACDelay;
	
	/** The maximal number of entries for auto-completion. */
	static uint s_nACMaxEntries;
	
private slots:
	void slotAutoCompleteTimeout();
	void slotAddEntry(FrontendToken*);
	void slotQueryFinished(uint);
	void slotFilterInsert(KTextEditor::CompletionEntry*, QString*);
};

#endif
