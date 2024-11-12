#include "HTMLWatchList.h"
#include "util.h"
#include <Windows.h>
#include <shellapi.h>


HTMLWatchList::HTMLWatchList()
{
	loadFromFile();
}


HTMLWatchList::~HTMLWatchList()
{
}

void HTMLWatchList::saveToFile()
{
	std::ofstream f("watchlist.html");

	f << "<!DOCTYPE html>" << "\n";
	f << "<html>" << "\n";

	f << "<head>" << "\n";
	f << "<title>Movie Playlist</title>" << "\n";
	f << "</head>" << "\n";

	f << "<body>" << "\n";

	f << "<table border=\"1\">" << "\n";

	f << "<tr>" << "\n";
	f << "<td>Title</td>" << "\n";
	f << "<td>Genre</td>" << "\n";
	f << "<td>Year</td>" << "\n";
	f << "<td>Likes</td>" << "\n";
	f << "<td>Trailer</td>" << "\n";
	f << "</tr>" << "\n";

	for (auto i : this->movieList)
	{
		f << "<tr>" << "\n";

		f << "<td>" << i.getTitle() << "</td>" << "\n";
		f << "<td>" << i.getGenre() << "</td>" << "\n";
		f << "<td>" << i.getYear() << "</td>" << "\n";
		f << "<td>" << i.getLikes() << "</td>" << "\n";
		f << "<td><a href=" << i.getTrailer() << ">Link</a></td>";

		f << "</tr>" << "\n";
	}

	f << "</table>" << "\n";

	f << "</body>" << "\n";

	f << "</html>" << "\n";
	f.close();

}

void HTMLWatchList::openInApp()
{
	saveToFile();
	//ShellExecuteA(NULL, NULL, "chrome.exe", "watchlist.html", NULL, SW_SHOWMAXIMIZED);
	system("start watchlist.html");
	system("PAUSE");
}

void HTMLWatchList::loadFromFile() {
	std::ifstream inputFile("watchlist.html");
	if (!inputFile.is_open()) {
		std::cerr << "Error opening file." << std::endl;
		return;
	}

	std::string line;
	std::string delimiter = "<td>";
	std::string endDelimiter = "</td>";

	int year = 0 , likes = 0;
	std::string title = "", genre = "", trailer = "";

	while (std::getline(inputFile, line)) {
		size_t pos = 0;
		while ((pos = line.find(delimiter)) != std::string::npos) {
			line.erase(0, pos + delimiter.length());
			pos = line.find(endDelimiter);
			if (pos != std::string::npos) {
				std::string data = line.substr(0, pos);

				if (title == "" && data != "Title" && data != "Genre" && data != "Year" && data != "Likes" && data != "Trailer") {
					title = data;
				}
				else if (genre == "" && data != "Title" && data != "Genre" && data != "Year" && data != "Likes" && data != "Trailer") {
					genre = data;
				}
				else if (year == 0 && data != "Title" && data != "Genre" && data != "Year" && data != "Likes" && data != "Trailer") {
					std::cout << "Extracted year string: " << data << std::endl; // Debug output
					try {
						year = std::stoi(data);
					}
					catch (const std::exception& e) {
						std::cerr << "Error converting year: " << e.what() << std::endl;
					}
				}
				else if (likes == 0 && data != "Title" && data != "Genre" && data != "Year" && data != "Likes" && data != "Trailer") {
					likes = std::stoi(data);
				}
				else if (trailer == "" && data != "Title" && data != "Genre" && data != "Year" && data != "Likes" && data != "Trailer") {
					trailer = extractLink(data);
					//std::cout << extractLink(trailer) << std::endl;
					//std::cout << trailer << std::endl;
				}

				if (title != "" && genre != "" && year != 0 && likes != 0 && trailer != "") {
					this->movieList.push_back(Movie{ title, genre, year, likes, trailer });
					title = "";
					genre = "";
					year = 0;
					likes = 0;
					trailer = "";
				}

				line.erase(0, pos + endDelimiter.length());
			}
		}
	}

	inputFile.close();
}