#include "GUI.h"
#include "RepositoryException.h"
#include "MyModule.h"
#include <qlayout.h>
#include <qlabel.h>
#include <QMessageBox>
#include <QPainter>
#include <QFontMetrics>
#include <QWidget>
#include <QHBoxLayout>
#include <QPainter>
#include <QWidget>
#include <QHBoxLayout>
#include <algorithm>
#include <QRandomGenerator>
#include <QFontMetrics>
#include <QTableView>
#include <QHeaderView>
#include <QShortcut>
#include <QKeySequence>

GUI::GUI(Controller& serv, WatchList* watchList) : serv{ serv }, watchlist{ watchList }
{
	this->initGUI();

	QObject::connect(this->addButton, &QPushButton::clicked, this, &GUI::addButtonHandler);
	QObject::connect(this->deleteButton, &QPushButton::clicked, this, &GUI::deleteButtonHandler);
	QObject::connect(this->updateButton, &QPushButton::clicked, this, &GUI::updateButtonHandler);
	QObject::connect(this->filterButton, &QPushButton::clicked, this, &GUI::cycleThroughMovies);
	QObject::connect(this->nextButton, &QPushButton::clicked, this, &GUI::nextButtonHandler);
	QObject::connect(this->OpenButton, &QPushButton::clicked, this, &GUI::openButtonHandler);
	QObject::connect(this->watchButton, &QPushButton::clicked, this, &GUI::watchButtonHandler);
	QObject::connect(this->addToWatchButton, &QPushButton::clicked, this, &GUI::addToWatchButtonHandler);
	QObject::connect(this->deleteFromWatchButton, &QPushButton::clicked, this, &GUI::deleteFromWatchButtonHandler);
	
}

void GUI::statisticView() {
	// Create a dictionary with all the genres
	std::map<std::string, int> genres;
	for (auto s : this->serv.getItems()) {
		if (genres.find(s.getGenre()) == genres.end())
			genres[s.getGenre()] = 1;
		else
			genres[s.getGenre()]++;
	}

	// Calculate dimensions for the chart
	int barWidth = 22; // Width of each bar
	int spaceBetweenBars = 45; // Space between bars (adjusted for readability)
	int numRows = genres.size(); // Number of bars
	int maxCount = 0; // Maximum count to determine the scaling of the chart
	QFontMetrics fm(QFont("Verdana", 8)); // Font metrics for calculating text width
	for (const auto& genre : genres) {
		maxCount = std::max(maxCount, genre.second);
	}
	int maxLabelWidth = 0; // Maximum width of genre labels
	for (const auto& genre : genres) {
		int labelWidth = fm.horizontalAdvance(QString::fromStdString(genre.first));
		maxLabelWidth = std::max(maxLabelWidth, labelWidth);
	}
	int maxDigitWidth = fm.horizontalAdvance('9'); // Width of the widest digit
	int chartWidth = (barWidth + spaceBetweenBars) * numRows + maxLabelWidth + maxDigitWidth + 50; // Width of the chart (adjusted for labels and numbers)
	int chartHeight = maxCount * 44 + 50; // Height of the chart (scaled based on max count) + space for labels

	// Create a widget to hold the chart
	QWidget* window = new QWidget{};
	QHBoxLayout* mainLayout = new QHBoxLayout{ window };

	// Create a pixmap to draw the chart
	QPixmap pixmap(chartWidth, chartHeight);
	pixmap.fill(Qt::white);
	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing);

	// Set up font for genre labels
	QFont font("Verdana", 8);
	painter.setFont(font);

	// Draw genre labels
	int labelHeight = fm.height(); // Height of the label text
	int x = maxLabelWidth + 25; // Starting position for the first bar
	for (const auto& genre : genres) {
		// Calculate the center position of the current bar
		int barCenter = x + barWidth / 2;

		// Calculate the starting position for the text to center it with the bar
		int textX = barCenter - fm.horizontalAdvance(QString::fromStdString(genre.first)) / 2;
		int textY = chartHeight - 6;

		// Draw the genre label at the adjusted position
		painter.drawText(textX, textY, QString::fromStdString(genre.first));

		// Move to the next bar
		x += barWidth + spaceBetweenBars;
	}

	// Set up font for count numbers
	QFont countFont("Verdana", 8);
	painter.setFont(countFont);

	// Set pen color to blue for count numbers
	QPen blackPen(Qt::black);
	painter.setPen(blackPen);

	// Draw count numbers on the left side of the chart
	x = maxLabelWidth - 10;
	for (int i = 0; i <= maxCount + 4; i++) {
		// Draw the count number
		painter.drawText(x, chartHeight - 20 - i * 44, QString::number(i));

		// Draw dotted lines to the right of the numbers
		QPen dottedPen(Qt::black);
		dottedPen.setStyle(Qt::DotLine);
		painter.setPen(dottedPen);
		painter.drawLine(x + maxDigitWidth + 2, chartHeight - 20 - i * 44, chartWidth - 10, chartHeight - 20 - i * 44);
	}

	// Draw bars
	x = maxLabelWidth + 35;
	for (const auto& genre : genres) {
		// Generate a random color for the bar
		QColor randomColor = QColor::fromRgb(
			QRandomGenerator::global()->bounded(256),
			QRandomGenerator::global()->bounded(256),
			QRandomGenerator::global()->bounded(256)
		);

		// Set up pen with the random color for drawing the bar
		QPen pen(randomColor);
		pen.setWidth(barWidth);
		painter.setPen(pen);

		// Calculate bar height
		float barHeight = genre.second * 44 - 12;

		// Draw the bar
		painter.drawLine(x, chartHeight - 30, x, chartHeight - 20 - barHeight);

		// Update x position for the next bar
		x += barWidth + spaceBetweenBars;
	}

	painter.end(); // End painting on the pixmap

	// Create a label to display the pixmap
	QLabel* label = new QLabel(window);
	label->setPixmap(pixmap);

	mainLayout->addWidget(label);
	window->setLayout(mainLayout);

	window->show();
}


