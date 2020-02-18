#include "game_object.h"
#include "global.h"
#include "worldbase.h"


GameObject::GameObject()
{
	render_mesh = NULL;
	model_mesh = NULL;
	position = glm::vec3(0, 0, 0);
	rote_enable = false;
	scale = 1.0f;
}



glm::mat4 GameObject::getModeMatrix()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(scale));
	if (rote_enable)
	{
		model = glm::rotate(model, glm::radians(angle), rote_axis);
	}

	return model;
}

void GameObject::set_cast_shadow(bool c)
{
	GlobalVar::GAME_WORLD->shadowmap_mgr->set_cast_shadow_entites(this, c);

}

void GameObject::render(Shader * shader)
{

	glm::mat4 model = getModeMatrix();

	shader->setMat4("model", model);
	shader->setFloat("roughness", roughness);
	shader->setFloat("metallic", metallic);
	if (model_mesh != NULL)
	{
		model_mesh->Draw(shader);
	}
	else
	{
		render_mesh->render(shader);
	}
}

