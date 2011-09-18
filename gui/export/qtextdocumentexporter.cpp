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
#include <QTextTable>
#include <QTextTableFormat>
#include <QSettings>
#include <QDebug>
#include "page.h"
#include "qtextdocumentexporter.h"
#include "imagecache.h"
#include "ced/cedchar.h"
#include "ced/cedline.h"
#include "ced/cedpage.h"
#include "ced/cedparagraph.h"
#include "ced/cedpicture.h"
#include "ced/cedsection.h"
#include "ced/cedcolumn.h"
#include "common/tostring.h"
#include "common/fontstyle.h"
#include "common/tostring.h"
#include "quneiform_debug.h"

using namespace cf;

static void setFormatMargins(QTextFrameFormat& format, const cf::Rect& margins) {
    format.setTopMargin(margins.top());
    format.setRightMargin(margins.right());
    format.setBottomMargin(margins.bottom());
    format.setLeftMargin(margins.left());
}

static const char * SOFT_HYPEN = "\xAD";

static inline QRect toQRect(const cf::Rect& r) {
    return QRect(r.x(), r.y(), r.width(), r.height());
}

static inline QColor toQColor(const cf::Color& c) {
    return QColor(c.red(), c.green(), c.blue());
}

QTextDocumentExporter::QTextDocumentExporter(CEDPagePtr page, const FormatOptions& opts) :
        GenericExporter(page, opts),
        doc_(NULL),
        column_table_(NULL),
        column_num_(0),
        line_num_in_par_(0),
        par_line_count_(0),
        skip_columns_(true),
        page_(NULL)
{
}

CharAlternatives QTextDocumentExporter::charAlternatives(const cf::CEDChar& chr) const {
    CharAlternatives res;

    for(size_t i = 1; i < chr.alternativeCount(); i++) {
        CharAlternative alt;
        alt.code = convert(chr.alternativeAt(i).getChar());
        alt.probability = chr.alternativeAt(i).probability();
        res.append(alt);
    }

    return res;
}

QChar QTextDocumentExporter::convert(uchar ch) const {
    QString str = QString::fromUtf8(converter_.convert(ch).c_str());
    if(str.isEmpty())
        return QChar();
    return str.at(0);
}

void QTextDocumentExporter::exportCharAlternatives(QTextCharFormat& format, const cf::CEDChar& chr) const {
    if(chr.alternativeCount() < 2)
        return;

    // set tooltip
    if(formatOptions().showAlternatives()) {
        CharAlternatives alternatives = charAlternatives(chr);
        QString tooltip = "Alternatives:";

        static const char * TOOLTIP_FORMAT = "\n        '%1' (%2)";

        for(int i = 0, total = alternatives.length(); i < total; i++) {
            const CharAlternative& a = alternatives.at(i);
            tooltip.append(QString(TOOLTIP_FORMAT).arg(a.code).arg(a.probability));
        }

        format.setToolTip(tooltip);
        format.setUnderlineStyle(QTextCharFormat::DashUnderline);
        QSettings settings;
        settings.beginGroup("format");
        format.setUnderlineColor(settings.value("alternativeColor", Qt::red).value<QColor>());
        format.setProperty(ALTERNATIVES, alternatives.toVariant());
    }
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
    doc_->clear();
}

QTextCursor& QTextDocumentExporter::cursor() {
    return cursor_;
}

QTextDocument * QTextDocumentExporter::document() {
    return doc_;
}

void QTextDocumentExporter::insertColumnTable(int columns, const QTextTableFormat& fmt) {
    column_table_ = cursor_.insertTable(1, columns, fmt);
}

void QTextDocumentExporter::insertSectionFrame(cf::CEDSection& /*section*/) {
    QTextFrameFormat section_format;
//    setFormatMargins(section_format, section.margins());
    section_format.setProperty(BlockType, SECTION);
    cursor_.insertFrame(section_format);
}

void QTextDocumentExporter::insertSectionTable(cf::CEDSection& section) {
    QTextTableFormat tbl_format;
    tbl_format.setAlignment(Qt::AlignHCenter);
    tbl_format.setBorderStyle(QTextFrameFormat::BorderStyle_Dotted);
    tbl_format.setBorder(1);
    tbl_format.setCellPadding(3);
    tbl_format.setCellSpacing(0);
    tbl_format.setProperty(BlockType, SECTION);

    QVector<QTextLength> col_wd;
    for(size_t i = 0; i < section.columnCount(); i++) {
        CEDColumn * col = section.columnAt(i);
        if(col->width() > 0)
            col_wd.append(QTextLength(QTextLength::FixedLength, col->width()));
        else
            col_wd.append(QTextLength(QTextLength::VariableLength, 300));
    }

    tbl_format.setColumnWidthConstraints(col_wd);

    insertColumnTable(section.columnCount(), tbl_format);
}

void QTextDocumentExporter::moveCursorToColumn(int column) {
    Q_CHECK_PTR(column_table_);

    cursor_ = column_table_->cellAt(0, column).firstCursorPosition();
}

void QTextDocumentExporter::setDocument(QTextDocument * doc) {
    Q_ASSERT(doc);

    if(doc == doc_)
        return;

    doc_ = doc;
    cursor_ = QTextCursor(doc_);
}