void GUI::initGUI()
{
	QHBoxLayout* mainLayout = new QHBoxLayout{ this };
	this->adminButton = new QPushButton{ "Admin" };
	this->userButton = new QPushButton{ "User" };
	this->StatisticButton = new QPushButton{ "Genre Statistic" };
	this->TableViewButton = new QPushButton{ "Table View" };
	mainLayout->addWidget(adminButton);
	mainLayout->addWidget(userButton);
	mainLayout->addWidget(StatisticButton);	
	mainLayout->addWidget(TableViewButton);

	QObject::connect(this->adminButton, &QPushButton::clicked, this, [&]() {
		close();
		this->buildAdminMenu(); 
		this->populateList();	
		});
	QObject::connect(this->userButton, &QPushButton::clicked, [&]() {
		close();
		this->buildUserMenu(); 
		this->populateWatchList(); 
		});
	QObject::connect(this->StatisticButton, &QPushButton::clicked, this, [&]() {
		close();
		this->statisticView();
		});
	QObject::connect(this->TableViewButton, &QPushButton::clicked, this, [&]() {
		close();
		this->tableView();
		});
}

void GUI::tableView()
{
	// Create a model for the table
	MyModule* model = new MyModule(this->watchlist);
	//model->setWatchList(this->watchlist);

	// Create a table view with the model
	QTableView* view = new QTableView{};
	view->setModel(model);

	// Set up the table view
	view->setSelectionBehavior(QAbstractItemView::SelectRows);
	view->setSelectionMode(QAbstractItemView::SingleSelection);
	view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	// Create a widget to hold the table view
	QWidget* window = new QWidget{};
	QHBoxLayout* mainLayout = new QHBoxLayout{ window };
	mainLayout->addWidget(view);
	window->setLayout(mainLayout);

	window->show();
}

