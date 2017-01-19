#include "ResourceManager.h"

ResourceManager & ResourceManager::getInstance()
{
	static ResourceManager resourceManager;
	return resourceManager;
}

sf::Texture & ResourceManager::getTexture(const std::string& filename){
	if (mTextures.find(filename) == mTextures.end()) {
		printf("File not loaded: %s. Will load it for you.", filename);
		loadTexture(filename, sf::IntRect());
	}
	return mTextures[filename];
}

sf::Texture & ResourceManager::loadTexture(const std::string& filename, const sf::IntRect& rect){
	if (mTextures.find(filename) == mTextures.end()) {
		sf::Texture *texture = &mTextures[filename];
		bool textureLoaded = texture->loadFromFile(filename, rect);
		if (!textureLoaded)
			printf("Warning, Failed to find %s.",filename);
		texture->setSmooth(true);
	}
	return mTextures[filename];
}

void ResourceManager::clearTexture(const std::string& filename){
	mTextures.erase(filename);
}

void ResourceManager::clearAllTextures(){
	mTextures.clear();
}

sf::Font & ResourceManager::getFont(const std::string& filename){
	if (mFonts.find(filename) == mFonts.end()) {
		printf("File not loaded: %s. Will load it for you.", filename);
		loadFont(filename);
	}
	return mFonts[filename];
}

sf::Font & ResourceManager::loadFont(const std::string& filename){
	if (mFonts.find(filename) == mFonts.end()) {
		sf::Font* font = &mFonts[filename];
		bool fontLoaded = font->loadFromFile(filename);
		if (!fontLoaded)
			printf("Warning, Failed to find %s.", filename);
	}
	return mFonts[filename];
}

void ResourceManager::clearFont(const std::string& filename){
	mFonts.erase(filename);
}

void ResourceManager::clearAllFonts(){
	mFonts.clear();
}

ResourceManager::~ResourceManager(){
	clearAllTextures();
	clearAllFonts();
}

ResourceManager::ResourceManager(){

}
