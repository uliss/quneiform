/*
 * layoutoptions.h
 *
 *  Created on: 19.10.2009
 *      Author: uliss
 */

#ifndef LAYOUTOPTIONS_H_
#define LAYOUTOPTIONS_H_

#include <string>

#include "pumadef.h"
#include "cttypes.h"
#include "rect.h"

class RSPreProcessImage;
class RMPreProcessImage;

namespace CIF {

class LayoutOptions
{
public:
    LayoutOptions();
    bool autoRotate() const;
    bool dotMatrix() const;
    std::string layoutFilename() const;
    puma_picture_t pictures() const;
    Rect rect() const;
    void setAutoRotate(bool val);
    void setData(RSPreProcessImage& data);
    void setData(RMPreProcessImage& data);
    void setDotMatrix(bool val);
    void setOneColumn(bool val);
    void setPictures(puma_picture_t type);
    void setRect(const Rect& rect);
    void setTable(puma_table_t mode);
    int tablesNum() const;
private:
    bool auto_rotate_;
    bool dot_matrix_;
    bool one_column_;
    puma_picture_t pictures_;
    puma_table_t tables_;
    int tables_num_;
    Rect rect_template_;
    std::string layout_filename_;
    Bool32 rc_line_;
    Bool32 need_clean_line_;
};

}

#endif /* LAYOUTOPTIONS_H_ */
