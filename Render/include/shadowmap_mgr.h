#ifndef SHAWMAP_MGR_H
#define SHAWMAP_MGR_H

#include <vector>
#include "render_mesh.h"
#include "light.h"
#include "model.h"

class GameObject;
class Shader;
class ShadowmapMgr
{
public:
	ShadowmapMgr();
	~ShadowmapMgr();

	unsigned int depthMapFBO;
	unsigned int depthMap;
	void init_depth_map();

	//
	std::vector<GameObject*> cast_shadow_entites;

	DirLight *cast_shadow_light;
	Shader *depthShader;
	void set_cast_shadow_light(DirLight * l);
	void set_cast_shadow_entites(GameObject* ent, bool add);
	
	void render_depth_map();
	glm::mat4 getlightSpaceMatrix();
	void bind_depth_map(Shader *shader, unsigned int index);
	glm::mat4 lightProjection, lightView;

	void update_shadow_light_pos(float x, float y, float z);
	glm::vec3 light_pos;
};

#endif
