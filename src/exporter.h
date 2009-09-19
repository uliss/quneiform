/***************************************************************************
 *   Copyright (C) 2008 by Сергей Полтавский   *
 *   poltava@gentoo   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CIFEXPORTER_H
#define CIFEXPORTER_H

#include <ctime>
#include <string>
#include <string>
#include <sstream>

namespace CIF
{

class Cell;
class Document;
class Image;
class Page;
class Paragraph;
class Raw;
class String;
class Table;

/**
 @author Serj Poltavskiy <serge.uliss at gmail.com>
*/
class Exporter
{
    protected:
        bool m_skip_images_save;
        bool m_preserve_lines;
        std::string generate_image_name(const std::string& ext) const;
    public:
        Exporter() : m_skip_images_save(false), m_preserve_lines(false) {}
        virtual ~Exporter() {}
        virtual void exportCell(const Cell& cell) const = 0;
        virtual void exportDocument(const Document& doc) const = 0;
        virtual void exportImage(const Image& image) const = 0;
        virtual void exportPage(const Page& page) const = 0;
        virtual void exportParagraph(const Paragraph& par) const = 0;
        virtual void exportRaw(const Raw& raw) const = 0;
        virtual void exportString(const String& string) const = 0;
        virtual void exportTable(const Table& table) const = 0;
        bool isSkipImagesSave() const {
            return m_skip_images_save;
        }

        bool preserveLines() const {
            return m_preserve_lines;
        }

        void setSkipImagesSave(bool value) {
            m_skip_images_save = value;
        }

        static std::string datetime() {
            time_t t = time(0);
            tm * tmp = localtime(&t);
            if (tmp == 0)
                return "";
            char buf[200];
            if (strftime(buf, sizeof(buf), "%a %b %d %H:%M:%S %Y", tmp) == 0)
                return "";
            return std::string(buf);
        }

        void setPreserveLines(bool value) {
            m_preserve_lines = value;
        }
};

inline std::string Exporter::generate_image_name(const std::string& ext) const
{
    static int cnt = 0;
    std::ostringstream os;
    os << "image_" << ++cnt << "." << ext;
    return os.str();
}

}

#endif
