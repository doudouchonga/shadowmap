#include <glad/glad.h>
#include "shadowmap_mgr.h"
#include "global.h"
#include <algorithm>
#include "shader.h"
#include "render_mesh.h"
#include "worldbase.h"

ShadowmapMgr::ShadowmapMgr()
{
	depthMapFBO = -1;
	depthMap = -1;
}

ShadowmapMgr::~ShadowmapMgr()
{

}

void ShadowmapMgr::init_depth_map()
{
	// configure depth map FBO
	// -----------------------
	const unsigned int SHADOW_WIDTH = GlobalVar::SCR_WIDTH, SHADOW_HEIGHT = GlobalVar::SCR_HEIGHT;
	//const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	depthShader = new Shader("resources/shader/shadow_mapping_depth.vs", "resources/shader/shadow_mapping_depth.fs");
}

void ShadowmapMgr::set_cast_shadow_light(PointLight * l)
{
	this->cast_shadow_light = l;
}

void ShadowmapMgr::set_cast_shadow_entites(RenderMesh* ent, bool add)
{
	auto find_it = std::find(cast_shadow_entites.begin(), cast_shadow_entites.end(), ent);
	if (find_it == cast_shadow_entites.end())
	{
		if (add)
		{
			cast_shadow_entites.push_back(ent);
		}
		
	}
	else
	{
		if (!add)
		{
			cast_shadow_entites.erase(find_it);
		}
	}
}

void ShadowmapMgr::set_cast_shadow_models(Model* ent, bool add)
{
	auto find_it = std::find(cast_shadow_models.begin(), cast_shadow_models.end(), ent);
	if (find_it == cast_shadow_models.end())
	{
		if (add)
		{
			cast_shadow_models.push_back(ent);
		}

	}
	else
	{
		if (!add)
		{
			cast_shadow_models.erase(find_it);
		}
	}
}

glm::mat4 ShadowmapMgr::getlightSpaceMatrix()
{
	glm::mat4 lightProjection, lightView;
	float near_plane = 1.0f, far_plane = 10.5f;

	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(cast_shadow_light->position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));//cast_shadow_light->position
	/*glm::mat4 projection = glm::perspective(glm::radians(GlobalVar::GAME_WORLD->camera->Zoom), (float)GlobalVar::SCR_WIDTH / (float)GlobalVar::SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = GlobalVar::GAME_WORLD->camera->GetViewMatrix();
	return projection * view;*/
	return lightProjection * lightView;
}

void ShadowmapMgr::render_depth_map()
{
	if (cast_shadow_entites.size() == 0 && cast_shadow_models.size() == 0)
	{
		return; 
	}

	glm::mat4 lightSpaceMatrix = getlightSpaceMatrix();
	
	//为什么要设置成1024x1024呢
	//glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//BUG clear操作注意放到绑定fbo之后，不然等于这个fbo会一直没有清理过缓存
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	depthShader->use();
	//DEBUG
	//depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	
	depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

	for (auto it = cast_shadow_entites.begin(); it != cast_shadow_entites.end(); ++it)
	{
		(*it)->render(GlobalVar::GAME_WORLD->camera, depthShader, false);
	}

	for (auto it = cast_shadow_models.begin(); it != cast_shadow_models.end(); ++it)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.0, -1.0, 0.0));
		model = glm::scale(model, glm::vec3(0.25f));
		depthShader->setMat4("model", model);
		(*it)->Draw(depthShader);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}