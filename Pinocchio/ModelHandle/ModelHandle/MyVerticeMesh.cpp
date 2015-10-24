
#include "MyVerticeMesh.h"

MyVerticeMesh::MyVerticeMesh(Mesh m)
{
	vector<MeshVertex>::iterator ite;

	for (ite = m.vertices.begin(); ite != m.vertices.end(); ite++)
	{
		vertices.push_back(*ite);
	}
}

MyVerticeMesh::MyVerticeMesh(const MyVerticeMesh& m)
{
	for (int i = 0; i < m.vertices.size(); i++)
	{
		vertices.push_back(m.vertices[i]);
	}
}