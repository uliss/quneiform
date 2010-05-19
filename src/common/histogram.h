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

#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include <vector>
#include <algorithm>

namespace CIF
{

class Histogram
{
        typedef std::vector<unsigned short> HistogramVector;
    public:
        typedef HistogramVector::iterator iterator;
        typedef HistogramVector::const_iterator const_iterator;
    public:
        Histogram(size_t size) :
            hist_(size, 0) {
        }

        template<class IteratorBegin, class IteratorEnd>
        Histogram(IteratorBegin first, IteratorEnd last) {
            hist_.reserve(std::distance(first, last));
            std::copy(first, last, std::back_inserter(hist_));
        }

        iterator begin() {
            return hist_.begin();
        }

        const_iterator begin() const {
            return hist_.begin();
        }

        void clear() {
            std::fill(hist_.begin(), hist_.end(), 0);
        }

        bool empty() const {
            return hist_.empty();
        }

        iterator end() {
            return hist_.end();
        }

        const_iterator end() const {
            return hist_.end();
        }

        bool findU() const {
            return findU(0, size());
        }

        bool findU(size_t begin, size_t end) const;

        template<class IteratorBegin, class IteratorEnd>
        void init(IteratorBegin first, IteratorEnd last) {
            hist_.clear();
            hist_.reserve(std::distance(first, last));
            std::copy(first, last, std::back_inserter(hist_));
        }

        void lower(unsigned short value);

        iterator max() {
            return std::max_element(hist_.begin(), hist_.end());
        }

        const_iterator max() const {
            return std::max_element(hist_.begin(), hist_.end());
        }

        iterator min() {
            return std::min_element(hist_.begin(), hist_.end());
        }

        const_iterator min() const {
            return std::min_element(hist_.begin(), hist_.end());
        }

        void minimize();

        HistogramVector::reference operator[](size_t pos) {
            return hist_.at(pos);
        }

        HistogramVector::const_reference operator[](size_t pos) const {
            return hist_.at(pos);
        }

        void raise(unsigned short value);

        size_t size() const {
            return hist_.size();
        }

        size_t spaceCount() const;

        void uniqe() {
            std::unique(hist_.begin(), hist_.end());
        }

    private:
        HistogramVector hist_;
};

}

#endif /* HISTOGRAM_H_ */
