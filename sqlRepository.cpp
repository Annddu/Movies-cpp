#include "sqlRepository.h"

using namespace std;

sqlRepository::sqlRepository()
{
	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
		stmt = con->createStatement();
		stmt->execute("CREATE DATABASE IF NOT EXISTS " + this->database);
		con->setSchema(this->database);
		stmt->execute("CREATE TABLE IF NOT EXISTS movies (title VARCHAR(100), genre VARCHAR(100), year INT, likes INT, trailer VARCHAR(100))");
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	loadFromFile();
}

sqlRepository::~sqlRepository()
{
	//dtor
}

void sqlRepository::loadFromFile()
{
	try
	{
		res = stmt->executeQuery("SELECT * FROM movies");
		while (res->next())
		{
			string title = res->getString("title");
			string genre = res->getString("genre");
			int year = res->getInt("year");
			int likes = res->getInt("likes");
			string trailer = res->getString("trailer");
			Movie m{ title, genre, year, likes, trailer };
			add(m);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void sqlRepository::saveToFile()
{
	try
	{
		stmt->execute("DELETE FROM movies");
		for (auto m : this->items)
		{
			string title = m.getTitle();
			string genre = m.getGenre();
			int year = m.getYear();
			int likes = m.getLikes();
			string trailer = m.getTrailer();
			string query = "INSERT INTO movies (title, genre, year, likes, trailer) VALUES ('" + title + "', '" + genre + "', " + to_string(year) + ", " + to_string(likes) + ", '" + trailer + "')";
			stmt->execute(query);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

sqlRepository& sqlRepository::operator=(const sqlRepository& other)
{
	if (this == &other)
		return *this;
	this->items = other.items;
	return *this;
}