#ifndef GAME_TEXTURE_H
#define GAME_TEXTURE_H
#include <string>

class GameTexture
{
public:
	GameTexture();
	~GameTexture();
	static unsigned int load(char const * path);
	static unsigned int load_tex(const std::string& path);
	static unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = true);
};

#endif