void GUI::buildUserMenu()
{
	window = new QWidget{};
	QHBoxLayout* mainLayout = new QHBoxLayout{ this };
	QPushButton* backButton = new QPushButton{ "Back" };
	mainLayout->addWidget(backButton);
	QObject::connect(backButton, &QPushButton::clicked, [&]() {
		window->close();
		this->initGUI();
		});
	
	this->watchListWidget = new QListWidget{};

	this->filterButton = new QPushButton{ "Filter by genre:" };
	this->nextButton = new QPushButton{ "Next" };
	this->watchButton = new QPushButton{ "Watch" };
	this->addToWatchButton = new QPushButton{ "Add to watchlist" };
	this->OpenButton = new QPushButton{ "Open" };
	this->deleteFromWatchButton = new QPushButton{ "Delete from watchlist:" };

	QObject::connect(this->filterButton, &QPushButton::clicked, this, &GUI::cycleThroughMovies);
	QObject::connect(this->nextButton, &QPushButton::clicked, this, &GUI::nextButtonHandler);
	QObject::connect(this->watchButton, &QPushButton::clicked, this, &GUI::watchButtonHandler);
	QObject::connect(this->addToWatchButton, &QPushButton::clicked, this, &GUI::addToWatchButtonHandler);
	QObject::connect(this->OpenButton, &QPushButton::clicked, this, &GUI::openButtonHandler);
	QObject::connect(this->deleteFromWatchButton, &QPushButton::clicked, this, &GUI::deleteFromWatchButtonHandler);


	QGridLayout* cycleLayout = new QGridLayout{};
	this->genreFilter = new QLineEdit{};

	this->titleWatchDelete = new QLineEdit{};
	cycleLayout->addWidget(this->titleWatchDelete, 1, 1);
	cycleLayout->addWidget(this->deleteFromWatchButton, 1, 0);

	cycleLayout->addWidget(this->genreFilter, 0, 1);
	cycleLayout->addWidget(filterButton, 0, 0);
	cycleLayout->addWidget(watchButton, 4, 1);
	cycleLayout->addWidget(nextButton, 2, 1);
	cycleLayout->addWidget(addToWatchButton, 3, 1);
	cycleLayout->addWidget(OpenButton, 5, 1);
	
	//crete a box to print a movie
	this->genreListWidget = new QListWidget{};
	//this->genreListWidget->setFixedWidth(550);
	cycleLayout->addWidget(genreListWidget, 6, 0, 1, 2);


	mainLayout->addWidget(watchListWidget);
	mainLayout->addLayout(cycleLayout);

	this->watchlist->getSuggestions(this->serv.getItems(), "");

	Movie m = this->watchlist->getCurrentMovie();
	QListWidgetItem* item = new QListWidgetItem{ QString::fromStdString(m.getGenre() + ": " + m.getTitle() + "; " + std::to_string(m.getYear()) + "; " + std::to_string(m.getLikes()) + " likes; " + m.getTrailer()) };
	QFont font{ "Verdana", 10, 10 };
	item->setFont(font);
	this->genreListWidget->addItem(item);
	
	window->setLayout(mainLayout);
	window->show();
}

void GUI::addToWatchButtonHandler()
{
	this->watchlist->add();
	this->genreListWidget->clear();
	Movie m = this->watchlist->getCurrentMovie();
	QListWidgetItem* item = new QListWidgetItem{ QString::fromStdString(m.getGenre() + ": " + m.getTitle() + "; " + std::to_string(m.getYear()) + "; " + std::to_string(m.getLikes()) + " likes; " + m.getTrailer()) };
	QFont font{ "Verdana", 10, 10 };
	item->setFont(font);
	this->genreListWidget->addItem(item);
	this->populateWatchList();
	this->watchlist->saveToFile();
}

void GUI::watchButtonHandler()
{
	std::string completeUrl = this->watchlist->getCurrentMovie().getTrailer();
	system(std::string("start " + completeUrl).c_str());
}

void GUI::nextButtonHandler()
{
	this->watchlist->nextPos();
	this->genreListWidget->clear();
	Movie m = this->watchlist->getCurrentMovie();
	QListWidgetItem* item = new QListWidgetItem{ QString::fromStdString(m.getGenre() + ": " + m.getTitle() + "; " + std::to_string(m.getYear()) + "; " + std::to_string(m.getLikes()) + " likes; " + m.getTrailer()) };
	QFont font{ "Verdana", 10, 10 };
	item->setFont(font);
	this->genreListWidget->addItem(item);
}

