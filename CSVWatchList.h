#pragma once
#include "WatchList.h"
class CSVWatchList :
	public WatchList
{
private:
	void loadFromFile();

public:
	CSVWatchList();
	~CSVWatchList();

	void saveToFile();
	void openInApp();
};

