#include "CSVWatchList.h"
#include "util.h"
//#include "Movie.h"


CSVWatchList::CSVWatchList()
{
	this->loadFromFile();
}


CSVWatchList::~CSVWatchList()
{
}

void CSVWatchList::saveToFile()
{
	std::ofstream f("watchlist.csv");
	for (auto i : this->movieList)
		f << i << "\n";
	f.close();
}

void CSVWatchList::openInApp()
{
	saveToFile();
	system("notepad.exe watchlist.csv");
}

void CSVWatchList::loadFromFile()
{
	std::ifstream f("watchlist.csv");

	std::string temp;
	std::string* args;

	while (!f.eof())
	{
		std::getline(f, temp);
		if (f.eof() || temp == "")
		{
			f.close();
			break;
		}

		args = splitString(temp);
		this->movieList.push_back(Movie{ args[0], args[1], stoi(args[2]), stoi(args[3]), args[4] });
	}
	f.close();
}