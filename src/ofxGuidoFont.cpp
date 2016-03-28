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
	mNativeFont = new ofTrueTypeFont();
//	mNativeFont->setGlobalDpi(72);
	if(strcmp(faceName, "Times") != 0) {
		mName = faceName;
	} else {
		mName = "guidotext.ttf";
	}

	ofLogNotice("AurinDebug") << "Loading " << mName << " with size " << fSize;
	if(mNativeFont->loadFont(mName, fSize, true, true, true, 0.3, 72)){
		mMHeight = mNativeFont->getStringBoundingBox("M", 0, 0).height;
//	if(fNativeFont->loadFont(fName, fSize)){
		ofLogNotice("AurinDebug") << "Loading successful and 'M' height is " << mMHeight;
//		fNativeFont->setEncoding(OF_ENCODING_ISO_8859_15);
	}
}

ofxGuidoFont::~ofxGuidoFont(){
	delete mNativeFont;
}

// --------------------------------------------------------------
const char * ofxGuidoFont::GetName() const{
	return mName.c_str();
}
int ofxGuidoFont::GetSize() const{
	return mNativeFont ? int(mNativeFont->getLineHeight()) : 0;
}
int ofxGuidoFont::GetProperties() const{
	int properties = kFontNone;
	if(mNativeFont){
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
	ofRectangle r = mNativeFont->getStringBoundingBox(text, 0, 0);
	*width = r.width;
	*height = r.height;

}

void ofxGuidoFont::GetExtent(unsigned char c, float * width, float * height,
	VGDevice *) const{

	string text;
	text += wchar_t(c);
	ofRectangle r = mNativeFont->getStringBoundingBox(text, 0, 0);
	*width = r.width;
	*height = r.height;

}
