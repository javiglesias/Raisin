#include "Texture.h"
#include "RenderAPI.h"

Texture::Texture(const char* _path)
{
	strcpy(mPath, _path);
}

void Texture::Setup()
{
	id = _CreateTextureFromFile(mPath, &width, &heigth, &nr_channels);

}
