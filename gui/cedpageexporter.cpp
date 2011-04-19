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

#include <QtGlobal>
#include <QDebug>
#include <QTextFrame>
#include <QTextTable>
#include <QTextDocument>
#include "cedpageexporter.h"
#include "qtextdocumentexporter.h"
#include "ced/cedpage.h"
#include "ced/cedsection.h"

static cf::Rect getFormatMargins(const QTextFrameFormat& format) {
    return cf::Rect(cf::Point(format.leftMargin(), format.topMargin()),
                    cf::Point(format.rightMargin(), format.bottomMargin()));
}

CEDPageExporter::CEDPageExporter() :
        doc_(NULL),
        page_(NULL){

}

CEDPageExporter::~CEDPageExporter() {

}

QTextCursor * CEDPageExporter::cursor() {
    return &cursor_;
}

QTextDocument * CEDPageExporter::document() {
    return doc_;
}

void CEDPageExporter::doExport(QTextDocument * doc, cf::CEDPage * page) {
    Q_CHECK_PTR(doc);
    Q_CHECK_PTR(page);
    doc_ = doc;
    page_ = page;
    cursor_ = QTextCursor(doc);
    exportPage();
}

void CEDPageExporter::exportBlock(const QTextBlock& block) {
    qDebug() << Q_FUNC_INFO;
}

void CEDPageExporter::exportColumnTable(QTextTable * table) {
    Q_ASSERT(table);

    QTextTableFormat table_format = table->format();
    if(table_format.intProperty(QTextDocumentExporter::BlockType) != QTextDocumentExporter::SECTION) {
        qDebug() << Q_FUNC_INFO << "attept to export non section table";
        return;
    }

    cf::CEDSection * section = new cf::CEDSection;
    section->setMargins(getFormatMargins(table_format));

    page_->addSection(section);
}

void CEDPageExporter::exportPage() {
    // to document start
    cursor_.movePosition(QTextCursor::Start);
    QTextFrame * root = doc_->rootFrame();
    QTextFrameFormat format = root->frameFormat();

    if(format.intProperty(QTextDocumentExporter::BlockType) != QTextDocumentExporter::PAGE) {
        qDebug() << Q_FUNC_INFO << "trying to export non page frame as a page";
    }

    page_->setMargins(getFormatMargins(format));

    for(QTextFrame::Iterator it = root->begin(); !it.atEnd(); ++it) {
        QTextFrame * child_frame = it.currentFrame();

        if(child_frame == 0) {
            qDebug() << Q_FUNC_INFO << "non section text block found";
            exportBlock(it.currentBlock());
        }
        else {
            QTextTable * child_table = qobject_cast<QTextTable*>(child_frame);

            if (child_table)
                exportColumnTable(child_table);
            else
                exportSection(child_frame);
        }
    }
}

void CEDPageExporter::exportSection(QTextFrame * frame) {
    Q_ASSERT(frame);

    QTextFrameFormat fmt = frame->frameFormat();

    if(fmt.intProperty(QTextDocumentExporter::BlockType) != QTextDocumentExporter::PAGE) {
        qDebug() << Q_FUNC_INFO << "attempt to export non section frame";
    }

    cf::CEDSection * section = new cf::CEDSection;
    section->setMargins(getFormatMargins(fmt));

    page_->addSection(section);

}

cf::CEDPage * CEDPageExporter::page() {
    return page_;
}
