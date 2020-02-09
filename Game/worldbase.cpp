#include "camera.h"
#include "worldbase.h"
#include "global.h"
#include "shader.h"
#include "render_mesh.h"
#include "shadowmap_mgr.h"
#include "model.h"
#include "filesystem.h"
#include "gbuff.h"
#include "game_object.h"

#define NR_POINT_LIGHTS 64
#define NR_DIR_LIGHTS 1
#define NR_SPOT_LIGHTS 1

Worldbase::Worldbase()
{
	camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	lastX = (float)GlobalVar::SCR_WIDTH / 2.0;
	lastY = (float)GlobalVar::SCR_HEIGHT / 2.0;
	firstMouse = true;

	// timing
	deltaTime = 0.0f;
	lastFrame = 0.0f;

	GlobalVar::GAME_WORLD = this;
	shadowmap_mgr = new ShadowmapMgr();
}

Worldbase::~Worldbase()
{

}

void Worldbase::initSphere()
{

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359;
	for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
	{
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positions.push_back(glm::vec3(xPos, yPos, zPos));
			uv.push_back(glm::vec2(xSegment, ySegment));
			normals.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}

	bool oddRow = false;
	for (int y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (int x = 0; x <= X_SEGMENTS; ++x)
			{
				indices.push_back(y       * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.push_back(y       * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	std::vector<float> data;
	for (int i = 0; i < positions.size(); ++i)
	{
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);
		if (uv.size() > 0)
		{
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}
		if (normals.size() > 0)
		{
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}
	}

	RenderMesh *sphere_mesh = new RenderMesh();
	sphere_mesh->setupExt(data, indices);
	GameObject *sphere_obj = new GameObject();
	sphere_obj->render_mesh = sphere_mesh;
	sphere_obj->position = glm::vec3(0, 10, 0);
	sphere_obj->set_cast_shadow(true);
	add_scene_entity(sphere_obj);
}

void Worldbase::init()
{
	float currentFrame1 = glfwGetTime();
	float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };

    //plan_shader = new Shader("resources/shader/unlit_texture.vs", "resources/shader/unlit_texture.fs");
    //plan_shader = new Shader("resources/shader/multiple_lights.vs", "resources/shader/multiple_lights.fs");
    plan_shader = new Shader("resources/shader/multiple_lights_shadow.vs", "resources/shader/multiple_lights_shadow.fs");
	RenderMesh *plan_mesh = new RenderMesh();
    plan_mesh->setup(sizeof(planeVertices), planeVertices, "resources/textures/wood.png");
	GameObject *plan_obj = new GameObject();
	plan_obj->set_cast_shadow(true); 
	plan_obj->render_mesh = plan_mesh;
	add_scene_entity(plan_obj);

	float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    //cube_shader = new Shader("resources/shader/unlit_texture.vs", "resources/shader/unlit_texture.fs");
    //cube_shader = new Shader("resources/shader/multiple_lights.vs", "resources/shader/multiple_lights.fs");
	
	//Shader *cube_shader = new Shader("resources/shader/multiple_lights_shadow.vs", "resources/shader/multiple_lights_shadow.fs");
	RenderMesh *cube_mesh = new RenderMesh();
	cube_mesh->setup(sizeof(vertices), vertices, "resources/textures/container2.png", "resources/textures/container2_specular.png");
	for (unsigned int i = 0; i < 100; i++)
	{
		GameObject *cube_obj = new GameObject();
		cube_obj->set_cast_shadow(true);
		cube_obj->render_mesh = cube_mesh;
		cube_obj->rote_enable = true;
		cube_obj->rote_axis = glm::vec3(1.0f, 0.3f, 0.5f);

		float xPos = ((rand() % 50)) - 25;
		float yPos = ((rand() % 10));
		float zPos = ((rand() % 30)) - 25;

		cube_obj->position = glm::vec3(xPos, yPos, zPos);
		cube_obj->angle = 20 * i;
		add_scene_entity(cube_obj);
	}
	float currentFrame2 = glfwGetTime();
	std::cout << "1111:" << currentFrame2 - currentFrame1 << std::endl;
	//light
	const unsigned int NR_LIGHTS = 40;
	srand(13);
	for (unsigned int i = 0; i < NR_LIGHTS; i++)
	{
		// calculate slightly random offsets
		float xPos = ((rand() % 40) ) - 15;
		float yPos = ((rand() % 10) );
		float zPos = ((rand() % 40) ) - 15;
		
		auto index = this->add_light(LT_POINT);
		PointLight* l = (PointLight*)this->get_light(index);
		l->position = glm::vec3(xPos, yPos, zPos);//1.7f, 0.2f, 2.0f //-2.0f, 4.0f, -1.0f
		//l->position = glm::vec3(1.7f, 0.2f, 2.0f);//1.7f, 0.2f, 2.0f //-2.0f, 4.0f, -1.0f
		l->ambient = glm::vec3(0.05f, 0.05f, 0.05f);
		l->diffuse = glm::vec3(0.99f, 0.99f, 0.99f);
		l->specular = glm::vec3(0.1f, 0.1f, 0.1f);
		l->constant = 0.1f;
		l->linear = 0.85;
		l->quadratic = 0.002;
		//

	}
	//


	float currentFrame3 = glfwGetTime();
	std::cout << "2222:" << currentFrame3 - currentFrame2 << std::endl;
	//
	//
	glEnable(GL_DEPTH_TEST);
	shadowmap_mgr->init_depth_map();
	//
	debugDepthQuad = new Shader("resources/shader/debug_quad_depth.vs", "resources/shader/debug_quad_depth.fs");
	debugDepthQuad->use();
	debugDepthQuad->setInt("depthMap", 0);
	//
	Model *nanosuit = new Model(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));

	for (unsigned int i = 0; i < 100; i++)
	{
		float xPos = ((rand() % 40)) - 15;
		float yPos = 0;
		float zPos = ((rand() % 40)) - 15;

		
		GameObject *nanosuit_obj = new GameObject();
		nanosuit_obj->model_mesh = nanosuit;
		nanosuit_obj->scale = 0.25;
		nanosuit_obj->position = glm::vec3(xPos, yPos, zPos);
		nanosuit_obj->set_cast_shadow(true);
		add_scene_entity(nanosuit_obj);

	}
	float currentFrame4 = glfwGetTime();
	std::cout << "3333:" << currentFrame4 - currentFrame3 << std::endl;

	gbuff = new Gbuff();
	gbuff->init();

	//initSphere();
	
	auto index = this->add_light(LT_DIR);
	DirLight *dl = (DirLight*)this->get_light(index);
	dl->direction = glm::vec3(-1.0f, -1.0f, -1.0f);
	dl->ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	dl->diffuse = glm::vec3(0.99f, 0.99f, 0.99f);
	dl->specular = glm::vec3(0.1f, 0.1f, 0.1f);
	shadowmap_mgr->set_cast_shadow_light(dl);
}


//debug
unsigned int quadVAO = 0;
unsigned int quadVBO;

void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


float fps_time = -1.0;
int fps = 0;
void debug_fps()
{
	float currentFrame = glfwGetTime();
	fps += 1;
	if (fps_time < 0)
	{
		fps_time = currentFrame;
	}

	if (currentFrame - fps_time >= 1.0)
	{
		std::cout << "fps :" << fps << std::endl;
		fps = 0;
		fps_time = currentFrame;
	}
}

void Worldbase::render(GLFWwindow *window)
{
	
	debug_fps();
	//render_forward();
	render_deferred();
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
	
}

void Worldbase::render_forward()
{
	// render
	// ------
	//
	
	shadowmap_mgr->render_depth_map();
	//glViewport(0, 0, GlobalVar::SCR_WIDTH, GlobalVar::SCR_HEIGHT);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	set_camera_info(plan_shader);
	set_light_info(plan_shader);
	
	//
	
	for (auto it = scene_entities.begin(); it != scene_entities.end(); ++it)
	{
		(*it)->render(plan_shader);
	}
	//

	//DEBUG
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//float near_plane = 1.0f, far_plane = 10.5f;
	//debugDepthQuad->use();
	//debugDepthQuad->setFloat("near_plane", near_plane);
	//debugDepthQuad->setFloat("far_plane", far_plane);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, shadowmap_mgr->depthMap);//shadowmap_mgr->depthMap
	//renderQuad();
}

void Worldbase::set_camera_info(Shader * shader)
{
	shader->use();
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)GlobalVar::SCR_WIDTH / (float)GlobalVar::SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	// set light uniforms
	shader->setVec3("viewPos", camera->Position);
}

