#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glad/glad.h> 
#include <glm.hpp>
#include <string>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class RenderMesh;
class Model;
class Shader;
class GameObject
{
public:
	GameObject();
	float angle;
	glm::vec3 position;
	glm::vec3 rote_axis;
	bool rote_enable;
	float scale;
	glm::mat4 getModeMatrix();

	//
	void set_cast_shadow(bool c);
	void render(Shader * shader);
	RenderMesh *render_mesh;
	Model *model_mesh;

	float metallic;
	float roughness;

};

#endif