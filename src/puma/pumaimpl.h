/*
 * pumaimpl.h
 *
 *  Created on: 22.09.2009
 *      Author: uliss
 */

#ifndef PUMAIMPL_H_
#define PUMAIMPL_H_

#include <string>
#include <iosfwd>
#include <stdexcept>
#include <memory>

#include "pumadef.h"
#include "cfcompat.h"
#include "lang_def.h"
#include "rect.h"
#include "memorybuffer.h"
#include "cimage/imageinfo.h"

class CTIControl;

namespace CIF {

struct PumaException: std::runtime_error {
	PumaException(const std::string& msg) :
		std::runtime_error(msg) {
	}
};

class PumaImpl {
public:
	PumaImpl();
	~PumaImpl();

	void close();
	void open(char * dib);
	Rect pageTemplate() const;
	void recognize();
	void save(const std::string& outputFilename, int format) const;
	void save(void * dest, size_t size, int format) const;
	void setOptionAutoRotate(bool val);
	void setOptionBold(bool val);
	void setOptionDotMatrix(bool val);
	void setOptionFax100(bool val);
	void setOptionFormatMode(puma_format_mode_t format);
	void setOptionItalic(bool val);
	void setOptionLanguage(language_t language);
	void setOptionMonospaceName(const char * name);
	void setOptionOneColumn(bool val);
	void setOptionPictures(puma_picture_t type);
	void setOptionSansSerifName(const char * name);
	void setOptionSerifName(const char * name);
	void setOptionSize(bool val);
	void setOptionTable(puma_table_t mode);
	void setOptionUnrecognizedChar(char ch);
	void setOptionUserDictionaryName(const char * name);
	void setOptionUseSpeller(bool value = true);
	void setPageTemplate(const Rect& r);
public:
	static unsigned char * mainBuffer();
	static unsigned char * workBuffer();
	static const size_t MainBufferSize = 500000;
	static const size_t WorkBufferSize = 180000;
private:
	void binarizeImage();
	void clearAll();
	void extractComponents();
	void extractStrings();
	void formatResult();
	void layout();
	void loadLayoutFromFile(const std::string& fname);
	void modulesDone();
	void modulesInit();
	const char * modulePath() const;
	const char * moduleTmpPath() const;
	void normalize();
	void pass1();
	void pass2();
	void preOpenInitialize();
	void preprocessImage();
	void printResult(std::ostream& os);
	void printResultLine(std::ostream& os, size_t lineNumber);
	void postOpenInitialize();
	void recognizeCorrection();
	void recognizePass1();
	void recognizePass2();
	void recognizeSetup(int lang);
	void rotate(void * dib, Point * p);
	void rout(const std::string& fname, int Format) const;
	void rout(void * dest, size_t size, int format) const;
	void saveLayoutToFile(const std::string& fname);
	void savePass1(const std::string& fname);
	void saveToText(std::ostream& os) const;
	void saveToText(const std::string& filename) const;
	void setFormatOptions();
	void spellCorrection();
private:
	static FixedBuffer<unsigned char, MainBufferSize> main_buffer_;
	static FixedBuffer<unsigned char, WorkBufferSize> work_buffer_;
private:
	std::auto_ptr<CTIControl> cimage_;
	BitmapInfoHeader info_;
	Rect rect_template_;
	bool do_spell_corretion_;
	bool fax100_;
	bool one_column_;
	bool dot_matrix_;
	bool auto_rotate_;
	std::string user_dict_name_;

	std::string serif_name_;
	std::string sans_serif_name_;
	std::string monospace_name_;
	bool bold_;
    bool italic_;
	bool size_;
	puma_format_mode_t format_mode_;
	uchar unrecognized_char_;

	bool preserve_line_breaks_;
};

}

#endif /* PUMAIMPL_H_ */
