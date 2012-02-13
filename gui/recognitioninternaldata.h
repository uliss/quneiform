/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#ifndef RECOGNITIONINTERNALDATA_H
#define RECOGNITIONINTERNALDATA_H

#include <vector>
#include <QMap>
#include <QString>

#include "common/singleton.h"

class RecognitionInternalData
{
public:
    typedef std::vector<int> Histogram;

    RecognitionInternalData();
    void clear();

    Histogram componetHeightHistogram(const QString& key) const;
    Histogram componetWidthHistogram(const QString& key) const;

    void setComponentHeightHistogram(const QString& key, const Histogram& hist);
    void setComponentWidthHistogram(const QString& key, const Histogram& hist);
private:
    typedef QMap<QString, Histogram> HistogramMap;
    HistogramMap component_height_histogram_;
    HistogramMap component_width_histogram_;
};

typedef cf::Singleton<RecognitionInternalData> RecognitionInternal;

#endif // RECOGNITIONINTERNALDATA_H
