

#ifndef __SKELETONINFO_H__
#define __SKELETONINFO_H__

/*
骨骼信息类
存放骨骼自由度信息
*/
class SkeletonInfo
{
public :
	int freeActor;	//骨骼自由度

	SkeletonInfo();
	SkeletonInfo(int free);
};


#endif	//__SKELETONINFO_H__