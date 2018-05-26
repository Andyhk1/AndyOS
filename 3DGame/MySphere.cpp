#include "MySphere.h"
#include "MeshComponent.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Model3D.h"
#include "ModelLoader.h"
#include "Engine.h"
#include "debug.h"

MySphere::MySphere()
{
	Model3D* model = ModelLoader::LoadModel("cube.a3d", Format3D::FORMAT_A3D);

	for (int i = 0; i < model->vertex_count; i++)
		model->vertices[i].color = COLOR_WHITE;

	MeshComponent* mesh = CreateComponent<MeshComponent>("Mesh");
	mesh->vertices = model->vertices;
	mesh->vertex_count = model->vertex_count;
	//mesh->shader = Shader(0.1f, 0.0f, 1.5f);

	Rigidbody* phys = CreateComponent<Rigidbody>("Rigidbody");
	phys->collider = new SphereCollider();
	phys->bEnabled = 0;
}

MySphere::~MySphere()
{
}
