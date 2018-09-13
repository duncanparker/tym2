/*
	tym2 - A simple C++ framework, utilizing libmicrohttpd to serve simple web pages
	Copyright (C) 2018 Duncan Parker

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "pgdataprovider.h"

void PGDataProvider::SetConnectionString(std::string connectionString) {
	_connectionString = connectionString;
}

std::string PGDataProvider::GetConnectionString() {
	if(_connectionString.empty()) {
		return "";
	}
	std::string str(_connectionString);
	return str;
}

std::string PGDataProvider::GetMessage() {
	return _message;
}

// Prepare by default
std::string PGDataProvider::ExecuteQuery(bool dontprepare) {

}

bool PGDataProvider::Open() {
	const char* conninfo = _connectionString.c_str();
	_conn = PQconnectdb("hostaddr=127.0.0.1 user=tym2admin2 password=tym2password2 dbname=tym2");

	if (PQstatus(_conn) == CONNECTION_OK) {
		_connected = true;
		_message = "postgresql database connection succeeded";
	} else {
		_connected = false;
		_message = "Connection to database failed: ";
		std::string str(PQerrorMessage(_conn));
		_message.append(str);
	}
	return _connected;
}

void PGDataProvider::Close() {
	PQfinish(_conn);
}
