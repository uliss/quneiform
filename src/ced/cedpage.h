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

#ifndef CEDPAGE_H_
#define CEDPAGE_H_

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "blockelement.h"
#include "common/size.h"
#include "common/font.h"

namespace cf
{

class CEDSection;
class CEDPage;

typedef boost::shared_ptr<CEDPage> CEDPagePtr;

class CLA_EXPO CEDPage: public BlockElement
{
    public:
        CEDPage();

        /**
         * Adds section to page
         */
        void addSection(CEDSection * sect);

        void exportElement(CEDExporter& exp);

        /**
         * Returns image dpi
         * @see setImageDpi()
         */
        Size imageDpi() const;

        /**
         * Returns image name
         * @see setImageName()
         */
        std::string imageName() const;

        /**
         * Returns size of the original image in pixels
         * @see setImageSize()
         */
        Size imageSize() const;

        /**
         * Returns page language
         * @see setLanguage
         */
        language_t language() const;

        /**
         * Returns page borders in twips
         * @see pageSize(), setPageBorder()
         */
        Rect pageBorder() const;

        /**
         * Number of Pages (= 0 not in batch mode)
         * @see setPageNumber()
         */
        int pageNumber() const;

        /**
         * Returns page size in twips
         * @note 1 inch == 1440 twips for text editor
         * @see setPageSize()
         */
        Size pageSize() const;

        /**
         * Returns pointer to section with given number
         */
        CEDSection * sectionAt(size_t pos);

        /**
         * Returns number of sections
         */
        size_t sectionCount() const;

        /**
         * sets new value of current section
         */
        CEDSection * setCurrentSection(int number);

        /**
         * Sets image dpi
         * @see imageDpi()
         */
        void setImageDpi(const Size& dpi);

        /**
         * Sets image path
         * If the path is not specified, is searched in one
         * Directory with the file ed
         */
        void setImageName(const std::string& filename);

        /**
         * Sets image size in pixels
         * @see imageSize()
         */
        void setImageSize(const Size& size);

        /**
         * Sets page language
         * @see language()
         */
        void setLanguage(language_t language);

        /**
         * Sets page borders in twips
         * @see pageBorder()
         */
        void setPageBorder(const Rect& borders);

        /**
         * Sets page number
         * @see pageNumber()
         */
        void setPageNumber(int number);

        /**
         * Sets page size in twips
         * @see pageSize()
         */
        void setPageSize(const Size& size);

        /**
         * Sets page turn
         * @see turn()
         * @param angle  - tangent angle on the vertical images * 2048
         */
        void setTurn(int angle);

        /**
         * Returns tangent angle on the vertical images * 2048
         * @see setTurn()
         */
        int turn() const;
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            using boost::serialization::make_nvp;
            ar.template register_type<CEDSection>();
            ar & make_nvp("block-element", boost::serialization::base_object<BlockElement>(*this));
            ar & make_nvp("image-name", image_name_);
            ar & make_nvp("image-size", image_size_);
            ar & make_nvp("image-dpi", image_dpi_);
            ar & make_nvp("page-size", page_size_);
            ar & make_nvp("page-borders", page_borders_);
            ar & make_nvp("page-number", page_number_);
            ar & make_nvp("turn", turn_);
            ar & make_nvp("language", language_);
        }
#endif
    private:
        Size image_size_;
        Size image_dpi_;
        Size page_size_;
        Rect page_borders_;
        std::string image_name_;
        int turn_;
        int page_number_;
        language_t language_;
};

}

#endif /* CEDPAGE_H_ */
