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
}

TextEditor::~TextEditor() {
    delete doc_;
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

void TextEditor::contextMenuEvent(QContextMenuEvent * event) {
     QMenu * menu = createStandardContextMenu();
     QAction * zoom_in = menu->addAction(QIcon(":/img/oxygen/32x32/zoom_in.png"), tr("Zoom In"), this, SLOT(zoomIn()));
     QAction * zoom_out = menu->addAction(QIcon(":/img/oxygen/32x32/zoom_out.png"), tr("Zoom Out"), this, SLOT(zoomOut()));

     if(document()->isEmpty()) {
         zoom_in->setEnabled(false);
         zoom_out->setEnabled(false);
     }

     menu->exec(event->globalPos());

     delete menu;
 }

void TextEditor::showCurrentChar() {
    QRect bbox = textCursor().charFormat().property(QTextDocumentExporter::BBOX).toRect();
    if(!bbox.isEmpty())
        emit charSelected(bbox);
}

void TextEditor::showPage(Page * page) {
    if(page == NULL || page == page_)
        return;

    page_ = page;
    connectPageSignal(page_);
    setDocument(page_->document());
    setReadOnly(false);
}
