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

#include "recognitioninternaldata.h"

#include <QDebug>

RecognitionInternalData::RecognitionInternalData()
{
}

void RecognitionInternalData::clear()
{
    component_height_histogram_.clear();
    component_width_histogram_.clear();
}

RecognitionInternalData::Histogram RecognitionInternalData::componetHeightHistogram(const QString& key) const
{
    return component_height_histogram_.find(key).value();
}

RecognitionInternalData::Histogram RecognitionInternalData::componetWidthHistogram(const QString& key) const
{
    return component_width_histogram_.find(key).value();
}

void RecognitionInternalData::setComponentHeightHistogram(const QString& key, const Histogram& hist)
{
    component_height_histogram_[key] = hist;
}

void RecognitionInternalData::setComponentWidthHistogram(const QString& key, const Histogram& hist)
{
    component_width_histogram_[key] = hist;
}
