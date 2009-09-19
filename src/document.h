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
#ifndef CIFDOCUMENT_H
#define CIFDOCUMENT_H

#include <string>
#include <list>

#include "blockelement.h"
#include "page.h"

namespace CIF
{

/**
    @author
*/
class Document : public BlockElement
{
    public:
        typedef std::list<std::string> LangList;
        
        /**
         * 
         */
        Document();
        
        /**
         * Adds new document language
         * @param lang 
         */
        void addLanguage(const std::string& lang);
        
        /**
         * Returns document author
         * @return 
         */
        std::string author() const;

        /**
         * Returns document encoding
         * @return
         */
        std::string encoding() const;

        /**
         * Exports document
         * @param exp
         */
        void exportTo(Exporter& exp) const;

        /**
         * Returns languages of document
         * @return
         */
        LangList languageList() const;
        
        /**
         * Sets document author
         * @param author 
         */
        void setAuthor(const std::string& author);

        /**
         * Sets document encoding
         * @param value
         */
        void setEncoding(const std::string& value);
        
        /**
         * Sets language. Clears all previous languages and sets new one.
         * @param  
         */
        void setLanguage(const std::string& lang);

        /**
         * Sets document title
         * @param value
         */
        void setTitle(const std::string& value);

        /**
         * Returns document title
         * @return
         */
        std::string title() const;
    private:
        std::string m_encoding;
        std::string m_title;
        std::string m_author;
        LangList m_langs;
        bool isValid(const ElementPtr& ptr) const;
};

}

#endif