void QTextDocumentExporter::setPage(Page * page) {
    page_ = page;
}

void QTextDocumentExporter::writeCharacter(CEDChar& chr) {
    if(!chr.hasAlternatives()) {
        qDebug() << Q_FUNC_INFO << "CEDChar is empty";
        return;
    }

    std::string std_str = converter_.convert(chr.get());
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

void QTextDocumentExporter::writeColumnBegin(cf::CEDColumn&) {
    if(skip_columns_)
        return;

    Q_CHECK_PTR(column_table_);

    moveCursorToColumn(column_num_);
    column_num_++;
}

void QTextDocumentExporter::writeLineEnd(cf::CEDLine& line) {
    if(line.empty())
        return;

    if(formatOptions().preserveLineBreaks()) {
        if(line_num_in_par_ != 0) {
            QTextBlockFormat current_fmt = cursor_.blockFormat();
            current_fmt.setTextIndent(0);
            cursor_.setBlockFormat(current_fmt);
        }

        cursor_.insertText("\n", cursor_.charFormat());
    }
    else {
        if(line.hardBreak()) {
            cursor_.insertText("\n");
        }
        else {
            if(line.endsWithHyphen()) {
                cursor_.deletePreviousChar();
                cursor_.insertText(SOFT_HYPEN);
            }
            else {
                // skip last line
                if(line_num_in_par_ != par_line_count_ - 1)
                    cursor_.insertText(" "); // space
            }
        }
    }

    line_num_in_par_++;
}

void QTextDocumentExporter::writePageBegin(cf::CEDPage& page) {
    Q_ASSERT(doc_);

    doc_->setUndoRedoEnabled(false);

    QTextFrameFormat page_format;
    if(page.imageSize().isValid()) {
//        format.setWidth(page.imageSize().width());
//        format.setHeight(page.imageSize().height());
    }

    setFormatMargins(page_format, page.margins());
    page_format.setProperty(BlockType, PAGE);
    doc_->rootFrame()->setFrameFormat(page_format);
}

void QTextDocumentExporter::writePageEnd(cf::CEDPage&) {
    Q_ASSERT(doc_);
    doc_->setUndoRedoEnabled(true);
}

void QTextDocumentExporter::writeParagraphBegin(CEDParagraph& par) {
    QTextBlockFormat format;

    switch(par.align()) {
    case ALIGN_NONE:
    case ALIGN_LEFT:
        format.setAlignment(Qt::AlignLeft);
        break;
    case ALIGN_RIGHT:
        format.setAlignment(Qt::AlignRight);
        break;
    case ALIGN_CENTER:
        format.setAlignment(Qt::AlignHCenter);
        break;
    case ALIGN_JUSTIFY:
        format.setAlignment(Qt::AlignJustify);
        break;
    }

    if(par.indent() != 0)
        format.setTextIndent(par.indent());

    cursor_.beginEditBlock();
    cursor_.insertBlock(format);
    cursor_.movePosition(QTextCursor::StartOfBlock);
    line_num_in_par_ = 0;
    par_line_count_ = par.lineCount();
}

void QTextDocumentExporter::writeParagraphEnd(cf::CEDParagraph&) {
    cursor_.endEditBlock();
}

void QTextDocumentExporter::writePicture(cf::CEDPicture& pic) {
    if(!pic.image()) {
        qDebug() << Q_FUNC_INFO << "empty image given";
        return;
    }

    if(!page_) {
        qDebug() << Q_FUNC_INFO << "NULL page pointer";
        return;
    }

    QPixmap pixmap;
    QTransform t = QTransform().rotate(page_->angle());
    ImageCache::load(page_->imagePath(), &pixmap);

    cf::Rect r = pic.boundingRect();
    pixmap = pixmap.transformed(t);

    if(page_->pageArea().isValid())
        pixmap = pixmap.copy(page_->pageArea());

    pixmap = pixmap.copy(r.x(), r.y(), r.width(), r.height());

    static int image_counter = 0;
    cursor_.insertImage(pixmap.toImage(), QString("image %1").arg(++image_counter));
//    QTextFrameFormat format;
//    cursor_.currentFrame()->setFrameFormat(format);
}

void QTextDocumentExporter::writeSectionBegin(cf::CEDSection& section) {
    if(section.empty())
        return;

    column_num_ = 0;

    skip_columns_ = (section.columnCount() < 2);

    if(skip_columns_)
        insertSectionFrame(section);
    else
        insertSectionTable(section);
}

void QTextDocumentExporter::writeSectionEnd(cf::CEDSection& /*section*/) {
    Q_ASSERT(doc_);

    cursor_ = doc_->rootFrame()->lastCursorPosition();
}

CharAlternatives::CharAlternatives(const QVariant& var) {
    *this = var;
}

CharAlternatives& CharAlternatives::operator=(const QVariant& var) {
    if(!var.canConvert<CharAlternatives>()) {
        CF_WARNING("invalid alternative variant given");
        return *this;
    }

    *this = var.value<CharAlternatives>();
    return *this;
}
