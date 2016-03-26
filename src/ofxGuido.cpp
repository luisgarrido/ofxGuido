/*
 * ofxGuidoFbo.cpp
 *
 *  Created on: Mar 15, 2016
 *      Author: luis
 */

#include <ofxGuido.h>

ofxGuido::ofxGuido(int w, int h) :
	mARHandler(0), mGRHandler(0), mW(w), mH(h), mBackground(){
	mGuidoDevice = mGuidoSystem.CreateMemoryDevice(w, h);
	mGuidoDevice->SelectFillColor(VGColor(0, 0, 0));
	mGuidoInitDesc.graphicDevice = mGuidoDevice;
	mGuidoInitDesc.musicFont = "guido2t.ttf";
	mGuidoInitDesc.textFont = "guidotext.ttf";
	GuidoInit(&mGuidoInitDesc);
	GuidoGetDefaultLayoutSettings(&mGuidoLayoutSettings);
	mGuidoOnDrawDesc.hdc = mGuidoDevice;
	mGuidoParser = GuidoOpenParser();
	// Setup a large default format to avoid system breaks. They can also be avoided
	// by using the GMN advanced tag \set<autoSystemBreak="off">
	// Extra right margin makes room for the systems brace.
	GuidoPageFormat gpf = { 5000, 5000, 150, 100, 100, 100 }; // w, h, ml, mt, mr, mb
	GuidoSetDefaultPageFormat(&gpf);
	mBackground.set(0, 0, 0, 0);
}

void ofxGuido::loadString(const char * s){
	ARHandler arh = GuidoString2AR(mGuidoParser, s);
	int l, c;
	const char * errstr;
	GuidoErrCode err = GuidoParserGetErrorCode(mGuidoParser, l, c, &errstr);
	if(err != guidoNoErr){
		GuidoFreeAR(arh);
		ofLogError("ofxGuido") << "GuidoParser error: " << errstr;
	}
	else{
		GRHandler grh;
		err = GuidoAR2GR(arh, &mGuidoLayoutSettings, &grh);
		if(err == guidoNoErr){
			if(mGRHandler){
				GuidoFreeGR(mGRHandler);
			}
			mGRHandler = grh;
			mARHandler = arh;
			GuidoResizePageToMusic(mGRHandler);
			mGuidoOnDrawDesc.handle = mGRHandler;
		}
		else{
			errstr = GuidoGetErrorString(err);
			ofLogError("ofxGuido") << "GuidoAR2GR error: " << errstr;
			GuidoFreeAR(arh);
		}
	}
}

void ofxGuido::draw(int x, int y){
	GuidoPageFormat gpf;
	GuidoGetPageFormat(mGRHandler, 1, &gpf);
	ofLogNotice("ofxGuido") << gpf.width << "x" << gpf.height << " (" << gpf.margintop << ","
		<< gpf.marginbottom << "," << gpf.marginleft << "," << gpf.marginright << ")";
	float scaleX = mW / gpf.width;
	float scaleY = mH / gpf.height;
	int offsetX = 0, offsetY = 0;
	if(scaleX < scaleY){
		offsetY = (mH - (int)(gpf.height * scaleX)) / 2 - 1;
		ofLogNotice("ofxGuido") << "Scale: " << scaleX << " offsetY: " << offsetY;
	}
	else{
		offsetX = (mW - (int)(gpf.width * scaleY)) / 2 - 1;
		ofLogNotice("ofxGuido") << "Scale: " << scaleY << " offsetX: " << offsetX;
	}
	mGuidoOnDrawDesc.page = 1;
	mGuidoOnDrawDesc.scrollx = 0;
	mGuidoOnDrawDesc.scrolly = 0;
	mGuidoOnDrawDesc.updateRegion.erase = true;
	mGuidoOnDrawDesc.sizex = mW;
	mGuidoOnDrawDesc.sizey = mH;
	ofPushMatrix();
	ofPushStyle();
	ofFill();
	ofSetColor(mBackground);
	ofRect(x, y, mW, mH);
	ofTranslate(x + offsetX, y + offsetY);
	GuidoOnDraw(&mGuidoOnDrawDesc);
	ofPopStyle();
	ofPopMatrix();
}

void ofxGuido::setSize(int w, int h) {
	mW = w;
	mH = h;
}

void ofxGuido::setSpring(float s) {
	mGuidoLayoutSettings.spring = s;
}

void ofxGuido::setBackground(const ofColor & c) {
	mBackground.set(c);
}

ofxGuido::~ofxGuido(){
	GuidoFreeGR(mGRHandler);
	GuidoFreeAR(mARHandler);
	GuidoCloseParser(mGuidoParser);
	if(mGuidoDevice){
		delete mGuidoDevice;
	}
}

