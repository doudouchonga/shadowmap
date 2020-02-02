#ifndef SHAWMAP_MGR_H
#define SHAWMAP_MGR_H

#include <vector>
#include "render_mesh.h"
#include "light.h"

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
	std::vector<RenderMesh*> cast_shadow_entites;
	PointLight *cast_shadow_light;
	Shader *depthShader;
	void set_cast_shadow_light(PointLight * l);
	void set_cast_shadow_entites(RenderMesh* ent, bool add);
	void render_depth_map();
	glm::mat4 getlightSpaceMatrix();
};

#endif
