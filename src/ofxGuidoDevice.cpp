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
	fPenColor = VGColor(0, 0, 0);
	fFillColor = VGColor(0, 0, 0);
	fFontColor = VGColor(0, 0, 0);
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
}

// --------------------------------------------------------------
ofxGuidoDevice::~ofxGuidoDevice(){
}

// - Drawing services ------------------------------------------------
// --------------------------------------------------------------
bool ofxGuidoDevice::BeginDraw(){
	ofPushStyle();
	ofLogNotice("ofxGuido") << "vvvvvvv GUIDO BEGIN vvvvvvv";
	return true;
}

void ofxGuidoDevice::EndDraw(){
	ofPopStyle();
	ofLogNotice("ofxGuido") << "^^^^^^^^ GUIDO END ^^^^^^^^";
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
	ofColor(Color2ofColor(fPenColor));
	ofSetLineWidth(fLineThick);
	ofLine(fXPos, fYPos, x, y);
	fXPos = x;
	fYPos = y;
//	ofLogNotice("ofxGuido") << "LineTo (" << x << "," << y << ")";
}
void ofxGuidoDevice::Line(float x1, float y1, float x2, float y2){
	ofSetColor(Color2ofColor(fPenColor));
	ofSetLineWidth(fLineThick);
	ofLine(x1, y1, x2, y2);
//	ofLogNotice("ofxGuido") << "Line " << fLineThick << " px thick from (" << x1 << "," << y1 << ") to (" << x2 << "," << y2 << ") using color (" << (int) fPenColor.mRed <<"," << (int) fPenColor.mGreen <<"," << (int) fPenColor.mBlue <<"," << (int) fPenColor.mAlpha << ")";
}
void ofxGuidoDevice::Frame(float left, float top, float right, float bottom){
	ofNoFill();
	ofSetColor(Color2ofColor(fPenColor));
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
	path.setFillColor(Color2ofColor(fFillColor));
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
	path.setFillColor(Color2ofColor(fFillColor));
	path.setStrokeWidth(1);	// Draws better slurs and ties
	path.setStrokeColor(Color2ofColor(fFillColor));
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
	ofTranslate(x - fXOrigin, y - fYOrigin);
	ofTranslate(x, y);
	fXOrigin = x;
	fYOrigin = y;
}
void ofxGuidoDevice::OffsetOrigin(float x, float y){
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
		fCurrentFont = font;
	}
}

const VGFont * ofxGuidoDevice::GetMusicFont() const{
	return fMusicFont;
}

void ofxGuidoDevice::SetTextFont(const VGFont * font){
	// Workaround over "Times" being hardcoded at some places in guidolib
	if(strcmp(font->GetName(), "Times") == 0) {
		fTextFont = font = FontManager::gFontText;
	} else {
		fTextFont = font;
	}
	if(fCurrentFont != font){
		fCurrentFont = font;
	}
}

const VGFont * ofxGuidoDevice::GetTextFont() const{
	return fTextFont;
}

// - Text and music symbols services -------------------------------------
void ofxGuidoDevice::DrawMusicSymbol(float x, float y, unsigned int inSymbolID){
	string text;
	text += wchar_t(inSymbolID);

	ofSetColor(Color2ofColor(fFontColor));
	ofTrueTypeFont* f =
		(ofTrueTypeFont*)(&static_cast<const ofxGuidoFont*>(fCurrentFont)->NativeFont());
	f->drawString (text, int(x), int(y));
//	ofLogNotice("ofxGuido") << "DrawMusicSymbol at (" << x << "," << y << ") " << inSymbolID << " using " << fCurrentFont->GetName() << " and color (" << fFontColor.mRed << "," << fFontColor.mGreen << "," << fFontColor.mBlue << "," << fFontColor.mAlpha << ",";
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
	const ofxGuidoFont * gf = static_cast<const ofxGuidoFont *>(fTextFont);
	ofTrueTypeFont f = gf->NativeFont();
	f.drawString(text, int(x), int(y) + gf->GetMHeight());
//	ofLogNotice("ofxGuido") << "DrawString at (" << x << "," << y << ") '" << text << "' using " << fCurrentFont->GetName();
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
//	ofLogNotice("ofxGuido") << "SelectPenColor (" <<"," << (int) color.mRed <<"," << (int) color.mGreen <<"," << (int) color.mBlue <<"," << (int) color.mAlpha << ")";
}

void ofxGuidoDevice::PushPenColor(const VGColor & color){
	fPenColor = color;
	fPenColorStack.push(color);
//	ofLogNotice("ofxGuido") << "PushPenColor (" <<"," << (int) color.mRed <<"," << (int) color.mGreen <<"," << (int) color.mBlue <<"," << (int) color.mAlpha << ")";
}

void ofxGuidoDevice::PopPenColor(){
	fPenColorStack.pop();
	fPenColor = fPenColorStack.top();
}

void ofxGuidoDevice::SelectPenWidth(float width){
	fLineThick = width / 4.0; // kLineThick = 4, normalize to pixels used by ofLine()
	ofSetLineWidth(fLineThick);
//	ofLogNotice("ofxGuido") << "SelectPenWidth (" << fLineThick << ")";
}

void ofxGuidoDevice::PushPenWidth(float width){
	fLineThick = width / 4.0; // kLineThick = 4, normalize to pixels used by ofLine()
	fPenWidthStack.push(fLineThick);
//	ofLogNotice("ofxGuido") << "PushPenWidth (" << fLineThick << ")";
}

void ofxGuidoDevice::PopPenWidth(){
	fLineThick = fPenWidthStack.top();
	fPenWidthStack.pop();
//	ofLogNotice("ofxGuido") << "PopPenWidth (" << fLineThick << ")";
}

