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

	void render(Shader *shader);
	void setupExt(std::vector<float> data, std::vector<unsigned int> indices, const std::string& diffuse_path, const std::string& specular_path = "");
	unsigned int VAO;
	unsigned int draw_count;
	int render_type;
	//

	//
	Material material;
	unsigned int albedo;
	unsigned int normal;
	unsigned int metallic;
	unsigned int roughness;
	unsigned int ao;

	//
	void setPbrTex(const std::string& ironAlbedoMap, const std::string& ironNormalMap, const std::string& ironMetallicMap, const std::string& ironRoughnessMap, const std::string& ironAOMap);
};

#endif