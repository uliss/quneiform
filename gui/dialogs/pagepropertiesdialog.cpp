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

#include "pagepropertiesdialog.h"
#include "ui_pagepropertiesdialog.h"

#include "page.h"
#include "recognitioninternaldata.h"

PagePropertiesDialog::PagePropertiesDialog(Page * p, QWidget * parent) :
    QDialog(parent),
    ui_(new Ui::PagePropertiesDialog)
{
    ui_->setupUi(this);
    ui_->file->setText(p->imagePath());

    if(!p->imageURL().isSimple()) {
        // image numbers starts from 0, we shows them starting from 1
        QLabel * image_num = new QLabel(QString("%1").arg(p->imageURL().imageNumber() + 1));
        ui_->formLayout->insertRow(1, tr("Image number:"), image_num);
    }

    QSize sz = p->imageSize();
    ui_->imageSize->setText(QString("%1x%2").arg(sz.width()).arg(sz.height()));
    ui_->rotation->setNum(p->angle());
    ui_->recognized->setText(p->isRecognized() ? tr("Yes") : tr("No"));
    ui_->saved->setText(p->isExported() ? tr("Yes") : tr("No"));
    ui_->language->setText(p->language().trName());
    ui_->sections->setNum(p->blocksCount(BLOCK_SECTION));
    ui_->paragraphs->setNum(p->blocksCount(BLOCK_PARAGRAPH));
    ui_->characters->setNum(p->blocksCount(BLOCK_CHAR));
    ui_->images->setNum(p->blocksCount(BLOCK_PICTURE));
#ifndef NDEBUG
    ui_->heightHistogram->setData(RecognitionInternal::instance().componetHeightHistogram(p->imagePath()));
    ui_->widthHistogram->setData(RecognitionInternal::instance().componetWidthHistogram(p->imagePath()));
#endif

}

PagePropertiesDialog::~PagePropertiesDialog()
{
    delete ui_;
}
