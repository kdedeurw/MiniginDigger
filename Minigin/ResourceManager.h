#pragma once
#include "SingletonRef.h"
#include <map>

struct SDL_Texture;
class Font;
class ResourceManager final : public SingletonRef<ResourceManager>
{
public:
	~ResourceManager();

	void Init(const std::string& data);
	SDL_Texture* LoadTexture(const std::string& file);
	Font* LoadFont(const std::string& file, unsigned int size);
	const std::string& GetDataPath() const;

private:
	friend class SingletonRef<ResourceManager>;
	ResourceManager() = default;
	std::string m_DataPath;

	std::map<std::string, SDL_Texture*> m_Textures;
	std::map<std::string, Font*> m_Fonts;
};