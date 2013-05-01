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
#include <QComboBox>
#include <QSlider>
#include <QSettings>

#include "preferenceswidget.h"

PreferencesWidget::PreferencesWidget(QWidget * parent) :
    QWidget(parent)
{
}

QIcon PreferencesWidget::icon() const
{
    return icon_;
}

void PreferencesWidget::setIcon(const QIcon& icon)
{
    icon_ = icon;
}

void PreferencesWidget::setTitle(const QString& title)
{
    title_ = title;
}

QString PreferencesWidget::title() const
{
    return title_;
}

void PreferencesWidget::loadAll()
{
    foreach(QWidget * control, callbacks_.keys()) {
        Callbacks cb = callbacks_.value(control);
        cb.load(control);
    }
}

void PreferencesWidget::load(QWidget * control)
{
    if(!control) {
        qWarning() << Q_FUNC_INFO << "Null widget given";
        return;
    }

    if(!hasCallbacks(control)) {
        qWarning() << Q_FUNC_INFO << "given widget contains has no callbacks:" << control;
        return;
    }

    Callbacks cb = callbacks_.value(control);
    cb.load(control);
}

void PreferencesWidget::save(QWidget * control)
{
    if(!control) {
        qWarning() << Q_FUNC_INFO << "Null widget given";
        return;
    }

    if(!hasCallbacks(control)) {
        qWarning() << Q_FUNC_INFO << "given widget contains has no callbacks:" << control;
        return;
    }

    Callbacks cb = callbacks_.value(control);
    cb.save(control);
}

void PreferencesWidget::saveAll()
{
    foreach(QWidget * control, callbacks_.keys()) {
        Callbacks cb = callbacks_.value(control);
        cb.save(control);
    }
}

PreferencesWidget::Callbacks PreferencesWidget::standartCallbacks(const QVariant& data)
{
    return Callbacks(&standartLoadCallback, &standartSaveCallback, data);
}

bool PreferencesWidget::standartLoadCallback(QWidget * w, const QVariant& data)
{
    if(!w) {
        qWarning() << Q_FUNC_INFO << "NULL widget given";
        return false;
    }

    QString class_name = w->metaObject()->className();

    if(class_name == "QCheckBox") {
        QCheckBox * cb = static_cast<QCheckBox*>(w);
        QString key = data.toString();
        if(key.isEmpty()) {
            qWarning() << Q_FUNC_INFO << "widget name is empty. Don't know how to load it.";
            return false;
        }

        cb->setChecked(QSettings().value(key, false).toBool());
        return true;
    }

    if(class_name == "QComboBox") {
        QComboBox * cb = static_cast<QComboBox*>(w);
        QString key = data.toString();
        if(key.isEmpty()) {
            qWarning() << Q_FUNC_INFO << "widget name is empty. Don't know how to load it.";
            return false;
        }

        cb->setCurrentIndex(QSettings().value(key, 0).toInt());
    }

    if(class_name == "QSlider") {
        QSlider * sl = static_cast<QSlider*>(w);
        QString key = data.toString();
        if(key.isEmpty()) {
            qWarning() << Q_FUNC_INFO << "widget name is empty. Don't know how to load it.";
            return false;
        }

        sl->setSliderPosition(QSettings().value(key, 80).toInt());
    }

    return false;
}

bool PreferencesWidget::standartSaveCallback(QWidget * w, const QVariant& data)
{
    if(!w) {
        qWarning() << Q_FUNC_INFO << "NULL widget given";
        return false;
    }

    QString class_name = w->metaObject()->className();

    if(class_name == "QCheckBox") {
        QCheckBox * cb = static_cast<QCheckBox*>(w);
        QString key = data.toString();
        if(key.isEmpty()) {
            qWarning() << Q_FUNC_INFO << "widget name is empty. Don't know how to load it.";
            return false;
        }

        QSettings().setValue(key, cb->isChecked());
    }

    if(class_name == "QComboBox") {
        QComboBox * cb = static_cast<QComboBox*>(w);
        QString key = data.toString();
        if(key.isEmpty()) {
            qWarning() << Q_FUNC_INFO << "widget name is empty. Don't know how to load it.";
            return false;
        }

        QSettings().setValue(key, cb->currentIndex());
    }

    if(class_name == "QSlider") {
        QSlider * sl = static_cast<QSlider*>(w);
        QString key = data.toString();
        if(key.isEmpty()) {
            qWarning() << Q_FUNC_INFO << "widget name is empty. Don't know how to load it.";
            return false;
        }

        QSettings().setValue(key, sl->sliderPosition());
    }

    return false;
}

void PreferencesWidget::connectControl(QWidget * control,
                                       const char * signal,
                                       const Callbacks& callbacks)
{
    if(!control) {
        qWarning() << Q_FUNC_INFO << "NULL control widget given";
        return;
    }

    if(!signal) {
        qWarning() << Q_FUNC_INFO << "NULL signal given";
        return;
    }

    control->setParent(this);
    connect(control, signal, this, SLOT(handleControlSignal()));
    addCallbacks(control, callbacks);
    load(control);
}

void PreferencesWidget::disconnectControl(QWidget * control)
{
    if(!control) {
        qWarning() << Q_FUNC_INFO << "NULL control widget given";
        return;
    }

    if(!hasCallbacks(control)) {
        qWarning() << Q_FUNC_INFO << "no callbacks for given widget:" << control;
        return;
    }

    disconnect(control);
    removeCallbacks(control);
}

void PreferencesWidget::handleControlSignal()
{
    QWidget * control = qobject_cast<QWidget*>(sender());
    if(!control) {
        qCritical() << Q_FUNC_INFO << "invalid signal sender:" << sender();
        return;
    }

    if(!hasCallbacks(control)) {
        qCritical() << Q_FUNC_INFO << "callbacks not set for widget:" << control;
        return;
    }

    emit changed(control);
}

bool PreferencesWidget::hasCallbacks(QWidget * widget) const
{
    return callbacks_.contains(widget);
}

void PreferencesWidget::removeCallbacks(QWidget * widget)
{
    callbacks_.remove(widget);
}

void PreferencesWidget::addCallbacks(QWidget * widget, const Callbacks& callbacks)
{
    if(!widget) {
        qWarning() << Q_FUNC_INFO << "NULL widget given";
        return;
    }

    callbacks_.insert(widget, callbacks);
}
