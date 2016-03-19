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
#include <ofMain.h>

#include <FontManager.h>

#ifndef GL_MULTISAMPLE_ARB
#define GL_MULTISAMPLE_ARB 0x809D
#endif

// --------------------------------------------------------------
// static tools
static float CoordToRadian(float x, float y){
	return (float)atan2(x, y);
}
static ofColor Color2ofColor(const VGColor & c){
	return ofColor(c.mRed, c.mGreen, c.mBlue, c.mAlpha);
}

// --------------------------------------------------------------
void ofxGuidoDevice::initialize(){
	fXPos = fYPos = 0;
	fXOrigin = fYOrigin = 0;
	fLineThick = 1.0;
	fXScale = fYScale = 1.0f;
	fDPI = 72.f;
	fCurrentFont = 0;
	fFontAlign = 0;
	fRasterOpMode = kOpCopy;
	fFillColorStack.push(VGColor(0, 0, 0));
	fPenColorStack.push(VGColor(0, 0, 0));
}

// --------------------------------------------------------------
ofxGuidoDevice::ofxGuidoDevice(VGSystem* sys) :
	fTextFont(0), fMusicFont(0), fSystem(sys){
	initialize();
}

// --------------------------------------------------------------
// Called by ofGuidoSystem::createMemoryDevice
ofxGuidoDevice::ofxGuidoDevice(int width_, int height_, VGSystem* sys) :
	fTextFont(0), fMusicFont(0), fSystem(sys){
	initialize();
	fWidth = width_;
	fHeight = height_;

//	ofClear(255, 255, 255, 0);
//	glClearColor(0.0, 0.0, 0.0, 0.0);
#if 0
	glClear (GL_COLOR_BUFFER_BIT);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable( GL_DEPTH_TEST );
	glEnable(GL_MULTISAMPLE_ARB);
	drawCache.allocate(fWidth, fHeight, GL_RGBA, 8);
#endif

	int w, h;
	ofFbo::Settings settings;
#ifdef ASCOGRAPH_IOS
	fWidth = 1024;
	fHeight = 1024;

	/*GLint dims[2];
	 glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &dims[0]);
	 fWidth = w = dims[0]; fHeight = h = dims[1];*/
	settings.width = fWidth;
	settings.height = fHeight;
	settings.numSamples = 0;
#else
//	glClear (GL_COLOR_BUFFER_BIT);
//	glEnable (GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glDisable (GL_DEPTH_TEST);
//	glEnable(GL_MULTISAMPLE_ARB);

	fWidth = width_;
	fHeight = height_;
	settings.width = fWidth;
	settings.height = fHeight;
//	settings.numSamples = 0;
#endif
	//cout << "openFrameworksDevice: allocating FBO: " << fWidth << "x" << fHeight << endl;

	settings.internalformat = GL_RGBA;
	settings.useDepth = false;
	settings.useStencil = false;

	drawCache.allocate(settings);

	drawCache.begin();
	ofClear(255, 255, 255, 0);
	drawCache.end();
}
// --------------------------------------------------------------
ofxGuidoDevice::~ofxGuidoDevice(){
#ifdef ASCOGRAPH_IOS
	if (drawCache.isAllocated())
	glDeleteFramebuffersOES(1, &fbo);
#endif
}

// - Drawing services ------------------------------------------------
// --------------------------------------------------------------
bool ofxGuidoDevice::BeginDraw(){
	ofPushStyle();
	drawCache.begin();
	ofClear(255, 255, 255, 0);
	ofLogNotice("AurinDebug") << "BeginDraw";
	return true;
}
void ofxGuidoDevice::EndDraw(){
	drawCache.end();
	ofPopStyle();
	ofLogNotice("AurinDebug") << "EndDraw";
}
void ofxGuidoDevice::InvalidateRect(float /*left*/, float /*top*/, float /*right*/,
	float /*bottom*/){
}

