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
#include <fstream>
#include <streambuf>
#include <string>
#include <iostream>

class Router{
private:
	const char * _url;
	int _responseCode;
	bool _isfile;
	std::string _filepath;
	std::string _contenttype;
	std::string _stringContent;
public:
	/// Constructor - creates a instance of the Router Object.
	///	@param {const char*} url: the URL in the request made to the server.
	/// @remarks:	this constructor will resolve the resources requested by the url thereby providing data via its members back to its consumers.
	Router(const char * url);

	/// GetContent - returns raw string output of the route.
	/// @returns {string}: raw string output of the route.
	/// @remarks: typically used if Router::IsFile() is false.
	std::string GetContent();

	/// IsFile - used to determine if the URL is directly requesting a file.
	/// @returns {bool}: true if the resource request is a file, false if resource requested is not a file.
	/// @remarks: if false, it is expected the content returned comes from Router::GetContent().
	bool IsFile();

	/// GetFilepath - Gets the location of the file relative to executable.
	/// @returns {const char*}: the filepath of the resource requested.
	/// @remarks: used by consumers of instances of this Router class to return files directly.
	const char* GetFilepath();

	/// GetContentType - gets the mimetype of the resource requested by the URL.
	/// @returns {const char*}: the mimetype (e.g "text/plain"), of the requested resource.
	const char * GetContentType();

	/// GetResponseCode - gets the HTTP status code of the resource requested.
	/// @returns {int}: 200 - OK, 404 - Not Found. (only these are currently supported, 500 Bad Request is on roadmap)
	/// @remarks: Refer to the HTTP 1.1 standard rfc2616 (https://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10)
	int GetResponseCode();
};
