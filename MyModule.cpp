#include "MyModule.h"

MyModule::MyModule(WatchList* watchList, QObject* parent)
	: QAbstractTableModel(parent)
{
	this->watchList = watchList;
}

int MyModule::rowCount(const QModelIndex& parent) const
{
	return this->watchList->getList().size();
}

int MyModule::columnCount(const QModelIndex& parent) const
{
	return 5;
}

QVariant MyModule::data(const QModelIndex& index, int role) const
{
	int row = index.row();
	int column = index.column();

	std::vector<Movie> movies = this->watchList->getList();
	Movie currentMovie = movies[row];

	if (role == Qt::DisplayRole)
	{
		switch (column)
		{
		case 0:
			return QString::fromStdString(currentMovie.getTitle());
		case 1:
			return QString::fromStdString(currentMovie.getGenre());
		case 2:
			return QString::number(currentMovie.getYear());
		case 3:
			return QString::number(currentMovie.getLikes());
		case 4:
			return QString::fromStdString(currentMovie.getTrailer());
		default:
			break;
		}
	}

	return QVariant();
}