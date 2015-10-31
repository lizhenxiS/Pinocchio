

#include "Pixel.h"

Pixel::Pixel()
{
	length = 0;
	width = 0;
	height = 0;
	attachPoint = -1;
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 3; j++)
			box[i][j] = 0;
	}
}

Pixel::Pixel(float* pos, double l, double w, double h, int ap)
{
	length = l;
	width = w;
	height = h;
	attachPoint = ap;
	this->pos[0] = pos[0];
	this->pos[1] = pos[1];
	this->pos[2] = pos[2];
	double x = this->pos[0];
	double y = this->pos[1];
	double z = this->pos[2];
	double halfLen = length / 2;
	double halfWidth = width / 2;
	double halfHeight = height / 2;
	double solid[8][3] = {
		{ x - halfLen, y + halfHeight, z - halfWidth },
		{ x + halfLen, y + halfHeight, z - halfWidth },
		{ x + halfLen, y + halfHeight, z + halfWidth },
		{ x - halfLen, y + halfHeight, z + halfWidth },
		{ x - halfLen, y - halfHeight, z - halfWidth },
		{ x + halfLen, y - halfHeight, z - halfWidth },
		{ x + halfLen, y - halfHeight, z + halfWidth },
		{ x - halfLen, y - halfHeight, z + halfWidth }
	};
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 3; j++)
			box[i][j] = solid[i][j];
	}
}

void Pixel::updateLWH()
{
	length = box[1][0] - box[0][0];
	width = box[2][2] - box[1][2];
	height = box[3][1] - box[4][1];
}

double Pixel::getVolume()
{
	return length * width * height;
}


