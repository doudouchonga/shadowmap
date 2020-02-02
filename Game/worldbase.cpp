#include "camera.h"
#include "worldbase.h"
#include "global.h"
#include "shader.h"
#include "render_mesh.h"
#include "shadowmap_mgr.h"

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

void Worldbase::init()
{
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
    plan_mesh = new RenderMesh();
    plan_mesh->setup(sizeof(planeVertices), planeVertices, "resources/textures/wood.png");
	plan_mesh->set_cast_shadow(true);

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
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	for (unsigned int i = 0; i < 10; i++)
	{
		Shader *cube_shader = new Shader("resources/shader/multiple_lights_shadow.vs", "resources/shader/multiple_lights_shadow.fs");
		RenderMesh *cube_mesh = new RenderMesh();
		cube_mesh->set_shader(cube_shader);
		cube_mesh->setup(sizeof(vertices), vertices, "resources/textures/container2.png");
		cube_mesh->set_cast_shadow(true);
		cube_mesh->rote_enable = true;
		cube_mesh->rote_axis = glm::vec3(1.0f, 0.3f, 0.5f);
		cube_mesh->position = cubePositions[i];
		cube_mesh->angle = 20 * i;
		add_scene_entity(cube_mesh);
	}
    
	//light
	auto index = this->add_light(LT_POINT);
	PointLight* l = (PointLight*)this->get_light(index);
	l->position = glm::vec3(0.7f, 1.2f, 2.0f);//1.7f, 0.2f, 2.0f //-2.0f, 4.0f, -1.0f
	l->ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	l->diffuse = glm::vec3(0.99f, 0.99f, 0.99f);
	l->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	l->constant = 1.0f;
	l->linear = 0.09;
	l->quadratic = 0.032;
	shadowmap_mgr->set_cast_shadow_light(l);
	//
	/*auto dindex = this->add_light(LT_DIR);
	DirLight* dl = (DirLight*)this->get_light(dindex);
	dl->direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	dl->ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	dl->diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
	dl->specular = glm::vec3(0.5f, 0.5f, 0.5f);*/
	//
	glEnable(GL_DEPTH_TEST);
	shadowmap_mgr->init_depth_map();
	//
	debugDepthQuad = new Shader("resources/shader/debug_quad_depth.vs", "resources/shader/debug_quad_depth.fs");
	debugDepthQuad->use();
	debugDepthQuad->setInt("depthMap", 0);
	

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

void Worldbase::render(GLFWwindow *window)
{
    // render
    // ------
	//
	shadowmap_mgr->render_depth_map();
	//glViewport(0, 0, GlobalVar::SCR_WIDTH, GlobalVar::SCR_HEIGHT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	this->set_light_info(plan_shader);
	
    plan_mesh->render(camera, plan_shader);
    //
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	for (auto it = scene_entities.begin(); it!= scene_entities.end();++it)
	{
		set_light_info((*it)->render_shader);
		(*it)->render(camera);
	}
	
    //DEBUG
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//float near_plane = 1.0f, far_plane = 7.5f;
	//debugDepthQuad->use();
	//debugDepthQuad->setFloat("near_plane", near_plane);
	//debugDepthQuad->setFloat("far_plane", far_plane);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, shadowmap_mgr->depthMap);//shadowmap_mgr->depthMap
	//renderQuad();

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
	
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

void Worldbase::add_scene_entity(RenderMesh * ent)
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
