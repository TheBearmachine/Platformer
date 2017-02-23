#include "ItemDatabase.h"
#include <fstream>
#include <sstream>

static const char* DB_FILE = "Resources/ItemDatabase/ItemDatabase.txt";
static const char* ITEM_IMAGES_PATH = "Resources/Images/Items/";

ItemDatabase::ItemDatabase() {
	std::ifstream ifStream(DB_FILE);
	std::string line, name, desc, imageFilename;
	int width, height, maxStack;
	int index = 0;
	while (std::getline(ifStream, line)) {
		std::istringstream iss(line);
		iss >> name;
		mItemDatabase[index].name = name;
		iss >> width;
		mItemDatabase[index].width = width;
		iss >> height;
		mItemDatabase[index].height = height;
		iss >> maxStack;
		mItemDatabase[index].maxStack = maxStack;
		iss >> imageFilename;
		mItemDatabase[index].imageFilename = ITEM_IMAGES_PATH + imageFilename;
		std::getline(iss, desc);
		mItemDatabase[index].description = desc;

		index++;
	}
}

ItemDatabase& ItemDatabase::getInstance() {
	static ItemDatabase iDB;
	return iDB;
}

ItemDatabase::ItemStruct ItemDatabase::getItemInfo(int index) const {
	return mItemDatabase[index];
}

ItemDatabase::~ItemDatabase() {

}
