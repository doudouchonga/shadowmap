#include "render_mesh.h"
#include <glad/glad.h>
#include "game_texture.h"
#include "global.h"
#include "worldbase.h"
// #include <glm.hpp>
// #include <gtc/matrix_transform.hpp>
// #include <gtc/type_ptr.hpp>

RenderMesh::RenderMesh()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rote_axis = glm::vec3(0.0f, 0.0f, 0.0f);
	rote_enable = false;
	cast_shadow = false;
	material.diffuse_tex = -1;
	material.specular_tex = -1;
}

RenderMesh::~RenderMesh()
{

}

void RenderMesh::setup(int ver_size, float *Vertices,const std::string& diffuse_path, const std::string& specular_path)
{
	 // plane VAO
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, ver_size, Vertices, GL_STATIC_DRAW);
    //
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);
	this->material.diffuse_path = diffuse_path;
	this->material.specular_path = specular_path;
    this->material.diffuse_tex = GameTexture::load_tex(this->material.diffuse_path);
    this->material.specular_tex = GameTexture::load_tex(this->material.specular_path);

    draw_count = ver_size / 32;
}

void RenderMesh::set_shader(Shader* shader)
{
	render_shader = shader;
}

void RenderMesh::render(Camera *camera,Shader *shader, bool set_use)
{
	if (shader == NULL)
	{
		shader = render_shader;
	}
	if (set_use)
	{
		shader->use();
	}
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)GlobalVar::SCR_WIDTH / (float)GlobalVar::SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    // set light uniforms
    shader->setVec3("viewPos", camera->Position);
    shader->setInt("material.diffuse", 0);
    shader->setInt("material.specular", 1);
	shader->setFloat("material.shininess", 32.0f);
	if (this->material.diffuse_tex != GlobalVar::INVALID_TEX)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->material.diffuse_tex);
	}
	if (this->material.specular_tex != GlobalVar::INVALID_TEX)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->material.specular_tex);
	}
	if (GlobalVar::GAME_WORLD->shadowmap_mgr->depthMap != GlobalVar::INVALID_TEX)
	{
		shader->setInt("material.shadowMap", 2);
		shader->setInt("ActiveShadowMap", 1);

		glm::mat4 lightSpaceMatrix = GlobalVar::GAME_WORLD->shadowmap_mgr->getlightSpaceMatrix();
		shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, GlobalVar::GAME_WORLD->shadowmap_mgr->depthMap);
	}
    glm::mat4 model = getModeMatrix();
    shader->setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, draw_count);

    glBindVertexArray(0);
}

glm::mat4 RenderMesh::getModeMatrix()
{
	glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
	if (rote_enable)
	{
		model = glm::rotate(model, glm::radians(angle), rote_axis);
	}
    
    return model;
}

void RenderMesh::set_cast_shadow(bool c)
{
	GlobalVar::GAME_WORLD->shadowmap_mgr->set_cast_shadow_entites(this, c);

}
