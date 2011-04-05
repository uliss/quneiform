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
#include <QContextMenuEvent>
#include "texteditor.h"
#include "qtextdocumentexporter.h"
#include "page.h"

TextEditor::TextEditor(QWidget * parent) :
    QTextEdit(parent),
    page_(NULL),
    doc_(new QTextDocument)
{
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(showCurrentChar()));
    setReadOnly(true);
    settings_.beginGroup("format");
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

void TextEditor::addZoomActions(QMenu * menu) {
    if(document()->isEmpty())
        return;

    QAction * zoom_in = menu->addAction(QIcon(":/img/oxygen/32x32/zoom_in.png"), tr("Zoom In"), this, SLOT(zoomIn()));
    zoom_in->setShortcut(QKeySequence::ZoomIn);
    QAction * zoom_out = menu->addAction(QIcon(":/img/oxygen/32x32/zoom_out.png"), tr("Zoom Out"), this, SLOT(zoomOut()));
    zoom_out->setShortcut(QKeySequence::ZoomOut);
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
    setReadOnly(true);
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

    menu->exec(event->globalPos());
    delete menu;
 }

void TextEditor::showCurrentChar() {
    if(!settings_.value("showCurrentCharacter").toBool())
        return;

    QRect bbox = textCursor().charFormat().property(QTextDocumentExporter::BBOX).toRect();
    if(!bbox.isEmpty())
        emit charSelected(bbox);
}

void TextEditor::showPage(Page * page) {
    if(page == NULL || page == page_)
        return;

    page_ = page;
    connectPageSignal(page_);
    blockSignals(true);
    setDocument(page_->document());
    blockSignals(false);
}
