#include "ImgLoader.h"
#include "stb_image.h"
using namespace std;

bool ImgLoader::IsValid()
{
	return isValid;
}

ImgLoader::ImgLoader(std::string img_path)
{
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(img_path.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
	isValid = true;
}

ImgLoader::~ImgLoader()
{
	free();
}

void ImgLoader::free()
{
	if (isValid)
	{
		stbi_image_free(data);
		isValid = false;
	}
}
