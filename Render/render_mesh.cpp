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
	material.diffuse_tex = -1;
	material.specular_tex = -1;
	render_type = 0;
}

RenderMesh::~RenderMesh()
{

}

void RenderMesh::setup(int ver_size, float *Vertices,const std::string& diffuse_path, const std::string& specular_path)
{
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

void RenderMesh::setupExt(std::vector<float> data, std::vector<unsigned int> indices)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	float stride = (3 + 2 + 3) * sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

	draw_count = indices.size();
	render_type = 1;
}

void RenderMesh::render(Shader *shader)
{
	shader->use();
	shader->setInt("texture_diffuse1", 0);
	shader->setInt("texture_specular1", 1);

	
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

	GlobalVar::GAME_WORLD->shadowmap_mgr->bind_depth_map(shader, 2);

    glBindVertexArray(VAO);
	if (render_type == 0)
	{
		glDrawArrays(GL_TRIANGLES, 0, draw_count);
	}else if (render_type == 1)
	{
		glDrawElements(GL_TRIANGLE_STRIP, draw_count, GL_UNSIGNED_INT, 0);
	}
    
    glBindVertexArray(0);
}


