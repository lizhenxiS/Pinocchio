
#include "MyVerticeMesh.h"

MyVerticeMesh::MyVerticeMesh(Mesh m)
{
	vector<MeshVertex>::iterator ite;

	for (ite = m.vertices.begin(); ite != m.vertices.end(); ite++)
	{
		vertices.push_back(*ite);
	}
}