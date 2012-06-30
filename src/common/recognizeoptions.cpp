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

#include <iomanip>

#include "recognizeoptions.h"
#include "language.h"
#include "tostring.h"

namespace cf {

enum {
    AUTOROTATE       = 0x001,
    DOT_MATRIX       = 0x002,
    SPELL_CORRECTION = 0x004,
    FAX              = 0x008,
    ONE_COLUMN       = 0x010,
    FIND_PICTURES    = 0x020,
    CLEANUP_DELAYED  = 0x040
};

RecognizeOptions::RecognizeOptions() :
    language_(LANGUAGE_ENGLISH),
    table_mode_(TABLE_DEFAULT),
    flags_(0),
    turn_angle_(ANGLE_0),
    image_number_(0)
{
    setAutoRotate(false);
    setDotMatrix(false);
    setSpellCorrection(true);
    setFax(false);
    setOneColumn(false);
    setPictureSearch(true);
}

void RecognizeOptions::addReadRect(const Rect &r)
{
    read_rects_.push_back(r);
}

std::vector<Rect> RecognizeOptions::readRects() const
{
    return read_rects_;
}

void RecognizeOptions::clearReadRects()
{
    read_rects_.clear();
}

bool RecognizeOptions::hasReadRects() const
{
    return !read_rects_.empty();
}

bool RecognizeOptions::autoRotate() const {
    return hasFlag(AUTOROTATE);
}

bool RecognizeOptions::dotMatrix() const {
    return hasFlag(DOT_MATRIX);
}

bool RecognizeOptions::fax() const {
    return hasFlag(FAX);
}

language_t RecognizeOptions::language() const {
    return language_;
}

bool RecognizeOptions::oneColumn() const {
    return hasFlag(ONE_COLUMN);
}

bool RecognizeOptions::pictureSearch() const {
    return hasFlag(FIND_PICTURES);
}

void RecognizeOptions::setAutoRotate(bool value) {
    setFlag(AUTOROTATE, value);
}

void RecognizeOptions::setFax(bool value) {
    setFlag(FAX, value);
}

void RecognizeOptions::setDotMatrix(bool value) {
    setFlag(DOT_MATRIX, value);
}

void RecognizeOptions::setLanguage(language_t language) {
    language_ = language;
}

bool RecognizeOptions::debugCleanupDelayed() const
{
    return hasFlag(CLEANUP_DELAYED);
}

void RecognizeOptions::setDebugCleanupDelayed(bool value)
{
    setFlag(CLEANUP_DELAYED, value);
}

bool RecognizeOptions::hasTurn() const
{
    return turn_angle_ != ANGLE_0;
}

void RecognizeOptions::setTurnAngle(turn_angle_t angle)
{
    turn_angle_ = angle;
}

RecognizeOptions::turn_angle_t RecognizeOptions::turnAngle() const
{
    return turn_angle_;
}

void RecognizeOptions::setOneColumn(bool value) {
    setFlag(ONE_COLUMN, value);
}

void RecognizeOptions::setPictureSearch(bool value) {
    setFlag(FIND_PICTURES, value);
}

void RecognizeOptions::setSpellCorrection(bool value) {
    setFlag(SPELL_CORRECTION, value);
}

void RecognizeOptions::setTableMode(table_mode_t mode) {
    table_mode_ = mode;
}

void RecognizeOptions::setUserDict(const std::string& user_dict) {
    user_dict_name_ = user_dict;
}

bool RecognizeOptions::spellCorection() const {
    return hasFlag(SPELL_CORRECTION);
}

RecognizeOptions::table_mode_t RecognizeOptions::tableMode() const {
    return table_mode_;
}

const std::string& RecognizeOptions::userDict() const {
    return user_dict_name_;
}

int RecognizeOptions::imageNumber() const
{
    return image_number_;
}

void RecognizeOptions::setImageNumber(int number)
{
    image_number_ = number;
}

template<class T>
static void OPT(std::ostream& os, const std::string& name, const T& value) {
    static const std::string INDENT(4, ' ');
    static const int FIELD_WIDTH = 25;
    os << INDENT << std::left << std::setw(FIELD_WIDTH);
    os << name + ":" << value << "\n";
}

static void printRects(std::ostream& os, const std::vector<Rect>& rects) {
    os << "    " << "Read areas: ";

    for(size_t i = 0; i < rects.size(); i++) {
        os << "        " << rects[i] << ",\n";
    }

    os << "\n";
}

std::ostream& operator<<(std::ostream& os, const RecognizeOptions& opts) {
    os << "Recognize options:\n";
    os << std::boolalpha;
    OPT(os, "Spell", opts.spellCorection());
    OPT(os, "Fax", opts.fax());
    OPT(os, "Single column layout", opts.oneColumn());
    OPT(os, "Dot matix", opts.dotMatrix());
    OPT(os, "Pictures search", opts.pictureSearch());
    OPT(os, "Table mode", opts.tableMode());
    OPT(os, "Autorotate" , opts.autoRotate());
    OPT(os, "Language", Language(opts.language()));
    OPT(os, "User dictionary", opts.userDict());
    OPT(os, "Turn angle", opts.turnAngle());
    OPT(os, "Image number", opts.imageNumber());
    printRects(os, opts.readRects());
    os << std::noboolalpha;
    return os;
}

}
