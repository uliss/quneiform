/*
 * pumaimpl.cpp
 *
 *  Created on: 22.09.2009
 *      Author: uliss
 */

#include "pumaimpl.h"
#include "puma.h"
#include "mpuma.h"

#include <iostream>
#include <cassert>

using namespace std;

#ifndef NDEBUG
#define DBG(msg) cerr << msg << endl;
#else
#define DBG(msg) ;
#endif

static Handle ghStorage = NULL;

namespace CIF {

unsigned char * PumaImpl::main_buffer_ = 0;
unsigned char * PumaImpl::work_buffer_ = 0;

PumaImpl::PumaImpl() {
	initMainBuffer();
	initWorkBuffer();

	InitDebug();
	if (!ModulesInit((void*) ghStorage))
		throw PumaException("Puma init failed");
}

PumaImpl::~PumaImpl() {
	if (!ModulesDone((void*) ghStorage))
		cerr << "Puma done failed" << endl;
	DoneDebug();

	freeMainBuffer();
	freeWorkBuffer();
}

void PumaImpl::analyze() {
	if (!PUMA_XPageAnalysis())
		throw PumaException("Puma analyze failed");
}

void PumaImpl::close() {
	DBG("Puma close")
	if (!PUMA_XClose())
		throw PumaException("Puma XClose fail");
}

void PumaImpl::freeMainBuffer() {
	if (!main_buffer_)
		return;
	delete[] main_buffer_;
	main_buffer_ = NULL;
}

void PumaImpl::freeWorkBuffer() {
	if (!work_buffer_)
		return;
	delete[] work_buffer_;
	work_buffer_ = NULL;
}

void PumaImpl::initMainBuffer() {
	if (main_buffer_) {
		DBG("PumaImpl: main buffer is not empty")
		delete[] main_buffer_;
	}
	main_buffer_ = new unsigned char[MainBufferSize];
}

void PumaImpl::initWorkBuffer() {
	if (work_buffer_) {
		DBG ("PumaImpl: work buffer is not empty")
		delete[] work_buffer_;
	}
	work_buffer_ = new uchar[WorkBufferSize];
}

void PumaImpl::open(char * dib) {
	if (!PUMA_XOpen(dib, "none.txt"))
		throw PumaException("Puma open failed");
	DBG("Puma open")
}

void PumaImpl::recognize() {
	if (!PUMA_XFinalRecognition())
		throw PumaException("Puma recognition failed");
	DBG("Puma recognize")
}

void PumaImpl::save(const std::string& filename, int Format) const {
#ifndef NDEBUG
	cerr << "Puma save to: " << filename << endl;
#endif

	if (!PUMA_XSave(filename, static_cast<puma_format_t> (Format),
			PUMA_CODE_UTF8))
		throw PumaException("Puma save failed");
}

unsigned char * PumaImpl::mainBuffer() {
	assert(main_buffer_);
	return main_buffer_;
}

unsigned char * PumaImpl::workBuffer() {
	assert(work_buffer_);
	return work_buffer_;
}

}
