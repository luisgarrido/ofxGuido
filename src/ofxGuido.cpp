#include "ofxGuido.h"


ofxGuido::ofxGuido(GuidoLayoutSettings& layoutSettings)
{
	guido = new GuidoComponent();
//	guido->GuidoInit("DroidSansMono.ttf", "GUI/guido2.ttf");
	guido->GuidoInit("TibetanMachineUni.ttf", "guido2.ttf");
	guido->setGuidoLayoutSettings(layoutSettings);
}


bool ofxGuido::compile_string(const string& gstr)
{
	int err = guido->setGMNCode(gstr.c_str());
	return !(err == -1);
}

void ofxGuido::getPageFormat(GuidoPageFormat& format)
{
	GuidoGetPageFormat(guido->getGRHandler(), 1, &format);
}

void ofxGuido::draw_cache(int x, int y) {
	if (guido)
		guido->getDevice()->drawCache.draw(x, y);
}

void ofxGuido::draw(int x, int y, int w, int h) {
	if (guido)
		guido->draw(x, y, w, h);
}

void ofxGuido::setSize(int w, int h) {
	if (guido) {
		guido->setSize(w, h);
		guido->getDevice()->NotifySize(w, h);
	}
}
