#pragma once
#include "MemoryRepository.h"
#include "util.h"
#include "Movie.h"
#include <iostream>
#include <typeinfo>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "MemoryRepository.h"
#include "RepositoryException.h"

#include <stdlib.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

using namespace std;

class sqlRepository:
	public MemoryRepository
{
private:
	string server = "localhost:3306";
	string database = "movies";
	string username = "root";
	string password = "pkzs-2ubg-kd0o";

	sql::Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* res;

public:
	sqlRepository();
	~sqlRepository();

	sqlRepository& operator=(const sqlRepository& other);
	void saveToFile();
	void loadFromFile();
};