// - Standard graphic primitives -------------------------
void ofxGuidoDevice::MoveTo(float x, float y){
	fXPos = x;
	fYPos = y;
}
void ofxGuidoDevice::LineTo(float x, float y){
	ofFill();
	ofLine(fXPos, fYPos, x, y);
	fXPos = x;
	fYPos = y;
}
void ofxGuidoDevice::Line(float x1, float y1, float x2, float y2){
	ofFill();
	ofLine(x1, y1, x2, y2);
}
void ofxGuidoDevice::Frame(float left, float top, float right, float bottom){
	ofNoFill();
	ofSetLineWidth(fLineThick);
	ofRect(left, top, right - left, bottom - top);
}

void ofxGuidoDevice::Arc(float left, float top, float right, float bottom, float startX,
	float startY, float endX, float endY){
	const float midX = (left + right) * 0.5f;
	const float midY = (top + bottom) * 0.5f;
	const float width = right - left;
	const float height = bottom - top;
	const float fromRadians = CoordToRadian(startX - midX, startY - midY);
	const float toRadians = CoordToRadian(endX - midX, endY - midY);
	ofPath path;
	path.arc(left, top, width, height, fromRadians, toRadians, true);
	path.setFilled(true);
	path.draw();
}

// - Filled surfaces --------------------------------------
void ofxGuidoDevice::Triangle(float x1, float y1, float x2, float y2, float x3, float y3){
	ofFill();
	ofTriangle(x1, y1, 0, x2, y2, 0, x3, y3, 0);
}

void ofxGuidoDevice::Polygon(const float * xCoords, const float * yCoords, int count){
	if(count < 2)
		return;

	ofPath path;

	path.newSubPath();
	path.moveTo(xCoords[0], yCoords[0]);
	for(int i = 1; i < count; i++)
		path.lineTo(xCoords[i], yCoords[i]);
	path.setFilled(true);
	path.draw();
}

void ofxGuidoDevice::Rectangle(float left, float top, float right, float bottom){
	ofFill();
	ofRect(left, top, right - left, bottom - top);
}

// - Pen & brush services --------------------------------------------
void ofxGuidoDevice::SelectPen(const VGColor & color, float witdh){
	ofSetColor(Color2ofColor(color));
	ofSetLineWidth(witdh);
}
void ofxGuidoDevice::PushPen(const VGColor & color, float width){
	ofPushStyle();
	ofSetColor(Color2ofColor(color));
	ofSetLineWidth(width);
}
void ofxGuidoDevice::PopPen(){
	ofPopStyle();
}

void ofxGuidoDevice::SelectFillColor(const VGColor & color){
	ofFill();
	fFillColor = color;
	ofSetColor(Color2ofColor(color));
}
void ofxGuidoDevice::PushFillColor(const VGColor & color){
	fFillColorStack.push(color);
	SelectFillColor(color);
}
void ofxGuidoDevice::PopFillColor(){
	fFillColorStack.pop();
	SelectFillColor(fFillColorStack.top());
}

void ofxGuidoDevice::SetRasterOpMode(VRasterOpMode ROpMode){
	fRasterOpMode = ROpMode;
}
VGDevice::VRasterOpMode ofxGuidoDevice::GetRasterOpMode() const{
	return fRasterOpMode;
}

// - Bitmap services (bit-block copy methods) --------------------------
//>>>>>>>>>>>>>>>> todo
bool ofxGuidoDevice::CopyPixels(VGDevice* /*pSrcDC*/, float /*alpha*/){
	return false;
}

bool ofxGuidoDevice::CopyPixels(int /*xDest*/, int /*yDest*/, VGDevice* /*pSrcDC*/, int /*xSrc*/,
	int /*ySrc*/, int /*srcWidth*/, int /*srcHeight*/, float /*alpha*/){
	return false;
}

bool ofxGuidoDevice::CopyPixels(int /*xDest*/, int /*yDest*/, int /*dstWidth*/, int /*dstHeight*/,
	VGDevice* /*pSrcDC*/, float /*alpha*/){
	return false;
}

