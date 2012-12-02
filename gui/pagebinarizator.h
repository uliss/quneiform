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

#ifndef PAGEBINARIZATOR_H
#define PAGEBINARIZATOR_H

#include <QObject>
#include <QImage>

class BinarizationSettings;
class ImageURL;
class Page;

class PageBinarizator : public QObject
{
    Q_OBJECT
public:
    explicit PageBinarizator(QObject * parent = 0);
    bool binarize(Page * page);
    QImage binarize(const ImageURL& path, const BinarizationSettings& settings);
};

#endif // PAGEBINARIZATOR_H
