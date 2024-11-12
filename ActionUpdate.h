#pragma once
#include "Repository.h"
#include "Movie.h"
#include "Action.h"

class ActionUpdate : public Action
{
private:
	Repository* repo;
	Movie movie;
	Movie newMovie;

public:
	ActionUpdate(Repository* repo, const Movie& movie, const Movie& newMovie) : repo{ repo }, movie{ movie }, newMovie{ newMovie } {}

	void executeUndo() override { this->repo->update(movie); }
	void executeRedo() override { this->repo->update(newMovie); }
};