bool ofxGuidoDevice::CopyPixels(int /*xDest*/, int /*yDest*/, int /*dstWidth*/, int /*dstHeight*/,
	VGDevice* /*pSrcDC*/, int /*xSrc*/, int /*ySrc*/, int /*srcWidth*/, int /*srcHeight*/,
	float /*alpha*/){
	return false;
}
//>>>>>>>>>>>>>>>> todo

// - Coordinate services ------------------------------------------------
void ofxGuidoDevice::SetOrigin(float x, float y){
	//cout << "openFrameworksDevice::SetOrigin:" << x << " " << y << endl;
	ofTranslate(x - fXOrigin, y - fYOrigin);
	ofTranslate(x, y);
	fXOrigin = x;
	fYOrigin = y;
}
void ofxGuidoDevice::OffsetOrigin(float x, float y){
	//cout << "openFrameworksDevice::OffsetOrigin" << x << " " << y <<  endl;
	ofTranslate(x, y);
	fXOrigin += x;
	fYOrigin += y;
}

float ofxGuidoDevice::GetXOrigin() const{
	return fXOrigin;
}
float ofxGuidoDevice::GetYOrigin() const{
	return fYOrigin;
}

void ofxGuidoDevice::LogicalToDevice(float * x, float * y) const{
	*x = (*x * fXScale - fXOrigin);
	*y = (*y * fYScale - fYOrigin);
}

void ofxGuidoDevice::DeviceToLogical(float * x, float * y) const{
	*x = (*x + fXOrigin) / fXScale;
	*y = (*y + fYOrigin) / fYScale;
}

void ofxGuidoDevice::SetScale(float x, float y){
	//cout << "openFrameworksDevice::SetScale " << x << " " << y << endl;
	ofScale(x, y);
	fXScale = x;
	fYScale = y;
}
float ofxGuidoDevice::GetXScale() const{
	return fXScale;
}
float ofxGuidoDevice::GetYScale() const{
	return fYScale;
}

void ofxGuidoDevice::NotifySize(int width, int height){
	fWidth = width;
	fHeight = height;

	int w, h;
	ofFbo::Settings settings;
#ifdef ASCOGRAPH_IOS
	fWidth = 1024;
	fHeight = 1024;

	//GLint dims[2];
	//glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &dims[0]);
	//fWidth = w = dims[0]; fHeight = h = dims[1];
	settings.width = fWidth;
	settings.height = fHeight;
	settings.numSamples = 0;
#else

	fWidth = 1024;
	fHeight = 1024;
	settings.width = fWidth;
	settings.height = fHeight;
	glClear (GL_COLOR_BUFFER_BIT);
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable (GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE_ARB);
	settings.numSamples = 8;
#endif
	//cout << "openFrameworksDevice::NotifySize: allocating FBO: " << fWidth << "x" << fHeight << endl;
	settings.internalformat = GL_RGBA;
	settings.useDepth = false;
	settings.useStencil = false;

	drawCache.allocate(settings);

	drawCache.begin();
	ofClear(255, 255, 255, 0);
	drawCache.end();

}
int ofxGuidoDevice::GetWidth() const{
	return fWidth;
}
int ofxGuidoDevice::GetHeight() const{
	return fHeight;
}

// - Font services ---------------------------------------------------
void ofxGuidoDevice::SetMusicFont(const VGFont * font){
	fMusicFont = font;
	if(fCurrentFont != font){
		//fGraphics->setFont(static_cast<const openFrameworksFont*>(font)->NativeFont()); 
		fCurrentFont = font;
	}
//	ofLogNotice("AurinDebug") << "SetMusicFont " << font->GetName();
}
const VGFont * ofxGuidoDevice::GetMusicFont() const{
	return fMusicFont;
}
void ofxGuidoDevice::SetTextFont(const VGFont * font){
	if(strcmp(font->GetName(), "Times") == 0) {
		fTextFont = font = FontManager::gFontText;
	} else {
		fTextFont = font;
	}
	if(fCurrentFont != font){
		//fGraphics->setFont(static_cast<const openFrameworksFont*>(font)->NativeFont());
		fCurrentFont = font;
	}
	ofLogNotice("AurinDebug") << "SetTextFont " << font->GetName();
}
const VGFont * ofxGuidoDevice::GetTextFont() const{
	return fTextFont;
}