void Worldbase::render_deferred()
{
	//
	shadowmap_mgr->render_depth_map();

	gbuff->use();
	// 延迟渲染的时候，改成0
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	set_camera_info(gbuff->gbuff_shader);

	glm::mat4 lightSpaceMatrix = GlobalVar::GAME_WORLD->shadowmap_mgr->getlightSpaceMatrix();
	gbuff->gbuff_shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	//

	for (auto it = scene_entities.begin(); it != scene_entities.end(); ++it)
	{
		(*it)->render(gbuff->gbuff_shader);
	}

	gbuff->unuse();
	//
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	set_light_info(gbuff->shaderLightingPass);
	gbuff->bind();
	GlobalVar::GAME_WORLD->shadowmap_mgr->bind_depth_map(gbuff->shaderLightingPass, 4);

	/*debugDepthQuad->use();
	debugDepthQuad->setInt("gPosition", 0);
	debugDepthQuad->setInt("gNormal", 1);
	debugDepthQuad->setInt("gAlbedoSpec", 2);
	debugDepthQuad->setInt("gPositionLightSpace", 3);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuff->gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gbuff->gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gbuff->gAlbedoSpec);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gbuff->gPositionLightSpace);*/

	renderQuad();


	

}

void Worldbase::processInput(GLFWwindow *window)
{
    // per-frame time logic
    // --------------------
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera->Position.y += camera->MovementSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera->Position.y -= camera->MovementSpeed * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		shadowmap_mgr->update_shadow_light_pos(0.00, 0.00, 0.01);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		shadowmap_mgr->update_shadow_light_pos(0.00, 0.00, -0.01);
}

