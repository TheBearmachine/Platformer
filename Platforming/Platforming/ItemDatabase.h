#pragma once

#include <string>

class ItemDatabase {
public:
	struct ItemStruct {
		std::string name, imageFilename;
		int width, height, maxStack;
		std::string description;
	};

	static ItemDatabase& getInstance();

	ItemStruct getItemInfo(int index) const;

	ItemDatabase(const ItemDatabase&) = delete;
	ItemDatabase& operator=(const ItemDatabase&) = delete;
	~ItemDatabase();
private:
	ItemDatabase();

	ItemStruct mItemDatabase[10];
};