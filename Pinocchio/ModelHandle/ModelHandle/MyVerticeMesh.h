
#ifndef _MYVERTICEMESH_H__
#define _MYVERTICEMESH_H__

#include "mesh.h"

/*
��ΪMesh����Զ����࣬������Mesh�еĵ��λ����Ϣ
*/
class MyVerticeMesh{

public:
	MyVerticeMesh(Mesh m);
	MyVerticeMesh(const MyVerticeMesh& m);

public: //data
	vector<MeshVertex> vertices;

};

#endif	//_MYVERTICEMESH_H__