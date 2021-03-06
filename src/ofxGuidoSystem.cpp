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

#include <ofxGuidoDevice.h>
#include <ofxGuidoFont.h>
#include <ofxGuidoSystem.h>

#include <ofMain.h>

// --------------------------------------------------------------
// 		openFrameworks implementation of the VGSystem class
// --------------------------------------------------------------
VGDevice* ofxGuidoSystem::CreateDisplayDevice(){
	return new ofxGuidoDevice(this);
}

VGDevice* ofxGuidoSystem::CreateMemoryDevice(int width, int height){
	return new ofxGuidoDevice(width, height, this);
}

VGDevice* ofxGuidoSystem::CreateMemoryDevice(const char *){
//	FileInputStream file (File (path));
//	if (file.getStatus().failed()) return 0;
//
//	Image img;
//	GIFImageFormat gif;
//	JPEGImageFormat jpg; 
//	PNGImageFormat png;
//	if (gif.canUnderstand (file))
//		img = gif.decodeImage (file);
//	else if (jpg.canUnderstand (file))
//		img = jpg.decodeImage (file);
//	else if (png.canUnderstand (file))
//		Image img = png.decodeImage (file);
//	else return 0;
//
//	Graphics * g = new Graphics(img);
//	return new JuceDevice (g, this);
	return 0;
}

VGDevice* ofxGuidoSystem::CreatePrinterDevice(){
	return 0;
} // don't know how to create a printer device
VGDevice* ofxGuidoSystem::CreateAntiAliasedMemoryDevice(int width, int height){
	return CreateMemoryDevice(width, height);
}

// - Font services -----------------------------------------------
const VGFont* ofxGuidoSystem::CreateVGFont(const char * faceName, int size, int properties) const{
	return new ofxGuidoFont(faceName, size, properties);
}

