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
#include "router.h"
#include <string.h>
#include <iostream>

Router::Router(const char * url){
	_url = url;
	_isfile = false;
	if(0 == strcmp(_url, "/")) {
		_filepath = "public/index.html";
		_isfile = true;
		_responseCode = 200;
	} else if (0 == strncmp(_url, "/public", 7)) {
		_filepath = string(_url);
		// remove leading slash
		_filepath = _filepath.substr(1,_filepath.length() - 1);
		ifstream t(_filepath.c_str());
		if(t.good()){
			_responseCode = 200;
		} else {
			_filepath = "public/404.html";
			_responseCode = 404;
		}
		_isfile = true;
	} else {
		_filepath = "public/404.html";
		_responseCode = 404;
		_isfile = true;
	}

	// fall back to text/plain
	_contenttype = "text/plain";

	if(_isfile) { // Strict file extension returning
		// check for html
		size_t cut = _filepath.length() - 5;
		string ext = _filepath.substr(cut, 5);
		if(0 == ext.compare(".html")){ _contenttype = "text/html"; }
		if(0 == ext.compare(".json")){ _contenttype = "application/json"; }

		// check for css
		ext = ext.substr(1, 4);
		if(0 == ext.compare(".css")){ _contenttype = "text/css"; }
		if(0 == ext.compare(".png")){ _contenttype = "image/png"; }

		// check for js
		ext = ext.substr(1, 3);
		if(0 == ext.compare(".js")){ _contenttype = "text/javascript"; }
	}
}

string Router::GetContent(){
	string str;
	// retrieve content based on filepath.
	// maybe there is a need for an "isStream" boolean in which case a certain file extension could switch it.
	// Not currently used, but might be useful for a templating transform with a few a adjustments.
	if(_isfile) {
		ifstream t(_filepath.c_str());

		t.seekg(0, ios::end);
		str.reserve(t.tellg());
		t.seekg(0, ios::beg);

		str.assign((istreambuf_iterator<char>(t)),
					istreambuf_iterator<char>());
		return str;
	}
}

bool Router::IsFile() { return _isfile; }

const char* Router::GetFilepath() { return _filepath.c_str(); }

const char* Router::GetContentType(){ return _contenttype.c_str(); }

int Router::GetResponseCode() { return _responseCode; }
