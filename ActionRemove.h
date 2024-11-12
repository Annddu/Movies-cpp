#pragma once
#include "Action.h"
#include "Repository.h"
#include "Movie.h"

class ActionRemove : public Action
{
private:
	Repository* repo;
	Movie movie;

public:
	ActionRemove(Repository* repo, const Movie& movie) : repo{ repo }, movie{ movie } {}

	void executeUndo() override { this->repo->add(this->movie); }
	void executeRedo() override { this->repo->del(this->movie.getTitle()); }
};

