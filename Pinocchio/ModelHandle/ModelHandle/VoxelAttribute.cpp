

#include "VoxelAttribute.h"

VoxelAttribute::VoxelAttribute(int belongBone, double belongWeight, int linkVertex, int belongIndex)
{
	this->belongBone = belongBone;
	this->belongWeight = belongWeight;
	this->linkVertex = linkVertex;
	this->belongIndex = belongIndex;
}

VoxelAttribute::VoxelAttribute()
{

}