void Worldbase::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera->ProcessMouseMovement(xoffset, yoffset);
}

void Worldbase::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(yoffset);
}

void Worldbase::add_scene_entity(GameObject * ent)
{
	scene_entities.push_back(ent);
}


int Worldbase::add_light(LightType light_type)
{
	int result = -1;
	switch (light_type)
	{
	case LT_DIR:
		{
			// return new 
			if (this->dirLights.size() >= NR_DIR_LIGHTS)
			{
				auto l = dirLights[0];
				dirLights.pop_back();
				delete l;
			}

			auto l = new DirLight();
			dirLights.push_back(l);
			result = dirLights.size() - 1;
		}
		break;

	case LT_POINT:
	{
		if (this->pointLights.size() >= NR_POINT_LIGHTS)
		{
			auto l = pointLights[0];
			pointLights.pop_back();
			delete l;
		}

		PointLight* pl = new PointLight();
		pointLights.push_back(pl);
		result = pointLights.size() - 1;
	}
		
		break;
	case LT_SPOT:
	{
		if (this->spotLights.size() >= NR_SPOT_LIGHTS)
		{
			auto l = spotLights[0];
			spotLights.pop_back();
			delete l;
		}

		auto spl = new SpotLight();
		spotLights.push_back(spl);
		result = spotLights.size() - 1;
	}
		
		break;

	default:
		break;
	}
	return result + int(light_type) * 1000;
} 

LightBase* Worldbase::get_light(int i)
{
	LightBase * lit = NULL;
	LightType lt = LightType(i / 1000);
	int index = i % 1000;
	
	switch (lt)
	{
	case LT_DIR:
		lit = dirLights.at(index);
		break;
	case LT_POINT:
		lit = pointLights.at(index);
		break;
	case LT_SPOT:
		lit = spotLights.at(index);
		break;
	default:
		break;
	}
	return lit;
}

void Worldbase::set_light_info(Shader* lightingShader)
{
	lightingShader->use();
	//BUG 忘记设置，导致高光一直表现不对
	lightingShader->setFloat("shininess", 32.0);
	lightingShader->setVec3("viewPos", camera->Position);
	if (dirLights.size() > 0)
	{
		auto l = dirLights[0];
		lightingShader->setVec3("dirLight.direction", l->direction);
		lightingShader->setVec3("dirLight.ambient", l->ambient);
		lightingShader->setVec3("dirLight.diffuse", l->diffuse);
		lightingShader->setVec3("dirLight.specular", l->specular);
	}
	int i = 0;
	for (auto it = pointLights.begin();it != pointLights.end(); ++it)
	{
		std::string s = "pointLights[";
		s += std::to_string(i);
		s += "]";
		auto l = pointLights[i];
		// point light 1
		lightingShader->setVec3(s + ".position", l->position);
		lightingShader->setVec3(s + ".ambient", l->ambient);
		lightingShader->setVec3(s + ".diffuse", l->diffuse);
		lightingShader->setVec3(s + ".specular", l->specular);
		lightingShader->setFloat(s + ".constant", l->constant);
		lightingShader->setFloat(s + ".linear", l->linear);
		lightingShader->setFloat(s + ".quadratic", l->quadratic);
		i++;
	}
	if (spotLights.size() > 0)
	{
		auto l = spotLights[0];
		// spotLight  ->
		lightingShader->setVec3("spotLight.position", camera->Position);
		lightingShader->setVec3("spotLight.direction", camera->Front);
		lightingShader->setVec3("spotLight.ambient", l->ambient);
		lightingShader->setVec3("spotLight.diffuse", l->diffuse);
		lightingShader->setVec3("spotLight.specular", l->specular);
		lightingShader->setFloat("spotLight.constant", l->constant);
		lightingShader->setFloat("spotLight.linear", l->linear);
		lightingShader->setFloat("spotLight.quadratic", l->quadratic);
		lightingShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(l->cutOff)));
		lightingShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(l->outerCutOff)));
	}
	
	lightingShader->setInt("ActiveDirLight", dirLights.size());
	lightingShader->setInt("ActivePointLight", pointLights.size());
	lightingShader->setInt("ActiveSpotLight", spotLights.size());

}
