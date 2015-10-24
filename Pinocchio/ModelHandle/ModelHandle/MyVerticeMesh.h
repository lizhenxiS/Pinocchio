
#ifndef _MYVERTICEMESH_H__
#define _MYVERTICEMESH_H__

#include "mesh.h"

/*
作为Mesh类的自定义类，仅保留Mesh中的点的位置信息
*/
class MyVerticeMesh{

public:
	MyVerticeMesh(Mesh m);
	MyVerticeMesh(const MyVerticeMesh& m);

public: //data
	vector<MeshVertex> vertices;

};

#endif	//_MYVERTICEMESH_H__