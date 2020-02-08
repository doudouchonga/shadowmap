#ifndef GBUFF_H
#define GBUFF_H

#include <glad/glad.h> 

#include <string>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
class Shader;
class Gbuff
{
public:
	Gbuff();
	void init();
	void use();
	void unuse();
	void bind();
	unsigned int gBuffer;
    unsigned int gPosition, gNormal, gAlbedoSpec, gPositionLightSpace;
	Shader *gbuff_shader;
	Shader *shaderLightingPass;
};

#endif