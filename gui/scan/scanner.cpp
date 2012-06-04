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

#include "scanner.h"
#include "scan/iscanner.h"
#include "scan/sanescanner.h"
#include "scan/scanoptioninfo.h"

using namespace cf;

Scanner::Scanner(QObject *parent) :
    QObject(parent),
    backend_(NULL)
{
    backend_ = new cf::SaneScanner;
}

Scanner::~Scanner()
{
    close();
    delete backend_;
}

QImage Scanner::start()
{
    return QImage();
}

static inline ScannerOption::Type cfToQt(const cf::ScanOptionInfo::Type t)
{
    switch(t) {
    case ScanOptionInfo::BOOL:
        return ScannerOption::BOOL;
    case ScanOptionInfo::INT:
        return ScannerOption::INT;
    case ScanOptionInfo::FLOAT:
        return ScannerOption::FLOAT;
    case ScanOptionInfo::STRING:
        return ScannerOption::STRING;
    default:
        return ScannerOption::UNKNOWN;
    }
}

static QVariant cfToQt(const cf::ScanOptionInfo * info, const cf::ScanOptionInfo::Value& v)
{
    switch(info->type()) {
    case ScanOptionInfo::BOOL:
        return ScanOptionInfo::to<bool>(v);
    case ScanOptionInfo::INT:
        return ScanOptionInfo::to<int>(v);
    case ScanOptionInfo::FLOAT:
        return ScanOptionInfo::to<float>(v);
    case ScanOptionInfo::STRING:
        return QString::fromStdString(ScanOptionInfo::to<std::string>(v));
    default:
        return QVariant();
    }
}

static void cfInfoToQt(const cf::ScanOptionInfo * info, ScannerOption& opt)
{
    if(!info)
        return;

    if(info->isConstraintList()) {
        cf::ScanOptionInfo::ValueList vals = info->allowedValues();
        for(size_t i = 0; i < vals.size(); i++) {
            opt.addAllowedValue(cfToQt(info, vals[i]));
        }
    }
}

static ScannerOption cfScanOptionToQf(const cf::ScanOption& cf_opt)
{
    ScannerOption res;
    res.setName(cf_opt.name().c_str());
    res.setType(cfToQt(cf_opt.info()->type()));
    cfInfoToQt(cf_opt.info(), res);
    return res;
}

void Scanner::collectOptions()
{
    if(!backend_)
        return;

    IScanner::ScanOptions opts = backend_->options();

    for(size_t i = 0; i < opts.size(); i++) {
        ScannerOption opt = cfScanOptionToQf(opts[i]);
        opt.setValid();
        options_.insert(opt.name(), opt);
    }
}

QStringList Scanner::listDevices() const
{
    QStringList res;
    SaneScanner s;
    SaneScanner::DeviceList devices = s.listDevices();

    for(size_t i = 0; i < devices.size(); i++) {
        res << QString::fromStdString(devices[i]);
    }

    return res;
}

ScannerOption Scanner::option(const QString& name) const
{
    if(options_.contains(name))
        return options_.value(name);

    return ScannerOption();
}

bool Scanner::setOption(const QString& name, const QVariant& value)
{
    if(backend_)
        return false;

    if(!options_.contains(name)) {
        qWarning() << Q_FUNC_INFO << "option '" << name << "' not found";
        return false;
    }

    switch(options_[name].type()) {
    case ScannerOption::BOOL: {
        bool v = value.toBool();

        if(!backend_->setOption(name.toStdString(), v)) {
            qDebug() << Q_FUNC_INFO << "can't set option" << name << "to value:" << v;
            return false;

        }
        break;
    }
    case ScannerOption::INT: {
        bool ok = false;
        int v = value.toInt(&ok);

        if(!ok) {
            qDebug() << Q_FUNC_INFO << "non int value given";
            return false;
        }

        if(!backend_->setOption(name.toStdString(), v)) {
            qDebug() << Q_FUNC_INFO << "can't set option" << name << "to value:" << v;
            return false;
        }

        break;
    }
    default:
        qDebug() << Q_FUNC_INFO << "unsupported option type";
        return false;
    }

    options_[name].setValue(value);

    return true;
}

bool Scanner::setOption(const QString& name, bool value)
{
    return setOption(name, QVariant(value));
}

QString Scanner::name() const
{
    return name_;
}

void Scanner::close()
{
    if(backend_)
        backend_->close();
}

bool Scanner::open(const QString& name)
{
    bool rc = backend_->open(name.toStdString());

    if(!rc)
        return false;

    collectOptions();

    name_ = name;

    return true;
}
