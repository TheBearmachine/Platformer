#pragma once

#include <string>

class ItemDatabase {
public:
	struct ItemStruct {
		std::string name, imageFilename;
		int ID, width, height, maxStack;
		std::string description;
	};

	static ItemDatabase& getInstance();

	ItemStruct* getItemInfo(int index);
	ItemStruct* getItemInfoByName(const std::string& name);

	ItemDatabase(const ItemDatabase&) = delete;
	ItemDatabase& operator=(const ItemDatabase&) = delete;
	~ItemDatabase();
private:
	ItemDatabase();
	static const int DATABASE_SIZE = 10;

	ItemStruct mItemDatabase[DATABASE_SIZE];
};