// - Text and music symbols services -------------------------------------
void ofxGuidoDevice::DrawMusicSymbol(float x, float y, unsigned int inSymbolID){
	//cout << "openFrameworksDevice::DrawMusicSymbol: " << x << ", " << y << " id=" << inSymbolID << endl;
	string text;
	text += wchar_t(inSymbolID);

	ofSetColor(Color2ofColor(fFontColor));
	ofTrueTypeFont* f =
		(ofTrueTypeFont*)(&static_cast<const ofxGuidoFont*>(fCurrentFont)->NativeFont());
	f->drawStringAsShapes(text, int(x), int(y));
	//f->drawString (text, int(x), int(y));
	ofSetColor(Color2ofColor(fFillColor));
//	ofLogNotice("AurinDebug") << "DrawMusicSymbol " << inSymbolID << " using " << fCurrentFont->GetName() << " and color (" << fFontColor.mRed << "," << fFontColor.mGreen << "," << fFontColor.mBlue << "," << fFontColor.mAlpha << ",";
}

void ofxGuidoDevice::DrawString(float x, float y, const char * s, int inCharCount){
	float w, h;
	fTextFont->GetExtent(s, inCharCount, &w, &h, this);
	if(fFontAlign & kAlignCenter)
		x -= w / 2;
	else if(fFontAlign & kAlignRight)
		x -= w;

	string text(s, inCharCount);

	ofSetColor(Color2ofColor(fFontColor));
	ofTrueTypeFont* f =
		(ofTrueTypeFont*)(&static_cast<const ofxGuidoFont*>(fTextFont)->NativeFont());
	f->drawString(text, int(x), int(y));
	ofSetColor(Color2ofColor(fFillColor));
//	ofLogNotice("AurinDebug") << "DrawString at (" << x << "," << y << ") '" << text << "' using " << fCurrentFont->GetName();
}

void ofxGuidoDevice::SetFontColor(const VGColor & c){
	fFontColor = c;
}
VGColor ofxGuidoDevice::GetFontColor() const{
	return fFontColor;
}
void ofxGuidoDevice::SetFontBackgroundColor(const VGColor & c){
	fFontBackgroundColor = c;
}
VGColor ofxGuidoDevice::GetFontBackgroundColor() const{
	return fFontBackgroundColor;
}
void ofxGuidoDevice::SetFontAlign(unsigned int align){
	fFontAlign = align;
}
unsigned int ofxGuidoDevice::GetFontAlign() const{
	return fFontAlign;
}

// - Printer informations services ----------------------------------------
void ofxGuidoDevice::SetDPITag(float inDPI){
	fDPI = inDPI;
}
float ofxGuidoDevice::GetDPITag() const{
	return fDPI;
}

// - VGDevice extension --------------------------------------------
void ofxGuidoDevice::SelectPenColor(const VGColor & color){
	fPenColor = color;
}
void ofxGuidoDevice::PushPenColor(const VGColor & color){
	fPenColor = color;
	fPenColorStack.push(color);
}
void ofxGuidoDevice::PopPenColor(){
	fPenColorStack.pop();
	fPenColor = fPenColorStack.top();
}

void ofxGuidoDevice::SelectPenWidth(float width){
	fLineThick = width;
	ofSetLineWidth(fLineThick);
}
void ofxGuidoDevice::PushPenWidth(float width){
	fPenWidthStack.push(width);
	fLineThick = width;
}
void ofxGuidoDevice::PopPenWidth(){
	fLineThick = fPenWidthStack.top();
	fPenWidthStack.pop();
}

