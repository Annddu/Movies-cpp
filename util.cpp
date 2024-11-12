#include "util.h"
#include <sstream>
#include <algorithm>

std::string* splitString(std::string temp)
{
	std::string* args = new std::string[5];
	std::istringstream ss(temp);
	std::string token;
	int i = 0;

	while (std::getline(ss, token, ','))
		args[i++] = token;

	return args;
}

std::string extractLink(const std::string& html) {
	size_t start_pos = html.find("href=") + 5;  // Adding the length of "href="
	size_t end_pos = html.find("\"", start_pos);
	std::string link = html.substr(start_pos, end_pos - start_pos);

	// Trim the trailing ">Link</a>" characters
	size_t trim_pos = link.find(">Link</a>");
	if (trim_pos != std::string::npos) {
		link = link.substr(0, trim_pos);
	}

	return link;
}