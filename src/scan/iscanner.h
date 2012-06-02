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
#include "common/rect.h"
#include "common/image.h"

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
     * Returns option value
     * @param name - option name
     * @param value - pointer to result
     * @return true on success
     */
    bool option(const std::string& name, bool * value) const;
    bool option(const std::string& name, int * value) const;
    bool option(const std::string& name, float * value) const;
    bool option(const std::string& name, std::string * value) const;

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

    /**
     * Returns scanning area in mm
     * @return invalid retangle on error
     * @see setScanArea()
     */
    virtual Rect scanArea() const = 0;

    /**
     * Sets scanning area in mm
     * @return true on success, false on error
     * @see scanArea()
     */
    virtual bool setScanArea(const Rect& area) = 0;

    virtual ImagePtr start() = 0;

    void clearOptions();
    std::ostream& dumpOptions(std::ostream& os) const;
protected:
    virtual bool setBackendOption(const std::string& name, bool value) = 0;
    virtual bool setBackendOption(const std::string& name, float value) = 0;
    virtual bool setBackendOption(const std::string& name, const std::string& value) = 0;
    OptionIterator findOption(const std::string& name);
    OptionIteratorConst findOption(const std::string& name) const;
protected:
    ScanOptions opts_;
};

}

#endif // ISCANNER_H
