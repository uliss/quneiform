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
#include "quneiform_debug.h"
#include "qtextdocumentexporter.h"
#include "common/language.h"
#include "common/fontstyle.h"
#include "ced/cedpage.h"
#include "ced/cedsection.h"
#include "ced/cedcolumn.h"
#include "ced/cedparagraph.h"
#include "ced/cedline.h"
#include "ced/cedchar.h"

static inline cf::Rect toCfRect(const QRect& r) {
    return cf::Rect(cf::Point(r.x(), r.y()), r.width(), r.height());
}

static cf::Rect getFormatMargins(const QTextFrameFormat& format) {
    return cf::Rect(cf::Point((int) format.leftMargin(), (int) format.topMargin()),
                    cf::Point((int) format.rightMargin(), (int) format.bottomMargin()));
}

static cf::Language toCfLanguage(const ::Language& language) {
    return cf::Language(static_cast<language_t>(language.code()));
}

CEDPageExporter::CEDPageExporter() :
    doc_(NULL),
    page_(NULL),
    iconv_(new cf::Iconv) {
    setLanguage(Language::english());
}

CEDPageExporter::CEDPageExporter(const Language& lang) :
    doc_(NULL),
    page_(NULL),
    iconv_(new cf::Iconv) {
    setLanguage(lang);
}

CEDPageExporter::~CEDPageExporter() {
    delete iconv_;
}

std::string CEDPageExporter::convert(const QString& str) {
    Q_CHECK_PTR(iconv_);

    std::string res;
    try {
        res = iconv_->convert(str.toUtf8().data());
    }
    catch(std::exception& e) {
        qDebug() << Q_FUNC_INFO << e.what();
        return std::string();
    }

    return res;
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
    Q_CHECK_PTR(iconv_);
    doc_ = doc;
    page_ = page;
    cursor_ = QTextCursor(doc);
    iconv_->open("utf-8", toCfLanguage(language()).encoding());
    exportPage();
}

void CEDPageExporter::exportBBox(cf::Element * el, const QTextFormat& fmt) {
    Q_CHECK_PTR(el);

    if(!fmt.hasProperty(QTextDocumentExporter::BBOX))
        return;

    QRect bbox = fmt.property(QTextDocumentExporter::BBOX).toRect();
    el->setBoundingRect(toCfRect(bbox));
}

void CEDPageExporter::exportBlock(const QTextBlock& block) {
//    qDebug() << Q_FUNC_INFO;

//    QTextBlockFormat format = block.blockFormat();
//    if(format.intProperty(QTextDocumentExporter::BlockType) != QTextDocumentExporter::PARAGRAPH) {
//        qDebug() << Q_FUNC_INFO << "attempt to export non pargraph block";
//    }

//    qDebug() << block.text();
//    cf::CEDSection * last_section = page_->sectionAt(page_->sectionCount() - 1);
//    cf::CEDColumn * last_col = last_section->columnAt(last_section->columnCount() - 1);
//    cf::CEDParagraph * par = new cf::CEDParagraph;

//    fillPar(par, block.text());

//    last_col->addElement(par);
}

void CEDPageExporter::exportChar(const QTextFragment& fragment, cf::CEDLine * line) {
    if(fragment.length() > 1)
        CF_WARNING("invalid fragment given");

    if(fragment.charFormat().hasProperty(QTextDocumentExporter::BBOX)) {
        std::string cf_converted = convert(fragment.text());
        if(cf_converted.empty()) {
            CF_ERROR("can't convert char from utf-8");
            return;
        }

        line->addElement(makeChar(cf_converted[0], fragment.charFormat()));
    }
    else {
        exportString(fragment, line);
    }
}

void CEDPageExporter::exportCharAlternatives(cf::CEDChar * c, const QTextCharFormat& fmt) {
    Q_CHECK_PTR(c);
    if(!fmt.hasProperty(QTextDocumentExporter::ALTERNATIVES))
        return;

    CharAlternatives alternatives = fmt.property(QTextDocumentExporter::ALTERNATIVES);

    foreach(CharAlternative alt, alternatives) {
        std::string alt_str = convert(alt.code);
        if(alt_str.empty())
            continue;

        c->addAlternative(cf::Letter(alt_str[0], alt.probability));
    }
}

void CEDPageExporter::exportColumn() {

}

void CEDPageExporter::exportColumnTable(QTextTable * table) {
    if(!isSectionTable(table)) {
        qDebug() << Q_FUNC_INFO << "attept to export non section table";
        return;
    }

    QTextTableFormat table_format = table->format();

    cf::CEDSection * section = new cf::CEDSection;
    section->setMargins(getFormatMargins(table_format));

    page_->addSection(section);

    QVector<QTextLength> col_wd = table_format.columnWidthConstraints();

    for(int i = 0; i < table->columns(); i++) {
        cf::CEDColumn * col = new cf::CEDColumn;

        if(col_wd.at(i).type() == QTextLength::FixedLength)
            col->setWidth((int) col_wd.at(i).rawValue());

        section->addColumn(col);
    }
}

void CEDPageExporter::exportFontStyle(cf::CEDChar * c, const QTextCharFormat& fmt) {
    Q_CHECK_PTR(c);

    int style = 0;

    if(fmt.fontUnderline())
        style |= FONT_UNDERLINE;

    if(fmt.fontItalic())
        style |= FONT_ITALIC;

    if(fmt.fontWeight() >= QFont::DemiBold)
        style |= FONT_BOLD;

    if(fmt.fontStrikeOut())
        style |= FONT_STRIKE;

    c->setFontStyle(style);
}

