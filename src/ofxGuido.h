/*
 * ofxGuidoFbo.h
 *
 *  Created on: Mar 15, 2016
 *      Author: luis
 */

#ifndef SRC_OFXGUIDO_H_
#define SRC_OFXGUIDO_H_

#include "GUIDOEngine.h"
#include "ofxGuidoSystem.h"
#include "ofxGuidoDevice.h"

class ofxGuido {
	public:
		ofxGuido(int w, int h);
		virtual ~ofxGuido();
		void loadString(const char * s);
		void draw(int x, int y);

	private:
		ARHandler mARHandler;
		GRHandler mGRHandler;
		GuidoInitDesc mGuidoInitDesc;
		ofxGuidoSystem mGuidoSystem;
		VGDevice * mGuidoDevice;
		GuidoLayoutSettings mGuidoLayoutSettings;
		GuidoOnDrawDesc mGuidoOnDrawDesc;

};

#endif /* SRC_OFXGUIDO_H_ */
