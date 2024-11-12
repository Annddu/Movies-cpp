#pragma once
#include "WatchList.h"
class HTMLWatchList :
	public WatchList
{
private:
	void loadFromFile();
public:
	HTMLWatchList();
	~HTMLWatchList();

	void saveToFile();
	void openInApp();
};

