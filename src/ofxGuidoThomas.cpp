#include <ofxGuidoThomas.h>


ofxGuidoThomas::ofxGuidoThomas(GuidoLayoutSettings* layoutSettings)
{
	guido = new GuidoComponent();
#ifdef ASCOGRAPH_IOS
	string textfont = ofFilePath::getCurrentExeDir() + "../Resources/DroidSansMono.ttf";
	string guidofont = ofFilePath::getCurrentExeDir() + "../Resources/GUI/guido2.ttf";
#else
    string textfont("TibetanMachineUni.ttf");
	string guidofont("guido2.ttf");
#endif
	guido->GuidoInit(textfont.c_str(), guidofont.c_str());
	if (layoutSettings) guido->setGuidoLayoutSettings(*layoutSettings);
}


bool ofxGuidoThomas::compile_string(const string& gstr)
{
	int err = guido->setGMNCode(gstr.c_str());
	return !(err == -1);
}

void ofxGuidoThomas::getPageFormat(GuidoPageFormat& format)
{
	GuidoGetPageFormat(guido->getGRHandler(), 1, &format);
}

void ofxGuidoThomas::draw_cache(int x, int y) {
	if (guido) {

		guido->getDevice()->drawCache.draw(x, y);

	}
}

void ofxGuidoThomas::draw(int x, int y, int w, int h) {
	if (guido)
		guido->draw(x, y, w, h);
}

void ofxGuidoThomas::setSize(int w, int h) {
	if (guido) {
		guido->setSize(w, h);
		guido->getDevice()->NotifySize(w, h);
	}
}
