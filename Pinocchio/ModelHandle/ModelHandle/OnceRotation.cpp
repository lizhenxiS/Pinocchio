

#include "OnceRotation.h"
#include "GenerateTool.h"


OnceRotation::OnceRotation()
{
	angle = 0;
	lineStart = Vector3(0, 0, 0);
	lineEnd = Vector3(0, 0, 0);
}

OnceRotation::OnceRotation(double angle, Vector3 lineStart, Vector3 lineEnd)
{
	this->angle = angle;
	this->lineStart = lineStart;
	this->lineEnd = lineEnd;
}

Vector3 OnceRotation::pointRotate(Vector3 point)
{
	return rotateArroundLine(point, lineStart, lineEnd, angle);
}