void CEDPageExporter::exportMargins(cf::BlockElement * block, const QTextFrame * frame) {
    Q_CHECK_PTR(block);
    Q_CHECK_PTR(frame);
    block->setMargins(getFormatMargins(frame->frameFormat()));
}

void CEDPageExporter::exportPage() {
    // to document start
    cursor_.movePosition(QTextCursor::Start);
    QTextFrame * root = doc_->rootFrame();

    if(!isPage(root)) {
        CF_WARNING("attempt to export non page frame");
        return;
    }

    exportMargins(page_, root);
    exportPageChildren(root);
}

bool CEDPageExporter::exportPageChild(QTextFrame * child) {
    if(!child)
        return false;

    QTextTable * table = qobject_cast<QTextTable*>(child);

    if (table)
        exportColumnTable(table);
    else
        exportSection(child);

    return true;
}

void CEDPageExporter::exportPageChildren(QTextFrame * page) {
    for(QTextFrame::Iterator it = page->begin(); !it.atEnd(); ++it) {
        QTextFrame * page_child = it.currentFrame();

        if(!exportPageChild(page_child))
            CF_WARNING("ivalid block found");
    }
}

void CEDPageExporter::exportParagraph(const QTextBlock& block, cf::CEDColumn * col) {
    if(!isParagraph(block))
        CF_WARNING("attempt to export non paragraph block");

    cf::CEDParagraph * p = new cf::CEDParagraph;
    cf::CEDLine * l = new cf::CEDLine;
    p->addLine(l);
    for(QTextBlock::iterator it = block.begin(), end = block.end(); it != end; ++it) {
        QTextFragment fragment = it.fragment();
        if(fragment.length() == 1)
            exportChar(fragment, l);
        else
            exportString(fragment, l);
    }
    col->addElement(p);
}

void CEDPageExporter::exportSection(QTextFrame * section) {
    if(!isSection(section)) {
        CF_WARNING("attempt to export non section frame");
        return;
    }

    cf::CEDSection * cf_section = makeSingleColumnSectionLayout(section);
    page_->addSection(cf_section);
    exportSectionChildren(section, cf_section->columnAt(0));
}

void CEDPageExporter::exportSectionChildren(QTextFrame * section, cf::CEDColumn * col) {
    for(QTextFrame::Iterator it = section->begin(); !it.atEnd(); ++it) {
        QTextFrame * child_frame = it.currentFrame();

        if(child_frame == NULL)
            exportParagraph(it.currentBlock(), col);
        else
            CF_WARNING("nested frame found");
    }
}

void CEDPageExporter::exportString(const QTextFragment& fragment, cf::CEDLine * line) {
    Q_CHECK_PTR(line);

    QTextCharFormat fmt = fragment.charFormat();
    std::string cf_converted = convert(fragment.text());

    for(int i = 0, total = cf_converted.length(); i < total; i++)
        line->addElement(makeChar(cf_converted[i], fmt));
}

bool CEDPageExporter::isPage(QTextFrame * page) {
    if(!page)
        return false;

    return page->frameFormat().intProperty(QTextDocumentExporter::BlockType) ==
            QTextDocumentExporter::PAGE;
}

bool CEDPageExporter::isParagraph(const QTextBlock& par) {
    return par.blockFormat().intProperty(QTextDocumentExporter::BlockType) ==
            QTextDocumentExporter::PARAGRAPH;
}

bool CEDPageExporter::isSection(QTextFrame * section) {
    if(!section)
        return false;

    return section->format().intProperty(QTextDocumentExporter::BlockType) ==
            QTextDocumentExporter::SECTION;
}

bool CEDPageExporter::isSectionTable(QTextTable * table) {
    if(!table)
        return false;

    if(table->rows() != 1) {
        CF_WARNING("section table should have _one_ row");
        return false;
    }

    if(table->columns() < 1) {
        CF_WARNING("section table should have at least one column");
        return false;
    }

    return (table->format().intProperty(QTextDocumentExporter::BlockType) ==
            QTextDocumentExporter::SECTION);
}

Language CEDPageExporter::language() const {
    return language_;
}

cf::CEDSection * CEDPageExporter::makeSingleColumnSectionLayout(QTextFrame * frame) {
    cf::CEDSection * section = new cf::CEDSection;
    cf::CEDColumn * col = new cf::CEDColumn;
    section->addColumn(col);
    exportMargins(section, frame);
    return section;
}

cf::CEDChar * CEDPageExporter::makeChar(wchar_t ch, const QTextCharFormat& fmt) {
    if(ch > UCHAR_MAX)
        CF_WARNING("wide character given");

    cf::CEDChar * c = new cf::CEDChar(static_cast<uchar>(ch));
    exportBBox(c, fmt);
    exportFontStyle(c, fmt);
    exportCharAlternatives(c, fmt);
    return c;
}

cf::CEDPage * CEDPageExporter::page() {
    return page_;
}

void CEDPageExporter::setLanguage(const Language& lang) {
    language_ = lang;

    if(!lang.isValid()) {
        CF_WARNING("invalid language");
        return;
    }

    cf::Language cf_lang = toCfLanguage(lang);

    if(cf_lang.encoding().empty()) {
        CF_ERROR("invalid encoding");
        return;
    }

    iconv_->open("utf-8", cf_lang.encoding());
}

void CEDPageExporter::setPage(cf::CEDPage * page) {
    page_ = page;
}
