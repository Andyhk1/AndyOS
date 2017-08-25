#include "Box.h"
#include "MeshComponent.h"
#include "SphereCollider.h"
#include "Model3D.h"
#include "ModelLoader.h"

Box::Box()
{
	Model3D* mod = ModelLoader::LoadModel("plane.a3d", Format3D::FORMAT_A3D);

	char* img_buf;
	if (!ISO_FS::ReadFile("_img.bmp", img_buf))
	{
		Debug::Print("bmp not found");
		while (1);
	}
	BMP* bmp = new BMP(img_buf);
	Debug::Print("%i\n", bmp->width);

	//for (int y = 0; y < bmp->height; y++)
	//{
	//	for (int x = 0; x < bmp->width; x++)
	//	{
	//		uint32 pixel = bmp->pixels[y * bmp->width + x];
	//		VBE::SetPixel(x / 2, y / 2, pixel);
	//	}
	//}
	//VBE::Draw();
	//while (1);

	MeshComponent* mesh = CreateComponent<MeshComponent>("Mesh");
	mesh->vertices = mod->vertices;
	mesh->vertex_count = mod->vertex_count;
	mesh->tex_id = gl::GL::AddTexture(bmp);

	SphereCollider* sphere = CreateComponent<SphereCollider>("SphereCollider");
	sphere->radius = 1;
}
