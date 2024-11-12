#pragma once
#include "Repository.h"
#include "Movie.h"
#include "Action.h"

class ActionAdd : public Action
{
private:
	Repository* repo;
	Movie movie;

public:
	ActionAdd(Repository* repo, const Movie& movie) : repo{ repo }, movie{ movie } {}

	void executeUndo() override { this->repo->del(this->movie.getTitle()); }
	void executeRedo() override { this->repo->add(this->movie); }
};

