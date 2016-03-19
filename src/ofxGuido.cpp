/*
 * ofxGuidoFbo.cpp
 *
 *  Created on: Mar 15, 2016
 *      Author: luis
 */

#include <ofxGuido.h>

#include <ofFbo.h>

ofxGuido::ofxGuido(int w, int h) : mARHandler(), mGRHandler() {
	mGuidoDevice = mGuidoSystem.CreateMemoryDevice(w, h);
	mGuidoInitDesc.graphicDevice = mGuidoDevice;
	mGuidoInitDesc.musicFont = "guido2.ttf";
	mGuidoInitDesc.textFont = "guidotext.ttf";
	GuidoInit(&mGuidoInitDesc);
	GuidoGetDefaultLayoutSettings(& mGuidoLayoutSettings);
	mGuidoOnDrawDesc.hdc = mGuidoDevice;
}

void ofxGuido::loadString(const char * s) {
	GuidoParseString(s, &mARHandler);
	GuidoAR2GR(mARHandler, &mGuidoLayoutSettings, & mGuidoOnDrawDesc.handle);
	GuidoResizePageToMusic(mGuidoOnDrawDesc.handle);
	mGuidoOnDrawDesc.page = 1;
	mGuidoOnDrawDesc.scrollx = 0;
	mGuidoOnDrawDesc.scrolly = 0;
	mGuidoOnDrawDesc.updateRegion.erase = true;
	mGuidoOnDrawDesc.sizex = 600;
	mGuidoOnDrawDesc.sizey = 600;
//	mGuidoDevice->SetScale(2, 2);
	mGuidoDevice->SetFontColor(VGColor(0,0,0));

	GuidoOnDraw(&mGuidoOnDrawDesc);
	GuidoPageFormat gpf;
	GuidoGetPageFormat(mGuidoOnDrawDesc.handle, 1, &gpf);
	ofLogNotice("AurinDebug") << gpf.width << "x" << gpf.height;
}

void ofxGuido::draw(int x, int y) {
	ofFbo * fbo = static_cast<ofFbo *>(mGuidoDevice->GetNativeContext());
	fbo = & static_cast<ofxGuidoDevice *>(mGuidoDevice)->drawCache;
	fbo->begin();
	fbo->end();
	fbo->draw(x, y);
}

ofxGuido::~ofxGuido(){
	if(mGuidoDevice) {
		delete mGuidoDevice;
	}
}

