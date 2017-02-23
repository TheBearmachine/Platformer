#include "ItemDatabase.h"
#include <fstream>
#include <sstream>
#include <string>

static const char* DB_FILE = "Resources/ItemDatabase/ItemDatabase.txt";
static const char* ITEM_IMAGES_PATH = "Resources/Images/Items/";

ItemDatabase::ItemDatabase() {
	std::ifstream infile(DB_FILE);
	std::string line, name, desc, imageFilename;
	char c;
	int width, height, maxStack;
	int index = 0;
	std::istringstream iss;
	while (std::getline(infile, line)) {
		iss.clear();
		iss.str(line);
		mItemDatabase[index].ID = index;
		iss >> name >> width >> height >> maxStack >> imageFilename;
		mItemDatabase[index].name = name;
		mItemDatabase[index].width = width;
		mItemDatabase[index].height = height;
		mItemDatabase[index].maxStack = maxStack;
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

ItemDatabase::ItemStruct* ItemDatabase::getItemInfo(int index) {
	if (index < DATABASE_SIZE)
		return &mItemDatabase[index];
	return nullptr;
}

ItemDatabase::ItemStruct* ItemDatabase::getItemInfoByName(const std::string & name)
{
	for (int i = 0; i < DATABASE_SIZE; i++)
		if (mItemDatabase[i].name == name) {
			return &mItemDatabase[i];
		}
	return nullptr;
}

ItemDatabase::~ItemDatabase() {

}
