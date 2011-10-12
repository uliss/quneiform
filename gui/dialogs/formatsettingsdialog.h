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

#ifndef FORMATSETTINGSDIALOG_H
#define FORMATSETTINGSDIALOG_H

#include <QDialog>
#include <QList>

namespace Ui {
    class FormatSettingsDialog;
}

class Page;
class FormatSettings;
class QCheckBox;

class FormatSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FormatSettingsDialog();
    ~FormatSettingsDialog();

    /**
      * Setup dialog for single page
      */
    void setup(Page * page);

    /**
      * Setup dialog for pages
      */
    void setup(const QList<Page*> pages);
private slots:
    void save();
    void load();
private:
    typedef bool (FormatSettings::*getFuncPtr)() const;
    typedef void (FormatSettings::*setFuncPtr)(bool);
    void loadAlternatives();
    void loadFonts();
    void loadFormat();
    void saveAlternatives();
    void saveFonts();
    void saveFormat();
    void saveCheckboxState(QCheckBox * checkbox, setFuncPtr);
    void setCheckboxState(QCheckBox * checkbox, getFuncPtr);
    void setUnrecognzedChar(const QChar& c);
private:
    Ui::FormatSettingsDialog * ui_;
    QList<Page*> pages_;
};

#endif // FORMATSETTINGSDIALOG_H
