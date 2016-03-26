/*
 * ofxGuidoFbo.h
 *
 *  Created on: Mar 15, 2016
 *      Author: luis
 */

#ifndef SRC_OFXGUIDO_H_
#define SRC_OFXGUIDO_H_

#include <GUIDOEngine.h>
#include <GUIDOParse.h>
#include "ofxGuidoSystem.h"
#include "ofxGuidoDevice.h"

class ofxGuido {
	public:
		ofxGuido(int w, int h);
		virtual ~ofxGuido();
		void loadString(const char * s);
		void draw(int x, int y);
		void setSize(int w, int h);
		void setSpring(float s);
		void setBackground(const ofColor & c);

	private:
		ARHandler mARHandler;
		GRHandler mGRHandler;
		GuidoParser * mGuidoParser;
		GuidoInitDesc mGuidoInitDesc;
		ofxGuidoSystem mGuidoSystem;
		VGDevice * mGuidoDevice;
		GuidoPageFormat mGuidoPageFormat;
		GuidoLayoutSettings mGuidoLayoutSettings;
		GuidoOnDrawDesc mGuidoOnDrawDesc;
		int mW, mH;
		ofColor mBackground;

};

#endif /* SRC_OFXGUIDO_H_ */
