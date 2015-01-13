#include "ImageAnimate.h"

ImageAnimate::ImageAnimate():
frameDelay(0), currentCount(0)
{
	
}

ImageAnimate::ImageAnimate( std::vector<LTexture> v, int d) :
frameDelay(d), currentCount(0), frames(v)
{

}

ImageAnimate::~ImageAnimate()
{

}

void ImageAnimate::setBox(int x, int y, int w, int h)
{
	this->mBox.x = x;
	this->mBox.y = y;
	this->mBox.w = w;
	this->mBox.h = h;
	return;
}