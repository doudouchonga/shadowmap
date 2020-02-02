#ifndef RENDER_MESH_H
#define RENDER_MESH_H
#include <iostream>
#include "camera.h"
#include "shader.h"
#include <glm.hpp>

struct Material {
    std::string diffuse_path;
    std::string specular_path;
    float shininess;
	unsigned int diffuse_tex;
	unsigned int specular_tex;
}; 

class RenderMesh
{
public:
	RenderMesh();
	~RenderMesh();
	void setup(int ver_size, float *Vertices, const std::string& diffuse_path, const std::string& specular_path = "");
	void set_shader(Shader* shader);
	void render(Camera *cam,Shader *shader = NULL, bool set_use = true);
	unsigned int VAO;
	unsigned int draw_count;
	//
	float angle;
	glm::vec3 position;
	glm::vec3 rote_axis;
	bool rote_enable;
	glm::mat4 getModeMatrix();

	//
	Material material;
	Shader *render_shader;
	void set_cast_shadow(bool c);
private:
	//
	bool cast_shadow;
};

#endif