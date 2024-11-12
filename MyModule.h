#pragma once
#include <QabstractTableModel>
#include "Controller.h"

class MyModule : public QAbstractTableModel
{
	Q_OBJECT

private:
	WatchList* watchList;

public:
	explicit MyModule(WatchList* watchList, QObject* parent = NULL);

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
};