void GUI::openButtonHandler()
{
	this->watchlist->openInApp();
}

void GUI::deleteFromWatchButtonHandler()
{
	QString title = this->titleWatchDelete->text();
	if (title.toStdString() == "")
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Title cannot be empty");
		return;
	}
	this->watchlist->del(title.toStdString());
	this->populateWatchList();
	this->watchlist->saveToFile();
}

void GUI::cycleThroughMovies()
{
	this->genreListWidget->clear();
	QString genre = this->genreFilter->text();
	this->watchlist->getSuggestions(this->serv.getItems(), genre.toStdString());

	if (this->watchlist->getMaximumPos() == 0)
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "No suggestions for this genre, sorry!");
		return;
	}

	Movie m = this->watchlist->getCurrentMovie();
	QListWidgetItem* item = new QListWidgetItem{ QString::fromStdString(m.getGenre() + ": " + m.getTitle() + "; " + std::to_string(m.getYear()) + "; " + std::to_string(m.getLikes()) + " likes; " + m.getTrailer()) };
	QFont font{ "Verdana", 10, 10 };
	item->setFont(font);
	this->genreListWidget->addItem(item);
}

void GUI::populateWatchList()
{
	this->watchListWidget->clear();


	for (auto s : this->watchlist->getList())
	{
		//this->watchListWidget->addItem(QString::fromStdString(s.getGenre() + " " + s.getTitle() + " " + std::to_string(s.getYear()) + " " + s.getTrailer()));
		QListWidgetItem* item = new QListWidgetItem{ QString::fromStdString(s.getGenre() + ": " + s.getTitle() + "; " + std::to_string(s.getYear()) + "; " + std::to_string(s.getLikes()) + " likes; " + s.getTrailer()) };
		QFont font{ "Verdana", 10, 10 };
		item->setFont(font);
		this->watchListWidget->addItem(item);
	}
}

void GUI::undoButtonHandler()
{
	this->serv.undo();
	this->populateList();
}

void GUI::redoButtonHandler()
{
	this->serv.redo();
	this->populateList();
}

