#pragma once
#include <qwidget.h>
#include "Controller.h"
#include <qlistwidget.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qboxlayout.h>
#include <qformlayout.h>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>

class GUI : public QWidget
{
private:
	Controller& serv;
	WatchList* watchlist;

	QListWidget* movieListWidget;
	QListWidget* watchListWidget;
	QListWidget* genreListWidget;

	QLineEdit* titleEdit;
	QLineEdit* genreEdit;
	QLineEdit* yearEdit;
	QLineEdit* likesEdit;
	QLineEdit* trailerEdit;

	QLineEdit* titleDelete;

	QLineEdit* titleUpdate;
	QLineEdit* genreUpdate;
	QLineEdit* yearUpdate;
	QLineEdit* likesUpdate;
	QLineEdit* trailerUpdate;

	QLineEdit* genreFilter;

	QLineEdit* titleWatchDelete;

	QPushButton* addButton;
	QPushButton* deleteButton;
	QPushButton* updateButton;
	QPushButton* filterButton;
	QPushButton* nextButton;
	QPushButton* watchButton;
	QPushButton* addToWatchButton;
	QPushButton* OpenButton;
	QPushButton* deleteFromWatchButton;

	QPushButton* adminButton;
	QPushButton* userButton;
	QPushButton* StatisticButton;
	QPushButton* TableViewButton;

	QPushButton* CSVButton;
	QPushButton* HTMLButton;

	QPushButton* undoButton;
	QPushButton* redoButton;

	QWidget* window;

public:
	GUI(Controller& serv, WatchList* watchlist);

private:
	void buildAdminMenu();
	void buildUserMenu();

	void undoButtonHandler();
	void redoButtonHandler();
	
	void statisticView();

	void tableView();

	void initGUI();

	void populateList();
	void populateWatchList();
	
	void cycleThroughMovies();

	void nextButtonHandler();
	void openButtonHandler();
	void watchButtonHandler();
	void addToWatchButtonHandler();
	void deleteFromWatchButtonHandler();

	void addButtonHandler();
	void deleteButtonHandler();
	void updateButtonHandler();
};

