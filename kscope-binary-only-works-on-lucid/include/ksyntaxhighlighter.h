/*
 ksyntaxhighlighter.cpp

 Copyright (c) 2003 Trolltech AS
 Copyright (c) 2003 Scott Wheeler <wheeler@kde.org>

 This file is part of the KDE libraries

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License version 2 as published by the Free Software Foundation.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public License
 along with this library; see the file COPYING.LIB.  If not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA 02110-1301, USA.
*/

#ifndef KSYNTAXHIGHLIGHTER_H
#define KSYNTAXHIGHLIGHTER_H

#include <qtextedit.h>
#include <qsyntaxhighlighter.h>
#include <qcolor.h>
#include <qstringlist.h>

#include <kdelibs_export.h>

class QAccel;
class QTimer;
class KSpell;
class KSpellConfig;

/**
 * \brief Syntax sensitive text highlighter
 */
class KDEUI_EXPORT KSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    enum SyntaxMode {
	PlainTextMode,
	RichTextMode
    };
    KSyntaxHighlighter( QTextEdit *textEdit,
			 bool colorQuoting = false,
			 const QColor& QuoteColor0 = black,
			 const QColor& QuoteColor1 = QColor( 0x00, 0x80, 0x00 ),
			 const QColor& QuoteColor2 = QColor( 0x00, 0x80, 0x00 ),
			 const QColor& QuoteColor3 = QColor( 0x00, 0x80, 0x00 ),
			 SyntaxMode mode = PlainTextMode );
    ~KSyntaxHighlighter();

    int highlightParagraph( const QString& text, int endStateOfLastPara );

private:
    class KSyntaxHighlighterPrivate;
    KSyntaxHighlighterPrivate *d;
};

class KDEUI_EXPORT KSpellingHighlighter : public KSyntaxHighlighter
{
public:
    KSpellingHighlighter( QTextEdit *textEdit,
			  const QColor& spellColor = red,
			  bool colorQuoting = false,
			  const QColor& QuoteColor0 = black,
			  const QColor& QuoteColor1 = QColor( 0x00, 0x80, 0x00 ),
			  const QColor& QuoteColor2 = QColor( 0x00, 0x80, 0x00 ),
			  const QColor& QuoteColor3 = QColor( 0x00, 0x80, 0x00 ) );
    ~KSpellingHighlighter();

    virtual int highlightParagraph( const QString &text,
				    int endStateOfLastPara );
    virtual bool isMisspelled( const QString& word ) = 0;
    bool intraWordEditing() const;
    void setIntraWordEditing( bool editing );
    static QStringList personalWords();

private:
    void flushCurrentWord();

    class KSpellingHighlighterPrivate;
    KSpellingHighlighterPrivate *d;
};

/**
 * \brief Dictionary sensitive text highlighter
 */
class KDEUI_EXPORT KDictSpellingHighlighter : public QObject, public KSpellingHighlighter
{
Q_OBJECT

public:
    KDictSpellingHighlighter( QTextEdit *textEdit,
			      bool spellCheckingActive = true,
			      bool autoEnable = true,
			      const QColor& spellColor = red,
			      bool colorQuoting = false,
			      const QColor& QuoteColor0 = black,
			      const QColor& QuoteColor1 = QColor( 0x00, 0x80, 0x00 ),
			      const QColor& QuoteColor2 = QColor( 0x00, 0x70, 0x00 ),
			      const QColor& QuoteColor3 = QColor( 0x00, 0x60, 0x00 ),
                              KSpellConfig *spellConfig = 0 );
    ~KDictSpellingHighlighter();

    virtual bool isMisspelled( const QString &word );
    static void dictionaryChanged();
    void restartBackgroundSpellCheck();

    /**
     * @short Enable/Disable spell checking.
     *
     * If @p active is true then spell checking is enabled; otherwise it
     * is disabled. Note that you have to disable automatic (de)activation
     * with @ref setAutomatic() before you change the state of spell checking
     * if you want to persistently enable/disable spell checking.
     *
     * @param active if true, then spell checking is enabled
     *
     * @see isActive(), setAutomatic()
     */
    void setActive( bool active );

    /**
     * Returns the state of spell checking.
     *
     * @return true if spell checking is active
     *
     * @see setActive()
     */
    bool isActive() const;

    /**
     * @short En-/Disable automatic (de)activation in case of too many errors.
     *
     * If @p automatic is true then spell checking will be deactivated if
     * too many words were mispelled and spell checking will be activated
     * again if the amount of mispelled words drop below a certain threshold.
     *
     * @param automatic if true, then automatic (de)activation is enabled
     *
     * @see automatic()
     */
    void setAutomatic( bool automatic );

    /**
     * Returns the state of automatic (de)activation.
     *
     * @return true if automatic (de)activation is enabled
     *
     * @see setAutomatic()
     */
    bool automatic() const;

signals:
    void activeChanged(const QString &);
    void newSuggestions(const QString& originalword, const QStringList& suggestions,
                        unsigned int pos);

protected:
    QString spellKey();
    bool eventFilter(QObject *o, QEvent *e);

protected slots:
    void slotMisspelling( const QString &originalWord, const QStringList &suggestions, unsigned int pos );
    void slotCorrected( const QString &originalWord, const QString &, unsigned int );
    void slotRehighlight();
    void slotDictionaryChanged();
    void slotSpellReady( KSpell *spell );
    void slotAutoDetection();
    void slotLocalSpellConfigChanged();
    void slotKSpellNotResponding();

private:
    class KDictSpellingHighlighterPrivate;
    KDictSpellingHighlighterPrivate *d;
};

#endif
