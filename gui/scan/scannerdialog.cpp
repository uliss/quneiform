/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>

#include "scannerdialog.h"
#include "ui_scannerdialog.h"
#include "scanner.h"
#include "scanarea.h"

static const char * PROPERTY_OPTION_NAME = "OptionName";
static const char * PROPERTY_WIDGET_TYPE = "WidgetType";

ScannerDialog::ScannerDialog(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::ScannerDialog),
    scanner_(NULL),
    scan_area_(NULL)
{
    ui_->setupUi(this);
    scanner_ = new Scanner(this);
    setupUi();
    setupScanButtons();

    setupScanResolution();
    setupScanMode();
    setupScanArea();
}

ScannerDialog::~ScannerDialog()
{
    delete ui_;
    delete scanner_;
}

void ScannerDialog::addDialogOptionWidget(const QString& name, QWidget * w)
{
    if(!w) {
        qDebug() << Q_FUNC_INFO << "invalid widget given";
        return;
    }

    ui_->formLayout->addRow(name, w);
}

void ScannerDialog::setupUi()
{
    connect(this, SIGNAL(accepted()), this, SLOT(save()));


    QStringList l = scanner_->listDevices();
    foreach(QString s, l) {
        ui_->scannerComboBox->addItem(s);
    }

    l.pop_back();

    if(l.count() > 1) {
        ui_->stackedWidget->setCurrentWidget(ui_->scannerList);
    }
    else if(l.count() == 1) {
        handleScannerSelect(0);
        ui_->stackedWidget->setCurrentWidget(ui_->scanDialog);
    }

    connect(ui_->scannerComboBox,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(handleScannerSelect(int)));
}

void ScannerDialog::setupScanMode()
{
    if(!scanner_)
        return;

    ScannerOption option = scanner_->option("mode");
    if(!option.isValid())
        return;

    addDialogOptionWidget("Scan mode:", makeOptionWidget(option));
}

void ScannerDialog::setupScanResolution()
{
    if(!scanner_)
        return;

    ScannerOption option = scanner_->option("resolution");
    if(!option.isValid())
        return;

    addDialogOptionWidget("Resolution:", makeOptionWidget(option));
}

void ScannerDialog::setupScanArea()
{
    scan_area_ = new ScanArea(this);
    ui_->formLayout->addRow("", scan_area_);
}

void ScannerDialog::setupScanButtons()
{
    QPushButton * scanButton = ui_->buttonBox->addButton(tr("Scan"),
                                                         QDialogButtonBox::ActionRole);

    QPushButton * previewButton = ui_->buttonBox->addButton(tr("Preview"),
                                                            QDialogButtonBox::ActionRole);

    connect(previewButton, SIGNAL(clicked()), SLOT(handlePreview()));
}

ScannerDialog::OptionWidgetType ScannerDialog::widgetType(const ScannerOption& opt)
{
    static int type_map[5][3] = {
        //  INPUT,        LIST,           RANGE
        { UNKNOWN_WIDGET, UNKNOWN_WIDGET, UNKNOWN_WIDGET }, // UNKNOWN
        { CHECKBOX,       CHECKBOX,       CHECKBOX       }, // BOOL
        { FLOAT_INPUT,    COMBOBOX,       FLOAT_RANGE    }, // FLOAT
        { INT_INPUT,      COMBOBOX,       INT_RANGE      }, // INT
        { STRING_INPUT,   COMBOBOX,       UNKNOWN_WIDGET }  // STRING
    };

    return (OptionWidgetType) type_map[opt.type()][opt.constraint()];
}

