/*
 GUIDO Library
 Copyright (C) 2012	Grame

 openFrameworks Guido interface by Thomas Coffy (c) IRCAM 2014
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License (Version 2), 
 as published by the Free Software Foundation.
 A copy of the license can be found online at www.gnu.org/licenses.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 */

#include <ofxGuidoFont.h>
#include <string>

// --------------------------------------------------------------
// 		openFrameworks implementation of the VGFont class
// --------------------------------------------------------------
ofxGuidoFont::ofxGuidoFont(const char * faceName, int size, int properties){
	/*
	 int style = Font::plain;
	 if (properties & kFontBold)			style += Font::bold;
	 if (properties & kFontItalic)		style += Font::italic;
	 if (properties & kFontUnderline)	style += Font::underlined;
	 */

	float fSize = size;
//	if(strstr(faceName, "guido2.ttf")){
//		fSize *= 0.7;
//	}
	fNativeFont = new ofTrueTypeFont();
	fNativeFont->setGlobalDpi(72);
	if(strcmp(faceName, "Times") != 0) {
		fName = faceName;
	} else {
		fName = "guidotext.ttf";
	}

	ofLogNotice("AurinDebug") << "Loading " << fName;
	if(fNativeFont->loadFont(fName, fSize, true, true, true)){
		ofLogNotice("AurinDebug") << "Loading successful";
//		fNativeFont->setEncoding(OF_ENCODING_ISO_8859_15);
	}
}

ofxGuidoFont::~ofxGuidoFont(){
	delete fNativeFont;
}

// --------------------------------------------------------------
const char * ofxGuidoFont::GetName() const{
	return fName.c_str();
}
int ofxGuidoFont::GetSize() const{
	return fNativeFont ? int(fNativeFont->getLineHeight()) : 0;
}
int ofxGuidoFont::GetProperties() const{
	int properties = kFontNone;
	if(fNativeFont){
		//if (fNativeFont->isBold()) properties += kFontBold;
		//if (fNativeFont->isItalic()) properties += kFontItalic;
		//if (fNativeFont->isUnderlined()) properties += kFontUnderline;
	}
	return properties;
}

// - Symbol services ---------------------------------------------
void ofxGuidoFont::GetExtent(const char * s, int count, float * width, float * height,
	VGDevice *) const{

	string text(s, count);
	ofRectangle r = fNativeFont->getStringBoundingBox(text, 0, 0);
	*width = r.width;
	*height = r.height;

}

void ofxGuidoFont::GetExtent(unsigned char c, float * width, float * height,
	VGDevice *) const{

	string text;
	text += wchar_t(c);
	ofRectangle r = fNativeFont->getStringBoundingBox(text, 0, 0);
	*width = r.width;
	*height = r.height;

}
