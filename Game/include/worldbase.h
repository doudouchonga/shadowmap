#ifndef WORLDBASE_H
#define WORLDBASE_H

#include <GLFW/glfw3.h>
#include "light.h"

#include <vector>
#include "shadowmap_mgr.h"
class Gbuff;
class Model;
class Camera;
class Shader;
class RenderMesh;

class Worldbase
{
public:
	Worldbase();
	~Worldbase();
	void init();
	void render(GLFWwindow *window);
	void render_forward();
	void render_deferred();
	void processInput(GLFWwindow *window);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	Camera *camera;
	float lastX;
	float lastY;
	bool firstMouse;

	// timing
	float deltaTime;
	float lastFrame;

	Shader *plan_shader;
	RenderMesh *plan_mesh;

	std::vector<RenderMesh *> scene_entities;
	void add_scene_entity(RenderMesh * ent);

	std::vector<Model *> scene_models;
	void add_scene_model(Model * ent);

	std::vector<DirLight*> dirLights;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;

 	int add_light(LightType light_type);
 	LightBase* get_light(int i);

	void set_light_info(Shader* shader);
	void set_camera_info(Shader *shader);
	ShadowmapMgr * shadowmap_mgr;
	Shader *debugDepthQuad;
	//
	Gbuff* gbuff;

};

#endif