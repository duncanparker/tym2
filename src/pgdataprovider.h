/*
	tym2 - A simple C++ framework, utilizing libmicrohttpd to serve simple web pages
	Copyright (C) 2018 Duncan Parker

	This program is free software: you can redistribute it and/or modify
	it under the terms of the, GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <libpq-fe.h>
#include <string>

class PGDataProvider {
private:
	std::string _connectionString;
	bool _connected;
	std::string _message;
	PGconn * _conn;
public:
	void SetConnectionString(std::string connectionString);
	std::string GetConnectionString();
	std::string ExecuteQuery(bool prepare);
	std::string GetMessage();
	bool Open();
	void Close();
};
