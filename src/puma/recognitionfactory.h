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

#ifndef RECOGNITIONFACTORY_H
#define RECOGNITIONFACTORY_H

#include <boost/shared_ptr.hpp>

#include "globus.h"
#include "common/singleton.h"
#include "abstractrecognitionserver.h"

namespace cf {

typedef boost::shared_ptr<AbstractRecognitionServer> RecognitionPtr;

enum RecognitionServerType {
    SERVER_NULL = 0,
    SERVER_DUMMY,
    SERVER_LOCAL,
    SERVER_PROCESS
};

class CLA_EXPO RecognitionFactoryImpl
{
public:
    RecognitionPtr make(RecognitionServerType type);
private:
    RecognitionPtr ptr_;
};

typedef Singleton<RecognitionFactoryImpl> RecognitionFactory;

}

#endif // RECOGNITIONFACTORY_H
