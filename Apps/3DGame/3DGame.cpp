#include "3DGame.h"
#include "System.h"
#include "MyBox.h"
#include "MeshComponent.h"
#include "Model3D.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Thing.h"
#include "MySphere.h"
#include "BoxCollider.h"
#include "limits.h"

#include "../GL/GL.h"

void CreateCornell(Game* game)
{
	MyBox* walls[6];

	walls[0] = game->CreateObject<MyBox>("Floor");
	walls[0]->transform.position = Vector3(0, -1, 0);

	walls[1] = game->CreateObject<MyBox>("Roof");
	walls[1]->transform.position = Vector3(0, 1, 0);
	walls[1]->transform.rotation = Quaternion::FromEuler(Vector3(0, 0, M_PI));

	walls[2] = game->CreateObject<MyBox>("Wall-Left");
	walls[2]->transform.position = Vector3(-1, 0, 0);
	walls[2]->transform.rotation = Quaternion::FromEuler(Vector3(0, 0, -M_PI_2));
	
	walls[3] = game->CreateObject<MyBox>("Wall-Right");
	walls[3]->transform.position = Vector3(1, 0, 0);
	walls[3]->transform.rotation = Quaternion::FromEuler(Vector3(0, 0, M_PI_2));
	
	walls[4] = game->CreateObject<MyBox>("Wall-Back");
	walls[4]->transform.position = Vector3(0, 0, -1);
	walls[4]->transform.rotation = Quaternion::FromEuler(Vector3(M_PI_2, 0, 0));
	
	walls[5] = game->CreateObject<MyBox>("Wall-Front");
	walls[5]->transform.position = Vector3(0, 0, 1);
	walls[5]->transform.rotation = Quaternion::FromEuler(Vector3(-M_PI_2, 0, 0));

	ColRGB colors[] =
	{
		ColRGB(1, 1, 1),
		ColRGB(1, 1, 1),
		ColRGB(1, 0, 0),
		ColRGB(0, 1, 0),
		ColRGB(1, 1, 1),
		ColRGB(1, 1, 1)
	};

	for (int i = 0; i < 6; i++)
	{
		Vector3 scale(4, 4, 4);

		walls[i]->transform.position.x *= scale.x;
		walls[i]->transform.position.y *= scale.y;
		walls[i]->transform.position.z *= scale.z;

		walls[i]->transform.Scale(Vector3(4, 1, 4));

		MeshComponent* mesh = walls[i]->meshComponents[0];

		for (int j = 0; j < mesh->model->vertices.Count(); j++)
		{
			mesh->model->vertex_buffer[j].color = colors[i];
		}
	}
}

MyGame::MyGame()
{
	Camera* cam = CreateCamera<Camera>("Cam1");
	cam->transform.position = Vector3(0, 0, -8);

	//DirectionalLight* light = CreateLightSource<DirectionalLight>("Light");
	//light->transform.position = Vector3(0, 100, 0);
	//light->transform.rotation = Quaternion::LookAt(Vector3(), Vector3(0.3, -1, 0.5));

	PointLight* light = CreateLightSource<PointLight>("Light");
	light->transform.position = Vector3(0, 3, 0);
	light->intensity = 100;

	//Objects
	Thing* thing = CreateObject<Thing>("Thing");
	thing->transform.position = Vector3(-0.8, -0.6, 2);
	thing->transform.rotation = Quaternion::FromEuler(Vector3(0.1, 0.1, -M_PI / 8));
	thing->transform.scale = Vector3(0.05, 1.5, 0.05);

	MySphere* sphere = CreateObject<MySphere>("Sphere");
	sphere->transform.position = Vector3(-1, -1, 2);
	sphere->transform.scale = Vector3(0.7, 1, 0.7);

	MySphere* sphere2 = CreateObject<MySphere>("Sphere2");
	sphere2->transform.position = Vector3(1, -2, 2);
	sphere2->transform.scale = Vector3(1, 1, 1);
	sphere2->meshComponents[0]->shader = Shader(0, 0, FLT_MAX);

	CreateCornell(this);
}