QWidget * ScannerDialog::makeOptionWidget(const ScannerOption& opt)
{
    switch(widgetType(opt)) {
    case CHECKBOX: {
        QCheckBox * cb = new QCheckBox(this);
        cb->setChecked(opt.value().toBool());

        cb->setProperty(PROPERTY_OPTION_NAME, opt.name());
        cb->setProperty(PROPERTY_WIDGET_TYPE, (int) CHECKBOX);

        connect(cb, SIGNAL(clicked()), SLOT(handleOptionChange()));

        return cb;
    }
    case COMBOBOX: {
        QComboBox * cb = new QComboBox(this);
        foreach(QVariant v, opt.allowedValues()) {
            cb->addItem(v.toString(), v);

            if(v == opt.value())
                cb->setCurrentIndex(cb->count() - 1);
        }
        cb->setProperty(PROPERTY_OPTION_NAME, opt.name());
        cb->setProperty(PROPERTY_WIDGET_TYPE, (int) COMBOBOX);
        cb->setProperty(PROPERTY_OPTION_NAME, opt.name());
        cb->setProperty(PROPERTY_WIDGET_TYPE, (int) COMBOBOX);

        connect(cb, SIGNAL(currentIndexChanged(int)), SLOT(handleOptionChange()));

        return cb;
    }
    case FLOAT_RANGE: {
        QDoubleSpinBox * sb = new QDoubleSpinBox(this);

        if(opt.hasRangeMin())
            sb->setMinimum(opt.range().min().toFloat());

        if(opt.hasRangeMax())
            sb->setMaximum(opt.range().max().toFloat());

        if(opt.range().step().isValid())
            sb->setSingleStep(opt.range().step().toFloat());

        sb->setValue(opt.value().toFloat());

        sb->setProperty(PROPERTY_OPTION_NAME, opt.name());
        sb->setProperty(PROPERTY_WIDGET_TYPE, (int) FLOAT_RANGE);

        connect(sb, SIGNAL(valueChanged(double)), SLOT(handleOptionChange()));

        return sb;
    }
    case INT_RANGE: {
        QSpinBox * sb = new QSpinBox(this);

        if(opt.hasRangeMin())
            sb->setMinimum(opt.range().min().toInt());

        if(opt.hasRangeMax())
            sb->setMaximum(opt.range().max().toInt());

        if(opt.range().step().isValid())
            sb->setSingleStep(opt.range().step().toInt());

        sb->setValue(opt.value().toInt());

        sb->setProperty(PROPERTY_OPTION_NAME, opt.name());
        sb->setProperty(PROPERTY_WIDGET_TYPE, (int) INT_RANGE);

        connect(sb, SIGNAL(valueChanged(int)), SLOT(handleOptionChange()));

        return sb;
    }
    case UNKNOWN_WIDGET:
    default:
        qDebug() << Q_FUNC_INFO << "Unknown widget type:" << opt.type() << opt.constraint();
        return NULL;
    }
}

void ScannerDialog::handleScannerSelect(int idx)
{
    QString scanner_name = ui_->scannerComboBox->itemText(idx);
    ui_->scannerName->setText(scanner_name);

    bool rc = scanner_->open(scanner_name);

    if(!rc)
        qDebug() << "can't open scanner: " << scanner_name;
}

void ScannerDialog::handlePreview()
{
    qDebug() << Q_FUNC_INFO;

    if(!scanner_)
        return;

    QImage img = scanner_->start();

    if(img.isNull()) {
        qDebug() << Q_FUNC_INFO << "invalid image recieved";
        return;
    }

    if(!scan_area_) {
        qDebug() << Q_FUNC_INFO << "invalid scan area";
        return;
    }

    scan_area_->setPixmap(QPixmap::fromImage(img.scaled(scan_area_->size())));
}

void ScannerDialog::save()
{
    foreach(ScannerOption opt, changed_options_) {
        qDebug() << "Option " << opt.name() << "saved";
    }

    changed_options_.clear();
}

void ScannerDialog::handleOptionChange()
{
    QObject * obj = sender();

    if(!obj)
        return;

    QVariant vname = obj->property(PROPERTY_OPTION_NAME);
    if(!vname.isValid()) {
        qDebug() << Q_FUNC_INFO << "invalid sender" << obj;
        return;
    }

    QVariant vtype = obj->property(PROPERTY_WIDGET_TYPE);
    if(!vtype.isValid()) {
        qDebug() << Q_FUNC_INFO << "invalid widget" << obj;
        return;
    }

    qDebug() << Q_FUNC_INFO << "option changed: "
             << vname.toString() << "in"
             << vtype.toString() << "widget";

    if(!scanner_)
        return;

    ScannerOption opt = scanner_->option(vname.toString());
    if(!opt.isValid()) {
        qDebug() << Q_FUNC_INFO << "unknown option:" << vname.toString();
        return;
    }

    changed_options_[opt.name()] = opt;
}

