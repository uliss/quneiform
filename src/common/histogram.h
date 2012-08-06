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
#include <numeric>

namespace cf
{

template<class T>
class HistogramImpl
{
    public:
        typedef std::vector<T> HistogramVector;
        typedef typename HistogramVector::iterator iterator;
        typedef typename HistogramVector::const_iterator const_iterator;
        typedef typename HistogramVector::reference reference;
        typedef typename HistogramVector::const_reference const_reference;
        typedef T value_type;
    public:
        /**
         * Constructs histogram of defined size initiated with 0
         * @param size - size of histogram
         */
        HistogramImpl(size_t size) :
            hist_(size, 0) {
        }

        /**
         * Construct histogram from container values
         * @param first - first iterator
         * @param last - end iterator
         */
        template<class IteratorBegin, class IteratorEnd>
        HistogramImpl(IteratorBegin first, IteratorEnd last) {
            hist_.reserve(std::distance(first, last));
            std::copy(first, last, std::back_inserter(hist_));
        }

        void add(unsigned int value) {
            if(value < size())
                hist_[value]++;
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

        void compress() {
            std::unique(hist_.begin(), hist_.end());
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

        /**
         * Subtracts  value from all histogram values
         * if some histogram values less then @b value - it becames 0
         */
        void lower(T value);

        iterator max() {
            return std::max_element(hist_.begin(), hist_.end());
        }

        const_iterator max() const {
            return std::max_element(hist_.begin(), hist_.end());
        }

        T max_element() const {
            return *max();
        }

        iterator min() {
            return std::min_element(hist_.begin(), hist_.end());
        }

        const_iterator min() const {
            return std::min_element(hist_.begin(), hist_.end());
        }

        T min_element() const {
            return * min();
        }

        void minimize();

        /**
         * Returns reference to histogram value at positions @b pos
         * @throw std::out_of_range if invalid position given
         */
        reference operator[](size_t pos) {
            return hist_.at(pos);
        }

        const_reference operator[](size_t pos) const {
            return hist_.at(pos);
        }

        /**
         * Adds new value to the end of histogram
         */
        void push_back(T value);

        /**
         * Adds @b value to all histogram values
         */
        void raise(T value);

        /**
         * Resizes histogram
         */
        void resize(size_t new_sz);

        /**
         * Returns histogram size
         */
        size_t size() const;

        /**
         * Returns number of spaces (where elements == 0) between histogram peaks
         * @example for 01100011100 - it returns 3
         */
        size_t spaceCount() const;

        /**
         * Inserts space positions into given container
         * @see spaceCount()
         */
        template<class IteratorBegin>
        void spacePosition(IteratorBegin it) const;

        /**
         * Returns sum of all values in histogram
         */
        size_t sum() const;

        /**
         * Returns histogram weighted sum
         */
        size_t weightedSum() const;
    private:
        HistogramVector hist_;
};

template<class T>
bool HistogramImpl<T>::findU(size_t begin, size_t end) const {
    if (begin >= hist_.size() || end > hist_.size() || begin == end || begin > end)
        return false;

    bool left_descent = false;
    bool right_ascent = false;
    unsigned short current = hist_[begin];

    for (size_t i = begin; i < end; i++) {
        if (current < hist_[i]) {
            if (left_descent == false)
                current = hist_[i];
            else if (left_descent == true && right_ascent == false) {
                right_ascent = true;
                // found 'U'-like histogram
                // return
                return true;
            }
        } else if (current > hist_[i]) {
            if (left_descent == false) {
                current = hist_[i];
                left_descent = true;
            } else if (left_descent == true && right_ascent == false)
                current = hist_[i];
        }
    }

    return false;
}

template<class T>
void HistogramImpl<T>::lower(T value) {
    for (iterator it = hist_.begin(), end = hist_.end(); it != end; ++it) {
        if (*it > value)
            *it -= value;
        else
            *it = 0;
    }
}

template<class T>
void HistogramImpl<T>::minimize() {
    iterator it = min();
    if (it != end() || (*it) != 0)
        lower(*it);
}

template<class T>
void HistogramImpl<T>::push_back(T value) {
    hist_.push_back(value);
}

template<class T>
void HistogramImpl<T>::raise(T value) {
    for (iterator it = hist_.begin(), end = hist_.end(); it != end; ++it)
        *it += value;
}

template<class T>
void HistogramImpl<T>::resize(size_t new_sz)
{
    hist_.resize(new_sz);
}

template<class T>
size_t HistogramImpl<T>::size() const {
    return hist_.size();
}

template<class T>
size_t HistogramImpl<T>::spaceCount() const {
    size_t space_count = 0;
    bool space_flag = false;
    for (size_t i = 0, sz = hist_.size(); i < sz; i++) {
        if (hist_[i] == 0 && !space_flag) {
            space_flag = true;
            space_count++;
        } else if (hist_[i] != 0 && space_flag) {
            space_flag = false;
        }
    }
    return space_count;
}

template<class T>
size_t HistogramImpl<T>::sum() const
{
    return std::accumulate(hist_.begin(), hist_.end(), 0);
}

template<class T>
size_t HistogramImpl<T>::weightedSum() const
{
    size_t res = 0;
    for(size_t i = 0, total = hist_.size(); i < total; i++)
        res += i * hist_[i];

    return res;
}

template<class T>
template<class IteratorBegin>
void HistogramImpl<T>::spacePosition(IteratorBegin it) const {
    bool space_flag = false;
    for (size_t i = 0, sz = hist_.size(); i < sz; i++) {
        if (hist_[i] == 0 && !space_flag) {
            space_flag = true;
            it = i;
            ++it;
        } else if (hist_[i] != 0 && space_flag) {
            space_flag = false;
        }
    }
}

typedef HistogramImpl<unsigned char> Histogram;
typedef HistogramImpl<unsigned int> HistogramInt;

}

#endif /* HISTOGRAM_H_ */
