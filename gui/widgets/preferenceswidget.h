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

#ifndef PREFERENCESWIDGET_H
#define PREFERENCESWIDGET_H

#include <QWidget>
#include <QIcon>
#include <QMap>
#include <QVariant>

class PreferencesWidget : public QWidget
{
    Q_OBJECT
public:
    typedef bool (*LoadCallback)(QWidget*, const QVariant&);
    typedef bool (*SaveCallback)(QWidget*, const QVariant&);

    class Callbacks
    {
        LoadCallback load_;
        SaveCallback save_;
        QVariant parameter_;
    public:
        Callbacks(LoadCallback load = 0, SaveCallback save = 0, const QVariant& data = QVariant()) :
            load_(load),
            save_(save),
            parameter_(data)
        {}

        bool load(QWidget * w) const
        {
            if(load_)
                return load_(w, parameter_);
            return false;
        }

        bool save(QWidget * w) const
        {
            if(save_)
                return save_(w, parameter_);
            return false;
        }
    };
public:
    PreferencesWidget(QWidget * parent = 0);

    /**
     * Returns preferences icon
     * @see setIcon()
     */
    QIcon icon() const;

    /**
     * Sets preferences icon
     * @see icon()
     */
    void setIcon(const QIcon& icon);

    /**
     * Sets preferences title
     * @see title(), setIcon()
     */
    void setTitle(const QString& title);

    /**
     * Returns preferences title
     * @see setTitle()
     */
    QString title() const;

    /**
     * Connects given widget signal and load/save callbacks
     * @param control - widget to connect
     * @param signal - connected signal
     * @param callbacks - callbacks struct
     */
    void connectControl(QWidget * control, const char * signal, const Callbacks& callbacks);

    /**
     * Disconnects widget callbacks
     */
    void disconnectControl(QWidget * control);
signals:
    /**
     * Emitted when connected preferences control emits signal
     * @see connectControl()
     */
    void changed(QWidget*);
public slots:
    /**
     * Calls load callbacks for all connected widgets
     * @see connectControl(), saveAll()
     */
    void loadAll();

    /**
     * Calls load callback for given widget
     */
    void load(QWidget * control);

    /**
     * Calls save callback for given widget
     * @see saveAll()
     */
    void save(QWidget * control);

    /**
     * Calls save callbacks for all connected widgets
     * @see loadAll()
     */
    void saveAll();
protected:
    static Callbacks standartCallbacks(const QVariant& data = QVariant());
    static bool standartLoadCallback(QWidget * w, const QVariant& data);
    static bool standartSaveCallback(QWidget * w, const QVariant& data);
private slots:
    void handleControlSignal();
private:
    void addCallbacks(QWidget * widget, const Callbacks& callbacks);
    bool hasCallbacks(QWidget * widget) const;
    void removeCallbacks(QWidget * widget);
private:
    QMap<QWidget*, Callbacks> callbacks_;
    QString title_;
    QIcon icon_;
};

#endif // PREFERENCESWIDGET_H
