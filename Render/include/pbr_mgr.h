#ifndef PBR_MGR_H
#define PBR_MGR_H

#include <glad/glad.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <string>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
class Shader;
class PbrMgr
{
public:
	PbrMgr();
	void init(string env_map_path);
	void init_cubeMap();
	void init_irradianceMap();
	void init_prefilterMap();
	void init_brdfLUT();

	void render_env_map();

	Shader *pbrShader;
	Shader *equirectangularToCubemapShader;
	Shader *irradianceShader;
	Shader *prefilterShader;
	Shader *brdfShader;
	Shader *backgroundShader;

	glm::mat4 captureProjection;
	glm::mat4 captureViews[6];
	
	unsigned int captureFBO;
	unsigned int captureRBO;
	unsigned int envCubemap;
	unsigned int irradianceMap;
	unsigned int prefilterMap;
	unsigned int brdfLUTTexture;

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	void renderQuad();
	void renderCube();

};

#endif