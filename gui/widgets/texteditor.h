/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QTextEdit>
#include <QSettings>

class Page;
class QTextDocument;
class SpellCheckHighlighter;

class TextEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEditor(QWidget * parent = 0);
    ~TextEditor();

    /**
      * Shows page content in editor
      */
    void showPage(Page * page);
public slots:
    void checkSpelling();
signals:
    /**
      * Emitted on change cursor position
      * @param bbox - bounding box of current character
      */
    void charSelected(const QRect& bbox);
protected:
    void contextMenuEvent(QContextMenuEvent * event);
private slots:
    void alignCenter();
    void alignJustify();
    void alignLeft();
    void alignRight();
    void clearText();
    void handlePageDestroy();
    void showCurrentChar();
    void toggleBold();
    void toggleItalic();
    void toggleUnderlined();
private:
    void addAlignmentActions(QMenu * menu);
    void addFontActions(QMenu * menu);
    void addSpellCheckActions(QMenu * menu);
    void addSpellSuggestMenu(QMenu * menu, const QPoint& pos);
    void addUndoRedoActions(QMenu * menu);
    void addZoomActions(QMenu * menu);
    void connectPage();
    void disconnectPage();
    void setupActions();
    void setupFontActions();
    void setupRedoAction();
    void setupSpellActions();
    void setupUndoAction();
    void setupZoomActions();
private:
    Page * page_;
    QTextDocument * doc_;
    QSettings settings_;
    QAction * redo_;
    QAction * undo_;
    QAction * bold_;
    QAction * italic_;
    QAction * underlined_;
    QAction * zoom_in_;
    QAction * zoom_out_;
    QAction * spell_check_;
    SpellCheckHighlighter * highlighter_;
};

#endif // TEXTEDITOR_H
