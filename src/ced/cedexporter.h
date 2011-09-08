/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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

#ifndef CEDEXPORTER_H_
#define CEDEXPORTER_H_

#include "globus.h"
#include "cedfwd.h"
#include "cedpageptr.h"
#include <boost/noncopyable.hpp>

namespace cf
{

class CLA_EXPO CEDExporter: public boost::noncopyable
{
    public:
        CEDExporter(CEDPagePtr p) : page_(p) {}
        virtual ~CEDExporter() {}
        virtual void exportChar(CEDChar& chr) = 0;
        virtual void exportColumn(CEDColumn& col) = 0;
        virtual void exportFrame(CEDFrame& frame) = 0;
        virtual void exportLine(CEDLine& line) = 0;
        virtual void exportPage(CEDPage& page) = 0;
        virtual void exportParagraph(CEDParagraph& par) = 0;
        virtual void exportPicture(CEDPicture& pict) = 0;
        virtual void exportSection(CEDSection& sect) = 0;
        virtual void exportTable(CEDTable& table) = 0;
        CEDPagePtr page() { return page_; }
        void setPage(CEDPagePtr p) { page_ = p; }
    private:
        CEDPagePtr page_;
};

}

#endif /* CEDEXPORTER_H_ */
