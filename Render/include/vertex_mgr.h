#ifndef VERTEX_MGR_H
#define VERTEX_MGR_H

#include <glad/glad.h> 

#include <string>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
struct Vertex;
struct VertexUnit
{
	int type;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};

class VertexMgr
{
public:
	VertexMgr();
	map<string, VertexUnit> vao_pool;
	
	int get_vao(string key, vector<Vertex> *vertices, vector<unsigned int> *indices);
	int VertexMgr::get_vao_by_data(string key, int ver_size, float *Vertices);
};

#endif