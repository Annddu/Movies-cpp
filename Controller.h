#pragma once
#include "Repository.h"
#include "WatchList.h"
#include "Action.h"
#include "ActionAdd.h"
#include "ActionRemove.h"
#include "ActionUpdate.h"

class Controller
{
private:
	Repository* repo;
	std::vector<Action*> undoStack;
	std::vector<Action*> redoStack;

public:
	Controller();
	Controller(Repository* repo) { this->repo = repo; }
	~Controller();

	void undo();
	void redo();

	void add(const std::string& title, const std::string genre, const int& year, const int& likes, std::string trailer);
	int del(const std::string& title);
	int update(const std::string& title, const std::string genre, const int& year, const int& likes, std::string trailer);
	void incLikes(std::string title) { this->repo->incLikes(title); }
	void saveToFile() { this->repo->saveToFile(); }

	int getLength() { return this->repo->getLength(); }
	int getPosition(const std::string& title) { return this->repo->getPosition(title); }
	std::vector<Movie> getItems() { return this->repo->getItems(); }

};

