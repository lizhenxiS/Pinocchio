

#ifndef _SKELETONNODE_H__
#define _SKELETONNODE_H__

#include <vector.h>
#include <vector>
using namespace std;

class SkeletonNode
{
public :
	int parentIndex;
	vector<int> childrenIndex;
	Vector3 pos;
};



#endif	//_SKELETONNODE_H__