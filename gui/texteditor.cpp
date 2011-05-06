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

#include <QDebug>
#include <QMenu>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextLayout>
#include <QContextMenuEvent>
#include <QTextDocumentFragment>
#include <QPainter>
#include "texteditor.h"
#include "qtextdocumentexporter.h"
#include "page.h"

TextEditor::TextEditor(QWidget * parent) :
    QTextEdit(parent),
    page_(NULL),
    doc_(new QTextDocument),
    redo_(NULL),
    undo_(NULL),
    bold_(NULL),
    italic_(NULL),
    underlined_(NULL),
    zoom_in_(NULL),
    zoom_out_(NULL)
{
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(showCurrentChar()));
    setReadOnly(true);
    setTextInteractionFlags(Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
    settings_.beginGroup("format");

    setupActions();
}

TextEditor::~TextEditor() {
    delete doc_;
}

void TextEditor::addAlignmentActions(QMenu * menu) {
    if(document()->isEmpty())
        return;

    QMenu * align_menu = menu->addMenu(tr("Alignment"));

    QAction * align_left = align_menu->addAction(QIcon(":/img/oxygen/32x32/text_left.png"), tr("Left"), this, SLOT(alignLeft()));
    QAction * align_right = align_menu->addAction(QIcon(":/img/oxygen/32x32/text_right.png"), tr("Right"), this, SLOT(alignRight()));
    QAction * align_center = align_menu->addAction(QIcon(":/img/oxygen/32x32/text_center.png"), tr("Center"), this, SLOT(alignCenter()));
    QAction * align_justify = align_menu->addAction(QIcon(":/img/oxygen/32x32/text_block.png"), tr("Justify"), this, SLOT(alignJustify()));

    switch(textCursor().blockFormat().alignment()) {
    case Qt::AlignLeft:
        align_left->setCheckable(true);
        align_left->setChecked(true);
        break;
    case Qt::AlignRight:
        align_right->setCheckable(true);
        align_right->setChecked(true);
        break;
    case Qt::AlignHCenter:
        align_center->setCheckable(true);
        align_center->setChecked(true);
        break;
    case Qt::AlignJustify:
        align_justify->setCheckable(true);
        align_justify->setChecked(true);
        break;
    default:
        break;
    }
}

void TextEditor::addFontActions(QMenu * menu) {
    if(document()->isEmpty())
        return;

    QMenu * font_menu = menu->addMenu(tr("Font"));

    if(!textCursor().hasSelection()) {
        font_menu->setDisabled(true);
        return;
    }

    QTextCharFormat format = textCursor().charFormat();

    bold_->setChecked(format.fontWeight() == QFont::Bold);
    italic_->setChecked(format.fontItalic());
    underlined_->setChecked(format.fontUnderline());

    font_menu->addAction(bold_);
    font_menu->addAction(italic_);
    font_menu->addAction(underlined_);
}

void TextEditor::addUndoRedoActions(QMenu * menu) {
    if(document()->isEmpty())
        return;

    menu->addAction(undo_);
    menu->addAction(redo_);
}

void TextEditor::addZoomActions(QMenu * menu) {
    if(document()->isEmpty())
        return;

    menu->addAction(zoom_in_);
    menu->addAction(zoom_out_);
}

void TextEditor::alignCenter() {
    setAlignment(Qt::AlignHCenter);
}

void TextEditor::alignJustify() {
    setAlignment(Qt::AlignJustify);
}

void TextEditor::alignLeft() {
    setAlignment(Qt::AlignLeft);
}

void TextEditor::alignRight() {
    setAlignment(Qt::AlignRight);
}

void TextEditor::clearText() {
    setDocument(doc_);
    doc_->clear();
}

void TextEditor::connectPageSignal(Page * page) {
    Q_ASSERT(page);
    connect(page, SIGNAL(destroyed()), this, SLOT(clearText()));
}

void TextEditor::disconnectPageSignal(Page * page) {
    Q_ASSERT(page);
    disconnect(page, SIGNAL(destroyed()), this, SLOT(clearText()));
}

void TextEditor::contextMenuEvent(QContextMenuEvent * event) {
    QMenu * menu = createStandardContextMenu();
    addZoomActions(menu);
    menu->addSeparator();
    addAlignmentActions(menu);
    addFontActions(menu);
    menu->addSeparator();
    addUndoRedoActions(menu);

    menu->exec(event->globalPos());
    delete menu;
 }