void GUI::buildAdminMenu()
{
	//window->close();
	window = new QWidget{};
	QHBoxLayout* mainLayout = new QHBoxLayout{ this };
	QPushButton* backButton = new QPushButton{ "Back" };
	QPushButton* undoButton = new QPushButton{ "Undo" };
	QPushButton* redoButton = new QPushButton{ "Redo" };
	mainLayout->addWidget(backButton);
	mainLayout->addWidget(undoButton);
	mainLayout->addWidget(redoButton);
	QObject::connect(backButton, &QPushButton::clicked, [&]() {
		window->close();
		});
	QObject::connect(undoButton, &QPushButton::clicked, [&]() {
		this->undoButtonHandler();
		});
	QObject::connect(redoButton, &QPushButton::clicked, [&]() {
		this->redoButtonHandler();
		});

	// Create QShortcut objects for undo and redo
	QShortcut* undoShortcut = new QShortcut(QKeySequence::Undo, window);
	QShortcut* redoShortcut = new QShortcut(QKeySequence::Redo, window);

	// Connect shortcuts to handlers
	QObject::connect(undoShortcut, &QShortcut::activated, [&]() {
		this->undoButtonHandler();
		});
	QObject::connect(redoShortcut, &QShortcut::activated, [&]() {
		this->redoButtonHandler();
		});

	this->movieListWidget = new QListWidget{};
	mainLayout->addWidget(movieListWidget);
	Qt::Alignment aligntop = Qt::AlignTop;
	Qt::Alignment alignbottom = Qt::AlignBottom;
	this->addButton = new QPushButton{ "Add" };
	this->deleteButton = new QPushButton{ "Delete" };
	this->updateButton = new QPushButton{ "Update" };

	QObject::connect(this->addButton, &QPushButton::clicked, this, &GUI::addButtonHandler);
	QObject::connect(this->deleteButton, &QPushButton::clicked, this, &GUI::deleteButtonHandler);
	QObject::connect(this->updateButton, &QPushButton::clicked, this, &GUI::updateButtonHandler);

	QGridLayout* rightLayout = new QGridLayout{};

	QLabel* titleLabel = new QLabel{ "Title:" };
	this->titleEdit = new QLineEdit{};
	QLabel* genreLabel = new QLabel{ "Genre:" };
	this->genreEdit = new QLineEdit{};
	QLabel* yearLabel = new QLabel{ "Year:" };
	this->yearEdit = new QLineEdit{};
	QLabel* likesLabel = new QLabel{ "Likes:" };
	this->likesEdit = new QLineEdit{};
	QLabel* trailerLabel = new QLabel{ "Trailer:" };
	this->trailerEdit = new QLineEdit{};


	rightLayout->addWidget(titleLabel, 0, 0, aligntop);
	rightLayout->addWidget(this->titleEdit, 0, 1, aligntop);
	rightLayout->addWidget(genreLabel, 1, 0, aligntop);
	rightLayout->addWidget(this->genreEdit, 1, 1, aligntop);
	rightLayout->addWidget(yearLabel, 2, 0, aligntop);
	rightLayout->addWidget(this->yearEdit, 2, 1, aligntop);
	rightLayout->addWidget(likesLabel, 3, 0, aligntop);
	rightLayout->addWidget(this->likesEdit, 3, 1, aligntop);
	rightLayout->addWidget(trailerLabel, 4, 0, aligntop);
	rightLayout->addWidget(this->trailerEdit, 4, 1, aligntop);
	rightLayout->addWidget(this->addButton, 5, 1.5, alignbottom);

	QGridLayout* deleteLayout = new QGridLayout{};
	QLabel* titleDeleteLabel = new QLabel{ "Title:" };
	this->titleDelete = new QLineEdit{};
	deleteLayout->addWidget(titleDeleteLabel, 0, 0, aligntop);
	deleteLayout->addWidget(this->titleDelete, 0, 1, aligntop);
	deleteLayout->addWidget(this->deleteButton, 1, 1.5, alignbottom);

	QGridLayout* updateLayout = new QGridLayout{};
	QLabel* titleUpdateLabel = new QLabel{ "Title:" };
	this->titleUpdate = new QLineEdit{};
	QLabel* genreUpdateLabel = new QLabel{ "Genre:" };
	this->genreUpdate = new QLineEdit{};
	QLabel* yearUpdateLabel = new QLabel{ "Year:" };
	this->yearUpdate = new QLineEdit{};
	QLabel* likesUpdateLabel = new QLabel{ "Likes:" };
	this->likesUpdate = new QLineEdit{};
	QLabel* trailerUpdateLabel = new QLabel{ "Trailer:" };
	this->trailerUpdate = new QLineEdit{};
	updateLayout->addWidget(titleUpdateLabel, 0, 0, aligntop);
	updateLayout->addWidget(this->titleUpdate, 0, 1, aligntop);
	updateLayout->addWidget(genreUpdateLabel, 1, 0, aligntop);
	updateLayout->addWidget(this->genreUpdate, 1, 1, aligntop);
	updateLayout->addWidget(yearUpdateLabel, 2, 0, aligntop);
	updateLayout->addWidget(this->yearUpdate, 2, 1, aligntop);
	updateLayout->addWidget(likesUpdateLabel, 3, 0, aligntop);
	updateLayout->addWidget(this->likesUpdate, 3, 1, aligntop);
	updateLayout->addWidget(trailerUpdateLabel, 4, 0, aligntop);
	updateLayout->addWidget(this->trailerUpdate, 4, 1, aligntop);
	updateLayout->addWidget(this->updateButton, 5, 1.5, alignbottom);



	mainLayout->addLayout(rightLayout);
	mainLayout->addLayout(deleteLayout);
	mainLayout->addLayout(updateLayout);

	window->setLayout(mainLayout);
	window->show();
}

