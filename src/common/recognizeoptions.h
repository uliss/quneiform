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

#ifndef RECOGNIZEOPTIONS_H_
#define RECOGNIZEOPTIONS_H_

#include <iostream>
#include <string>
#include <vector>

#include "lang_def.h"
#include "globus.h"
#include "common/rect.h"
#include "serialize.h"

#ifdef CF_SERIALIZE
#include <boost/serialization/vector.hpp>
#endif

namespace cf {

class CLA_EXPO RecognizeOptions
{
    public:
        RecognizeOptions();

        enum table_mode_t
        {
            TABLE_NONE = 0,
            TABLE_DEFAULT,
            TABLE_ONLY_LINE,
            TABLE_ONLY_TEXT,
            TABLE_LINE_TEXT
        };

        enum turn_angle_t {
            ANGLE_0 = 0,
            ANGLE_90 = 90,
            ANGLE_180 = 180,
            ANGLE_270 = 270
        };

        void addReadRect(const Rect& r);
        std::vector<Rect> readRects() const;
        void clearReadRects();
        bool hasReadRects() const;

        bool autoRotate() const;
        bool dotMatrix() const;
        bool fax() const;
        bool pictureSearch() const;
        table_mode_t tableMode() const;
        const std::string& userDict() const;

        /**
         * Returns image number, if input file contains several images inside,
         * like in PDF or TIFF formats.
         * If file contains only one image, returns 0
         * @see setImageNumber()
         */
        int imageNumber() const;

        /**
         * Sets image number, if input file contains several images
         * @param number - new image number
         * @see imageNumber()
         */
        void setImageNumber(int number);

        /**
         * Sets recognition language
         */
        language_t language() const;
        bool oneColumn() const;
        bool spellCorection() const;

        void setAutoRotate(bool value);
        void setFax(bool value);
        void setDotMatrix(bool value);
        void setLanguage(language_t language);

        bool debugCleanupDelayed() const;
        void setDebugCleanupDelayed(bool value);

        /**
          * Returns true if has page turn
          */
        bool hasTurn() const;

        /**
          * Sets page turn angle
          * @note values: 0, 90, 180, 270
          * @see turnAngle(), hasTurn()
          */
        void setTurnAngle(turn_angle_t angle);

        /**
          * Returns page turn angle
          * @see setTurnAngle(), hasTurn()
          */
        turn_angle_t turnAngle() const;

        /**
         * Sets one column layout
         */
        void setOneColumn(bool value);
        void setPictureSearch(bool value);
        void setSpellCorrection(bool value);
        void setTableMode(table_mode_t mode);
        void setUserDict(const std::string& user_dict);
    private:
        bool hasFlag(uint flag) const { return flags_ & flag; }
        void setFlag(uint flag, bool value) { value ? setFlag(flag) : unsetFlag(flag); }
        void setFlag(uint flag) { flags_ |= flag; }
        void unsetFlag(uint flag) { flags_ &= (~flag); }
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            using boost::serialization::make_nvp;
            ar & make_nvp("language", language_);
            ar & make_nvp("table-mode", table_mode_);
            ar & make_nvp("user-dict", user_dict_name_);
            ar & make_nvp("flags", flags_);
            ar & make_nvp("turn-angle", turn_angle_);
            ar & make_nvp("read-rects", read_rects_);
            ar & make_nvp("image-number", image_number_);
        }
#endif
    private:
        language_t language_;
        table_mode_t table_mode_;
        std::string user_dict_name_;
        size_t flags_;
        turn_angle_t turn_angle_;
        std::vector<Rect> read_rects_;
        int image_number_;
};

FUN_EXPO__ std::ostream& operator<<(std::ostream& os, const RecognizeOptions& opts);

}

#endif /* RECOGNIZEOPTIONS_H_ */
