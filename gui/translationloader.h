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

#ifndef TRANSLATIONLOADER_H
#define TRANSLATIONLOADER_H

#include <QTranslator>
#include <QStringList>
#include <QLocale>

class TranslationLoader
{
public:
    TranslationLoader();

    QTranslator * applicationTranslator();
    QTranslator * systemTranslator();

    void load();
private:
    void loadApplicationTranslation();
    bool loadApplicationTranslation(const QString& path);
    void loadApplicationTranslations(const QStringList& paths);
    void loadSystemTranslation();
private:
    QLocale locale_;
private:
    static QTranslator app_;
    static QTranslator system_;
};

#endif // TRANSLATIONLOADER_H