void GUI::populateList()
{
	this->movieListWidget->clear();

	for (auto s : this->serv.getItems())
	{
		//this->movieListWidget->addItem(QString::fromStdString(s.getGenre() + " " + s.getTitle() + " " + std::to_string(s.getYear()) + " " + s.getTrailer()));

		QListWidgetItem* item = new QListWidgetItem{ QString::fromStdString(s.getGenre() + ": " + s.getTitle() + "; " + std::to_string(s.getYear()) + "; " + std::to_string(s.getLikes()) + " likes; " + s.getTrailer()) };
		QFont font{ "Verdana", 10, 10 };
		item->setFont(font);
		this->movieListWidget->addItem(item);
	}
}

void GUI::addButtonHandler()
{
	QString title = this->titleEdit->text();
	QString genre = this->genreEdit->text();
	QString year = this->yearEdit->text();
	QString likes = this->likesEdit->text();
	QString trailer = this->trailerEdit->text();

	if (title.toStdString() == "")
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Title cannot be empty");
		return;
	}
	if (genre.toStdString() == "")
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Genre cannot be empty");
		return;
	}
	if (year.toStdString() == "")
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Year cannot be empty");
		return;
	}
	//verify if year is a number
	for (auto c : year.toStdString())
		if (!isdigit(c))
		{
			QMessageBox messageBox;
			messageBox.critical(0, "Error", "Year must be a number");
			return;
		}
	if (likes.toStdString() == "")
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Likes cannot be empty");
		return;
	}
	//verify if likes is a number
	for (auto c : likes.toStdString())
		if (!isdigit(c))
		{
			QMessageBox messageBox;
			messageBox.critical(0, "Error", "Likes must be a number");
			return;
		}
	if (trailer.toStdString() == "")
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Trailer cannot be empty");
		return;
	}

	//this->serv.add(title.toStdString(), genre.toStdString(), year.toInt(), likes.toInt(), trailer.toStdString());
	try
	{
		this->serv.add(title.toStdString(), genre.toStdString(), year.toInt(), likes.toInt(), trailer.toStdString());
	}
	catch (MovieException& e)
	{
		for (auto s : e.getErrors())
		{
			QMessageBox messageBox;
			messageBox.critical(0, "Error", QString::fromStdString(s));
		}
	}
	catch (RepositoryException& e)
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", QString::fromStdString(e.what()));
	}
	catch (DuplicateMovieException& e)
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", QString::fromStdString(e.what()));
	}
	catch (FileException& e)
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", QString::fromStdString(e.what()));
	}
	catch (std::invalid_argument e)
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", QString::fromStdString(e.what()));
	}
	
	this->serv.saveToFile();
	this->populateList();
}

void GUI::deleteButtonHandler()
{
	QString title = this->titleDelete->text();
	if (title.toStdString() == "")
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Title cannot be empty");
		return;
	}
	this->serv.del(title.toStdString());
	this->serv.saveToFile();
	this->populateList();
}

void GUI::updateButtonHandler()
{
	QString title = this->titleUpdate->text();
	QString genre = this->genreUpdate->text();
	QString year = this->yearUpdate->text();
	QString likes = this->likesUpdate->text();
	QString trailer = this->trailerUpdate->text();

	if (title.toStdString() == "")
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Title cannot be empty");
		return;
	}
	if (genre.toStdString() == "")
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Genre cannot be empty");
		return;
	}
	if (year.toStdString() == "")
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Year cannot be empty");
		return;
	}
	//verify if year is a number
	for (auto c : year.toStdString())
		if (!isdigit(c))
		{
			QMessageBox messageBox;
			messageBox.critical(0, "Error", "Year must be a number");
			return;
		}
	if (likes.toStdString() == "")
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Likes cannot be empty");
		return;
	}
	//verify if likes is a number
	for (auto c : likes.toStdString())
		if (!isdigit(c))
		{
			QMessageBox messageBox;
			messageBox.critical(0, "Error", "Likes must be a number");
			return;
		}
	if (trailer.toStdString() == "")
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Trailer cannot be empty");
		return;
	}

	this->serv.update(title.toStdString(), genre.toStdString(), year.toInt(), likes.toInt(), trailer.toStdString());
	this->serv.saveToFile();
	this->populateList();
}