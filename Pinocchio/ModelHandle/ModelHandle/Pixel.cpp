

#include "Pixel.h"

Pixel::Pixel()
{
	length = 0;
	wide = 0;
	height = 0;
	attachPoint = -1;
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;
}

Pixel::Pixel(float* pos, double l, double w, double h, int ap)
{
	length = l;
	wide = w;
	height = h;
	attachPoint = ap;
	this->pos[0] = pos[0];
	this->pos[1] = pos[1];
	this->pos[2] = pos[2];
}

double Pixel::getVolume()
{
	return length * wide * height; 
}


