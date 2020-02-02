#ifndef LIGHT_H
#define LIGHT_H
#include <glm.hpp>
enum LightType
{
    LT_DIR = 0,
    LT_POINT,
    LT_SPOT,
};

struct LightBase
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct DirLight: LightBase {
    glm::vec3 direction;
	

};

struct PointLight: LightBase {
    glm::vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
};

struct SpotLight: LightBase {
    glm::vec3 position;
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
        
};


#endif