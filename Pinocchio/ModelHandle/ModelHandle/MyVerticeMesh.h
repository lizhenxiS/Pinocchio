
#ifndef _MYVERTICEMESH_H__
#define _MYVERTICEMESH_H__

#include "mesh.h"

class MyVerticeMesh{

public:
	MyVerticeMesh(Mesh m);

public: //data
	vector<MeshVertex> vertices;

};

#endif	//_MYVERTICEMESH_H__