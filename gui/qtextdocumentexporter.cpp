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
#include "qtextdocumentexporter.h"
#include "ced/cedchar.h"
#include "ced/cedline.h"
#include "ced/cedpage.h"
#include "ced/cedparagraph.h"
#include "ced/cedpicture.h"
#include "ced/cedsection.h"
#include "ced/cedcolumn.h"
#include "common/tostring.h"
#include "export/rout_own.h"
#include "rdib/bmp.h"
#include "compat_defs.h"

using namespace cf;

static const char * SOFT_HYPEN = "\xAD";

static inline QRect toQRect(const cf::Rect& r) {
    return QRect(r.x(), r.y(), r.width(), r.height());
}

static inline QColor toQColor(const cf::Color& c) {
    return QColor(c.red(), c.green(), c.blue());
}

QTextDocumentExporter::QTextDocumentExporter(CEDPage * page, const FormatOptions& opts) :
        GenericExporter(page, opts),
        doc_(NULL),
        cursor_(NULL),
        current_col_num_(0),
        line_num_in_par_(0),
        do_column_layout_(false)
{
}

QTextDocumentExporter::~QTextDocumentExporter() {
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

    // set tooltip
    if(formatOptions().showAlternatives()) {
        AltMap alt_map = charAlternatives(chr);

        QString tooltip = "Alternatives:";

        foreach(QString alt, alt_map.keys()) {
            tooltip.append(QString("\n        '%1' (%2)").arg(alt).arg(alt_map.value(alt).toInt()));
        }

        format.setToolTip(tooltip);
        format.setUnderlineStyle(QTextCharFormat::DashUnderline);
        QSettings settings;
        settings.beginGroup("format");
        format.setUnderlineColor(settings.value("alternativeColor", Qt::red).value<QColor>());

        format.setProperty(ALTERNATIVES, QVariant(alt_map));
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

QTextCursor * QTextDocumentExporter::cursor() {
    return cursor_;
}

QTextDocument * QTextDocumentExporter::document() {
    return doc_;
}

void QTextDocumentExporter::setDocument(QTextDocument * doc) {
    Q_ASSERT(doc);

    if(doc == doc_)
        return;

    doc_ = doc;
    delete cursor_;
    cursor_ = new QTextCursor(doc_);
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

    cursor_->insertText(str, format);
}

void QTextDocumentExporter::writeColumnBegin(cf::CEDColumn&) {
    if(!do_column_layout_)
        return;

    if(current_col_num_++ == 0)
        return;

    QTextTable * table = cursor_->currentTable();
    if(!table) {
        qDebug() << Q_FUNC_INFO << "no current table";
    } else {
        table->appendColumns(1);
    }
}

void QTextDocumentExporter::writeLineEnd(cf::CEDLine& line) {
    if(line.empty())
        return;

    if(formatOptions().preserveLineBreaks()) {
        if(line_num_in_par_++ != 0) {
            QTextBlockFormat current_fmt = cursor_->blockFormat();
            current_fmt.setTextIndent(0);
            cursor_->setBlockFormat(current_fmt);
        }

        cursor_->insertText("\n", cursor_->charFormat());
    }
    else {
        if(line.endsWithHyphen()) {
            cursor_->deletePreviousChar();
            cursor_->insertText(SOFT_HYPEN);
        }
        else {
            cursor_->insertText(" "); // space
        }
    }
}

void QTextDocumentExporter::writePageBegin(cf::CEDPage& page) {
    QTextFrameFormat format;

    if(page.imageSize().isValid()) {
//        format.setWidth(page.imageSize().width());
//        format.setHeight(page.imageSize().height());
    }

    format.setTopMargin(page.marginTop());
    format.setRightMargin(page.marginRight());
    format.setBottomMargin(page.marginBottom());
    format.setLeftMargin(page.marginLeft());

    cursor_->insertFrame(format);
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

    cursor_->insertBlock(format);
    cursor_->movePosition(QTextCursor::StartOfBlock);
    line_num_in_par_ = 0;
}

void QTextDocumentExporter::writePicture(cf::CEDPicture& pic) {
    if(!pic.image()) {
        qDebug() << Q_FUNC_INFO << "empty image given";
        return;
    }

    ImagePtr im_ptr = pic.image();

    BITMAPFILEHEADER bf; //  bmp fileheader
    BITMAPINFOHEADER * bfinfo = (BITMAPINFOHEADER *) im_ptr->data();
    bf.bfType = 0x4d42; // 'BM'
    bf.bfSize = sizeof(BITMAPFILEHEADER) + im_ptr->dataSize();
    // fileheader + infoheader + palette
    bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bfinfo->biClrUsed
            * sizeof(RGBQUAD);

    std::ostringstream buf;
    buf.write((char*) &bf, sizeof(bf));
    buf << *im_ptr;

//    if(im_ptr->width() == 0 || im_ptr->height() == 0) {
//        qDebug() << Q_FUNC_INFO << "invalid image size: (" << im_ptr->width() << im_ptr->height() << ")";
//        return;
//    }
    std::string s = buf.str();
    QImage img = QImage::fromData((uchar*)s.data(), s.size());

    if(img.isNull()) {
        qDebug() << Q_FUNC_INFO << "invalid image given";
        return;
    }

    static int image_counter = 0;

    cursor_->insertImage(img, QString("image %1").arg(++image_counter));
    QTextFrameFormat format;
    cursor_->currentFrame()->setFrameFormat(format);
}

void QTextDocumentExporter::writeSectionBegin(cf::CEDSection& section) {
    current_col_num_ = 0;
    do_column_layout_ = (section.columnCount() > 1);

    if(do_column_layout_) {
        QTextTableFormat format;
        format.setAlignment(Qt::AlignHCenter);
        format.setBorderStyle(QTextFrameFormat::BorderStyle_Dotted);
        format.setBorder(1);
        format.setCellPadding(3);
        format.setCellSpacing(0);

        QVector<QTextLength> col_wd;
        for(size_t i = 0; i < section.columnCount(); i++) {
            CEDColumn * col = section.columnAt(i);
            if(col->width() > 0)
                col_wd.append(QTextLength(QTextLength::FixedLength, col->width()));
            else
                col_wd.append(QTextLength(QTextLength::VariableLength, 300));
        }

        format.setColumnWidthConstraints(col_wd);

        cursor_->insertTable(1, 1, format);
    }
}
