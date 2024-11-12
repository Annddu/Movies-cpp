#include "GUI.h"
#include <QtWidgets/QApplication>

#include "Console.h"
#include "CSVWatchList.h"
#include "HTMLWatchList.h"
#include "FileRepository.h"
#include "sqlRepository.h"
#include "Tests.h"
#include <Windows.h>
#include <conio.h>

int main(int argc, char *argv[])
{/*
    QApplication a(argc, argv);
	MemoryRepository* repo = new FileRepository{ "movie_database.csv" };
	WatchList* watchlist = new CSVWatchList();
    Controller serv{ repo };
	GUI gui{ serv, watchlist };
    gui.show();
    return a.exec();*/
    QApplication a(argc, argv);

    QWidget* main = new QWidget{};
    QVBoxLayout* vL = new QVBoxLayout(main);
    QPushButton* btnCSV = new QPushButton("CSV", main);
    QPushButton* btnHTML = new QPushButton("HTML", main);
    vL->addWidget(btnCSV);
    vL->addWidget(btnHTML);
    main->show();
    main->setWindowTitle("Choose Repository Type");
    //main->setFixedHeight(main->sizeHint().height());
    //main->setFixedWidth(main->sizeHint().width());
	MemoryRepository* repo = nullptr;
	Controller* service = nullptr;
	WatchList* html = nullptr;
	WatchList* csv = nullptr;
    GUI* gui = nullptr;

    // Connect signals to slots to create the Service and GUI instances
    QObject::connect(btnCSV, &QPushButton::clicked, [&]() {
		repo = new FileRepository{ "movie_database.csv" };
		csv = new CSVWatchList();
		service = new Controller{ repo };

		gui = new GUI{ *service, csv }; // Pass by reference or pointer based on your implementation
        gui->show();
        });

    QObject::connect(btnHTML, &QPushButton::clicked, [&]() {
        repo = new FileRepository{ "movie_database.csv" };
		html = new HTMLWatchList();
        service = new Controller{ repo };

        gui = new GUI{ *service, html }; // Pass by reference or pointer based on your implementation
        gui->show();
        });

    return a.exec();
}