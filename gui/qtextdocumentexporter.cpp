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

#include <QString>
#include <QTextDocumentFragment>
#include <QTextCharFormat>
#include <QTextBlock>
#include <QDebug>
#include "qtextdocumentexporter.h"
#include "ced/cedchar.h"
#include "common/tostring.h"
#include "export/rout_own.h"

using namespace cf;

static inline QRect toQRect(const cf::Rect& r) {
    return QRect(r.x(), r.y(), r.width(), r.height());
}

static inline QColor toQColor(const cf::Color& c) {
    return QColor(c.red(), c.green(), c.blue());
}

QTextDocumentExporter::QTextDocumentExporter(CEDPage * page, const FormatOptions& opts) :
        GenericExporter(page, opts), doc_(), cursor_(&doc_)
{
}

QTextDocumentExporter::AltMap QTextDocumentExporter::charAlternatives(const cf::CEDChar& chr) const {
    AltMap alts;

    for(size_t i = 1; i < chr.alternativeCount(); i++) {
        int probability = chr.alternativeAt(i).probability();
        QString str = QString::fromUtf8(converter_.convert(chr.alternativeAt(i).getChar()).c_str());
        alts.insert(str, QVariant(probability));
    }

    return alts;
}

void QTextDocumentExporter::exportCharAlternatives(QTextCharFormat& format, const cf::CEDChar& chr) const {
    if(chr.alternativeCount() < 2)
        return;

    AltMap alt_map = charAlternatives(chr);

    // set tooltip
    if(formatOptions().showAlternatives()) {
        QString tooltip = "Alternatives:";

        foreach(QString alt, alt_map.keys()) {
            tooltip.append(QString("\n        '%1' (%2)").arg(alt).arg(alt_map.value(alt).toInt()));
        }

        format.setToolTip(tooltip);
    }

    format.setProperty(ALTERNATIVES, QVariant(alt_map));
}

void QTextDocumentExporter::exportElementColor(QTextFormat& format, const cf::Element& el) const {
    if(!el.color().isNull())
        format.setForeground(QBrush(toQColor(el.color())));
}

void QTextDocumentExporter::exportElementBgColor(QTextFormat& format, const cf::Element& el) const {
    if(!el.backgroundColor().isNull())
        format.setBackground(QBrush(toQColor(el.backgroundColor())));
}

void QTextDocumentExporter::exportElementBBox(QTextFormat& format, const cf::Element& el) const {
    format.setProperty(BBOX, toQRect(el.boundingRect()));
}

void QTextDocumentExporter::exportCharItalic(QTextCharFormat& format, const cf::CEDChar& chr) const {
    if(formatOptions().isItalicUsed() && (chr.fontStyle() & FONT_ITALIC))
        format.setFontItalic(true);
}

void QTextDocumentExporter::exportCharBold(QTextCharFormat& format, const cf::CEDChar& chr) const {
    if(formatOptions().isBoldUsed() && (chr.fontStyle() & FONT_BOLD))
        format.setFontWeight(QFont::Bold);
}

void QTextDocumentExporter::exportCharUnderline(QTextCharFormat& format, const cf::CEDChar& chr) const {
    if(formatOptions().isUnderlinedUsed() && (chr.fontStyle() & FONT_UNDERLINE))
        format.setFontUnderline(true);
}

void QTextDocumentExporter::exportCharFontSize(QTextCharFormat& format, const cf::CEDChar& chr) const {
    if(formatOptions().isFontSizeUsed() && chr.fontHeight() > 0)
        format.setFontPointSize(chr.fontHeight());
}

void QTextDocumentExporter::clear() {
    doc_.clear();
}

QTextCursor& QTextDocumentExporter::cursor() {
    return cursor_;
}

QTextDocument& QTextDocumentExporter::document() {
    return doc_;
}

void QTextDocumentExporter::writeCharacter(CEDChar& chr) {
    if(!chr.hasAlternatives()) {
        qDebug() << Q_FUNC_INFO << "CEDChar is empty";
        return;
    }

    std::string std_str = converter_.convert(chr.alternativeAt(0).getChar());
    QString str = QString::fromUtf8(std_str.c_str());
    QTextCharFormat format;

    exportElementBBox(format, chr);
    exportElementColor(format, chr);
    exportElementBgColor(format, chr);

    exportCharAlternatives(format, chr);
    exportCharItalic(format, chr);
    exportCharBold(format, chr);
    exportCharUnderline(format, chr);
    exportCharFontSize(format, chr);

    cursor_.insertText(str, format);
}

void QTextDocumentExporter::writePageBegin(CEDPage& page) {
    cursor_.insertFragment(QTextDocumentFragment());
}

void QTextDocumentExporter::writeParagraphBegin(CEDParagraph& par) {
//    cursor_.insertBlock(QTextBlock());
}
