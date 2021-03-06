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

#include "iscanner.h"
#include "scanoptionvalue.h"
#include "scanoptioninfo.h"
#include "scan_debug.h"

namespace cf {

IScanner::IScanner()
{
}

IScanner::~IScanner()
{
}

const IScanner::ScanOptions& IScanner::options() const
{
    return opts_;
}

IScanner::ScanOptions& IScanner::options()
{
    return opts_;
}

bool IScanner::hasOption(const std::string& name) const
{
    return findOption(name) !=  opts_.end();
}

bool IScanner::option(const std::string& name, bool * value) const
{
    OptionIteratorConst it = findOption(name);
    if(it == opts_.end())
        return false;

    if(value)
        *value = it->value()->getBool();

    return true;
}

bool IScanner::option(const std::string& name, int * value) const
{
    OptionIteratorConst it = findOption(name);
    if(it == opts_.end())
        return false;

    if(value)
        *value = it->value()->getInt();

    return true;
}

bool IScanner::option(const std::string& name, float * value) const
{
    OptionIteratorConst it = findOption(name);
    if(it == opts_.end())
        return false;

    if(value)
        *value = it->value()->getFloat();

    return true;
}

bool IScanner::option(const std::string& name, std::string * value) const
{
    OptionIteratorConst it = findOption(name);
    if(it == opts_.end())
        return false;

    if(value)
        *value = it->value()->getString();

    return true;
}

template<class T>
bool setOptionInner(IScanner * scanner, const std::string& name, const T& value)
{
    IScanner::OptionIterator it = scanner->findOption(name);

    if(it == scanner->opts_.end())
        return false;

    bool rc = it->setValue(value);
    if(!rc)
        return false;

    return scanner->setBackendOption(name, value);
}

bool IScanner::setOption(const std::string& name, bool value)
{
    return cf::setOptionInner(this, name, value);
}

bool IScanner::setOption(const std::string& name, int value)
{
    return cf::setOptionInner(this, name, value);
}

bool IScanner::setOption(const std::string& name, float value)
{
    return cf::setOptionInner(this, name, value);
}

bool IScanner::setOption(const std::string &name, const std::string& value)
{
    return cf::setOptionInner(this, name, value);
}

void IScanner::clearOptions()
{
    opts_.clear();
}

std::ostream& IScanner::dumpOptions(std::ostream& os) const
{
    for(OptionIteratorConst it = opts_.begin(); it != opts_.end(); ++it) {
        os << (*it) << "\n";
    }

    return os;
}

IScanner::OptionIterator IScanner::findOption(const std::string& name)
{
    for(OptionIterator it = opts_.begin(); it != opts_.end(); ++it) {
        if(it->name() == name)
            return it;
    }

    return opts_.end();
}

IScanner::OptionIteratorConst IScanner::findOption(const std::string &name) const
{
    for(OptionIteratorConst it = opts_.begin(); it != opts_.end(); ++it) {
        if(it->name() == name)
            return it;
    }

    return opts_.end();
}

}
