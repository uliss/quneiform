/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef CIFCONFIG_H_
#define CIFCONFIG_H_

#include "singleton.h"

namespace CIF
{

class ConfigImpl
{
    public:
        ConfigImpl() :
                verbose_(false) {
        }

        bool verbose() const {
            return verbose_;
        }

        void setVerbose(bool value) {
            verbose_ = value;
        }
    private:
        bool verbose_;
};

typedef Singleton<ConfigImpl> Config;

}

#endif /* CIFCONFIG_H_ */
