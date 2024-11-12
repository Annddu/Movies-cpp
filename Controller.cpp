#include "Controller.h"



Controller::Controller()
{
}


Controller::~Controller()
{
}

void Controller::add(const std::string& title, const std::string genre, const int& year, const int& likes, std::string trailer)
{
	Movie m{ title, genre, year, likes, trailer };
	this->repo->add(m);
	Action* action = new ActionAdd{ this->repo, m };
	this->undoStack.push_back(action);
	for (auto action : this->redoStack)
		delete action;
	this->redoStack.clear();
}

int Controller::del(const std::string& title)
{
	if (this->repo->getPosition(title) == -1)
		return 0;
	Movie m = this->repo->getMovie(title);
	Action* action = new ActionRemove{ this->repo, m };
	this->undoStack.push_back(action);
	for (auto action : this->redoStack)
		delete action;
	this->redoStack.clear();
	return this->repo->del(title);
}

int Controller::update(const std::string& title, const std::string genre, const int& year, const int& likes, std::string trailer)
{
	if (this->repo->getPosition(title) == -1)
		return 0;
	Movie oldm = this->repo->getMovie(title);
	Movie m{ title, genre, year, likes, trailer };
	Action* action = new ActionUpdate{ this->repo, oldm, m };
	this->undoStack.push_back(action);
	for (auto action : this->redoStack)
		delete action;
	this->redoStack.clear();
	return this->repo->update(Movie{ title, genre, year, likes, trailer });
}

void Controller::undo()
{
	if (this->undoStack.empty())
	{
		return;
	}

	Action* action = this->undoStack.back();
	this->undoStack.pop_back();
	action->executeUndo();
	this->redoStack.push_back(action);
}

void Controller::redo()
{
	if (this->redoStack.empty())
	{
		return;
	}
	Action* action = this->redoStack.back();
	this->redoStack.pop_back();
	action->executeRedo();
	this->undoStack.push_back(action);
}