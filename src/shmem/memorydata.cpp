/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#include <string.h>
#include <sstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/current_function.hpp>

#include "common/debug.h"
#include "common/helper.h"
#include "common/console_messages.h"
#include "memorydata.h"
#include "export/cuneiformbinexporter.h"
#include "load/cuneiformbinloader.h"

namespace cf {

static const size_t PAGE_BUFFER_SIZE = 2 * 1024 * 1024;
static const size_t IMAGE_PATH_SIZE = 1024;

#define CF_ERROR std::cerr << cf::console::error << BOOST_CURRENT_FUNCTION << ": "
#define CF_WARNING std::cerr << cf::console::warning << BOOST_CURRENT_FUNCTION << ": "
#define CF_INFO cf::Debug() << cf::console::info << BOOST_CURRENT_FUNCTION << ": "

typedef boost::archive::binary_oarchive BinOutputArchive;
typedef boost::archive::binary_iarchive BinInputArchive;

template<class T, size_t MAXSIZE>
class RawData {
public:
    RawData() : size_(0) {
        clear();
    }

    void clear() {
        memset(buffer_, 0, MAXSIZE);
    }

    T load() const {
        if(size_ == 0)
            return T();

        try {
            std::stringstream buf;
            buf.write(buffer_, (std::streamsize) size_);
            BinInputArchive ar(buf);

            T data;
            ar >> data;
            return data;
        }
        catch(std::exception& e) {
            CF_ERROR << " load error: " << e.what() << std::endl;
            throw MemoryData::Exception(e.what());
        }
    }

    bool save(const T& data) {
        try {
            std::stringstream buf;
            BinOutputArchive ar(buf);
            ar << data;

            size_ = streamSize(buf);
            if(size_ >= MAXSIZE) {
                CF_ERROR << " data is too big!" << std::endl;
                return false;
            }

            buf.read(buffer_, (std::streamsize)size_);
            return true;
        }
        catch(std::exception& e) {
            CF_ERROR << " save error: " << e.what() << std::endl;
            return false;
        }
    }

    size_t size() const { return size_; }
private:
    size_t size_;
    char buffer_[MAXSIZE];
};

template<>
CEDPagePtr RawData<CEDPagePtr, PAGE_BUFFER_SIZE>::load() const {
    CEDPagePtr res;

    if(size_ == 0)
        return res;

    try {
        std::stringstream buf;
        buf.write(buffer_, (std::streamsize)size_);
        CuneiformBinLoader l;
        res = l.load(buf);
        CF_INFO << size_ << " bytes read\n";
    }
    catch(std::exception& e) {
        CF_ERROR << e.what() << std::endl;
        throw MemoryData::Exception(e.what());
    }

    return res;
}

template<>
bool RawData<CEDPagePtr, PAGE_BUFFER_SIZE>::save(const CEDPagePtr& page) {
    std::stringstream buf;

    try {
        CuneiformBinExporter e(page, FormatOptions());
        e.exportTo(buf);
    }
    catch(std::exception& e) {
        CF_ERROR << "serialization error: " << e.what() << std::endl;
        return false;
    }

    size_ = streamSize(buf);

    if(size_ >= PAGE_BUFFER_SIZE) {
        CF_ERROR << "the page is too big (" << size_ << " bytes).\n"
                 << "It exceeds available memory size: " << PAGE_BUFFER_SIZE << " bytes." << std::endl;
        clear();
        return false;
    }

    buf.readsome(buffer_, PAGE_BUFFER_SIZE);

    if(buf.fail()) {
        CF_ERROR << "buffer read failed." << std::endl;
        clear();
        return false;
    }

    CF_INFO << size_ << " bytes stored\n";

    return true;
}

template<>
std::string RawData<std::string, IMAGE_PATH_SIZE>::load() const {
    if(size_ == 0)
        return std::string();

    if(size_ > IMAGE_PATH_SIZE)
        throw MemoryData::Exception("path data corrupted");

    return std::string(buffer_, size_);
}

template<>
bool RawData<std::string, IMAGE_PATH_SIZE>::save(const std::string& path) {
    if(path.size() > IMAGE_PATH_SIZE)
        throw MemoryData::Exception("path is too long");

    size_ = path.size();
    strncpy(buffer_, path.c_str(), IMAGE_PATH_SIZE - 1);
    return true;
}

typedef RawData<FormatOptions, 512> FormatOptionsData;
typedef RawData<RecognizeOptions, 512> RecognizeOptionsData;
typedef RawData<std::string, IMAGE_PATH_SIZE> ImagePathData;
typedef RawData<CEDPagePtr, PAGE_BUFFER_SIZE> PageData;

class ImageData {
public:
    static const size_t IMAGE_NAME_SIZE = 256;
    size_t size;
    int width;
    int height;
    char name[IMAGE_NAME_SIZE];
    size_t name_size;
    intptr_t memory;