void TextEditor::toggleBold() {
    if(!textCursor().hasSelection())
        return;

    QAction * act = qobject_cast<QAction*>(sender());

    if(!act)
        return;

    QTextCharFormat fmt;
    fmt.setFontWeight(act->isChecked() ? QFont::Bold : QFont::Normal);
    textCursor().mergeCharFormat(fmt);
}

void TextEditor::toggleItalic() {
    if(!textCursor().hasSelection())
        return;

    QAction * act = qobject_cast<QAction*>(sender());

    if(!act)
        return;

    QTextCharFormat fmt;
    fmt.setFontItalic(act->isChecked());
    textCursor().mergeCharFormat(fmt);
}

void TextEditor::toggleUnderlined() {
    if(!textCursor().hasSelection())
        return;

    QAction * act = qobject_cast<QAction*>(sender());

    if(!act)
        return;

    QTextCharFormat fmt;
    fmt.setFontUnderline(act->isChecked());
    textCursor().mergeCharFormat(fmt);
}

void TextEditor::showCurrentChar() {
    if(!settings_.value("showCurrentCharacter").toBool())
        return;

    QTextCursor cursor = textCursor();

    QRect bbox = cursor.charFormat().property(QTextDocumentExporter::BBOX).toRect();
    if(!bbox.isEmpty())
        emit charSelected(bbox);
    else {
        cursor.movePosition(QTextCursor::NextCharacter);
        bbox = cursor.charFormat().property(QTextDocumentExporter::BBOX).toRect();
        if(!bbox.isEmpty())
            emit charSelected(bbox);
    }
}

void TextEditor::showPage(Page * page) {
    if(page == NULL || page == page_)
        return;

    page_ = page;
    connectPageSignal(page_);
    blockSignals(true);
    setDocument(page_->document());
    blockSignals(false);

    undo_->setDisabled(true);
    redo_->setDisabled(true);
}

void TextEditor::setupActions() {
    setupFontActions();
    setupRedoAction();
    setupUndoAction();
    setupZoomActions();
}

void TextEditor::setupFontActions() {
    bold_ = new QAction(QIcon(":/img/oxygen/32x32/text_bold.png"), tr("bold"), this);
    bold_->setCheckable(true);
    bold_->setShortcut(QKeySequence::Bold);
    connect(bold_, SIGNAL(triggered()), this, SLOT(toggleBold()));
    addAction(bold_);

    italic_ = new QAction(QIcon(":/img/oxygen/32x32/text_italic.png"), tr("italic"), this);
    italic_->setCheckable(true);
    italic_->setShortcut(QKeySequence::Italic);
    connect(italic_, SIGNAL(triggered()), this, SLOT(toggleItalic()));
    addAction(italic_);

    underlined_ = new QAction(QIcon(":/img/oxygen/32x32/text_under.png"), tr("underlined"), this);
    underlined_->setCheckable(true);
    underlined_->setShortcut(QKeySequence::Underline);
    connect(underlined_, SIGNAL(triggered()), this, SLOT(toggleUnderlined()));
    addAction(underlined_);
}

void TextEditor::setupRedoAction() {
    redo_ = new QAction(QIcon(""), tr("Redo"), this);
    redo_->setShortcut(QKeySequence::Redo);
    connect(redo_, SIGNAL(triggered()), this, SLOT(redo()));
    connect(this, SIGNAL(redoAvailable(bool)), redo_, SLOT(setEnabled(bool)));
    addAction(redo_);
}

void TextEditor::setupUndoAction() {
    undo_ = new QAction(QIcon(":/img/oxygen/32x32/undo.png"), tr("Undo"), this);
    undo_->setShortcut(QKeySequence::Undo);
    connect(undo_, SIGNAL(triggered()), this, SLOT(undo()));
    connect(this, SIGNAL(undoAvailable(bool)), undo_, SLOT(setEnabled(bool)));
    addAction(undo_);
}

void TextEditor::setupZoomActions() {
    zoom_in_ = new QAction(QIcon(":/img/oxygen/32x32/zoom_in.png"), tr("Zoom In"), this);
    zoom_in_->setShortcut(QKeySequence::ZoomIn);
    connect(zoom_in_, SIGNAL(triggered()), this, SLOT(zoomIn()));
    addAction(zoom_in_);

    zoom_out_ = new QAction(QIcon(":/img/oxygen/32x32/zoom_out.png"), tr("Zoom Out"), this);
    zoom_out_->setShortcut(QKeySequence::ZoomOut);
    connect(zoom_out_, SIGNAL(triggered()), this, SLOT(zoomOut()));
    addAction(zoom_out_);
}
