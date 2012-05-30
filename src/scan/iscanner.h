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

#ifndef ISCANNER_H
#define ISCANNER_H

#include <stdexcept>
#include <vector>
#include <string>
#include <iosfwd>
#include <boost/any.hpp>

#include "scanoption.h"

namespace cf {

class IScanner
{
public:
    typedef std::vector<ScanOption> ScanOptions;
    typedef ScanOptions::const_iterator OptionIteratorConst;
    typedef ScanOptions::iterator OptionIterator;

    class Exception : public std::runtime_error {
    public:
        Exception(const std::string& msg, int code = 0) :
            std::runtime_error(msg),
            code_(code) {}
        int code() const { return code_; }
    private:
        int code_;
    };

public:
    IScanner();
    virtual ~IScanner();

    const ScanOptions& options() const;
    ScanOptions& options();

    /**
     * Returns true if option exists
     * @param name - option name
     * @return true on success
     */
    bool hasOption(const std::string& name) const;

    /**
     * Sets option value
     * @param name - option name
     * @param value - option value
     * @return true on success
     */
    bool setOption(const std::string& name, bool value);
    bool setOption(const std::string &name, int value);
    bool setOption(const std::string& name, float value);
    bool setOption(const std::string& name, const std::string& value);

    void clearOptions();
    std::ostream& dumpOptions(std::ostream& os) const;
protected:
    OptionIterator findOption(const std::string& name);
    OptionIteratorConst findOption(const std::string& name) const;
protected:
    ScanOptions opts_;
};

}

#endif // ISCANNER_H