    ImageData() : size(0), width(0), height(0), name_size(0), memory(0) {
        memset(name, 0, IMAGE_NAME_SIZE);
    }

    ImagePtr load(size_t maxSize) const {
        if(size == 0)
            return ImagePtr();

        if(size > maxSize)
            throw MemoryData::Exception("currupted image data");

        CF_INFO << "read image data: " << size << " bytes from " << &memory << "\n";

        Image * img = new Image((uchar*)&memory, size, Image::AllocatorNone);
        img->setSize(Size(width, height));
        if(name_size)
            img->setFileName(std::string(name, name_size));
        return ImagePtr(img);
    }

    bool save(ImagePtr image, size_t maxSize) {
        if(!image)
            throw MemoryData::Exception("NULL image given");

        if(image->dataSize() > maxSize) {
            std::ostringstream os;
            os << "image size is too big: " << image->dataSize() << " bytes\n";
            throw MemoryData::Exception(os.str());
        }

        size = image->dataSize();
        width = image->width();
        height = image->height();

        std::string fileName = image->fileName();

        if(fileName.size() > IMAGE_NAME_SIZE) {
            CF_ERROR << "file name is to long, it will be truncated\n";
            strncpy(name, fileName.c_str(), IMAGE_NAME_SIZE);
            name_size = IMAGE_NAME_SIZE;
        }
        else {
            strncpy(name, fileName.c_str(), fileName.size());
            name_size = fileName.size();
        }

        memcpy(&memory, image->data(), image->dataSize());

        CF_INFO << "write image data: " << image->dataSize() << " bytes to " << &memory << "\n";

        return true;
    }
};

struct MemoryDataPrivate {
    FormatOptionsData fopts;
    RecognizeOptionsData ropts;
    ImagePathData path;
    PageData page;
    ImageData image;
};

MemoryData::MemoryData(void * memory, size_t sz)
    : memory_(memory), size_(sz)
{
}

MemoryDataPrivate * MemoryData::data() const
{
    if(!memory_)
        throw Exception("NULL memory");

    if(size_ < sizeof(MemoryDataPrivate)) {
        std::ostringstream os;
        os << "shared memory size (" << size_ << " bytes) is too small to place "
           << sizeof(MemoryDataPrivate) << " byte block\n";
        throw Exception(os.str());
    }

    return static_cast<MemoryDataPrivate*>(memory_);
}

void MemoryData::clear()
{
    memset(memory_, 0, size_);
}

bool MemoryData::empty() const
{
    return memory_ == NULL || size_ == 0;
}

FormatOptions MemoryData::formatOptions() const
{
    return data()->fopts.load();
}

bool MemoryData::hasImagePath() const
{
    return data()->path.size() > 0;
}

ImagePtr MemoryData::image() const
{
    return data()->image.load(size_ - sizeof(MemoryDataPrivate));
}

std::string MemoryData::imagePath() const
{
    return data()->path.load();
}

void * MemoryData::memory()
{
    return memory_;
}

CEDPagePtr MemoryData::page() const
{
    return data()->page.load();
}

RecognizeOptions MemoryData::recognizeOptions() const
{
    return data()->ropts.load();
}

void MemoryData::setFormatOptions(const cf::FormatOptions& fopts)
{
    data()->fopts.save(fopts);
}

void MemoryData::setImage(ImagePtr image)
{
    data()->image.save(image, size_ - minBufferSize());
}

void MemoryData::setImagePath(const std::string& path)
{
    data()->path.save(path);
}

void MemoryData::setPage(cf::CEDPagePtr page)
{
    data()->page.save(page);
}

void MemoryData::setRecognizeOptions(const RecognizeOptions& ropts)
{
    data()->ropts.save(ropts);
}

size_t MemoryData::size() const
{
    return size_;
}

size_t MemoryData::minBufferSize()
{
    return sizeof(MemoryDataPrivate);
}

}
