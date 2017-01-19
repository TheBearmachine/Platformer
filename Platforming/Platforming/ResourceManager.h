#pragma once

#include <vector>
#include <string>
#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

class ResourceManager{
public:
	static ResourceManager& getInstance();

	//Texture related functions
	sf::Texture& getTexture(const std::string& filename);
	sf::Texture& loadTexture(const std::string& filename, const sf::IntRect& rect = sf::IntRect());
	void clearTexture(const std::string& filename);
	void clearAllTextures();

	//Font related functions
	sf::Font& getFont(const std::string& filename);
	sf::Font& loadFont(const std::string& filename);
	void clearFont(const std::string& filename);
	void clearAllFonts();

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	~ResourceManager();

private:
	ResourceManager();
	typedef std::map<std::string, sf::Texture> TextureMap;
	TextureMap mTextures;
	typedef std::map<std::string, sf::Font> FontMap;
	FontMap mFonts;
};
