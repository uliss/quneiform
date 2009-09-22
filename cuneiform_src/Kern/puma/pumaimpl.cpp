/*
 * pumaimpl.cpp
 *
 *  Created on: 22.09.2009
 *      Author: uliss
 */

#include "pumaimpl.h"
#include "puma.h"

#include <iostream>
using namespace std;

#ifndef NDEBUG
#define DBG(msg) cerr << msg << endl;
#else
#define DBG(msg) ;
#endif

namespace CIF {

PumaImpl::PumaImpl() {
	DBG("Puma init")
	if (!PUMA_Init())
		throw PumaException("Puma init failed");
}

PumaImpl::~PumaImpl() {
	DBG("~Puma");
	if (!PUMA_Done())
		cerr << "Puma done failed" << endl;
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

}
