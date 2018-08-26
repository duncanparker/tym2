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
	along with this program.  If not, see <https://www.gnu.org/licenses/>

	--------------------------------------------------------------------------------
	Libmicrohttpd is a dependency of tym2.

	Libmicrohttpd is licensed under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	Libmicrohttpd is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with your Libmicrohttpd installation; If not, see <https://www.gnu.org/licenses/>

*/
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
//#include <iostream>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <microhttpd.h>
#include "router.h"

#define PORT 8888

using namespace std;

int answer_to_connection (
		void *cls, struct MHD_Connection *connection,
		const char *url,
		const char *method, const char *version,
		const char *upload_data,
		size_t *upload_data_size, void **con_cls
) {
	struct MHD_Response *response;

	int fd;
	int ret;
	struct stat sbuf;

	Router* r = new Router(url);

	// Supporting only "GET" for now
	if(0 != strcmp(method, "GET")) { return MHD_NO; }

	if(r->IsFile()) {
		// If we can open the file, while Router(url) already checks this, it can fall back to other files
		// and therefore we do a final check here.
		// Some optimisation could be made for future release when this application is more realised
		if(-1 == (fd = open(r->GetFilepath(), O_RDONLY)) || 0 != fstat(fd, &sbuf)){
			/* error accessing file */
			if(fd != -1) { close (fd); }
			const char* errorstr = "An internal server error has occured";
			response  = MHD_create_response_from_buffer(
					strlen(errorstr),
					(void *) errorstr,
					MHD_RESPMEM_PERSISTENT
			);
			if(response) {
				ret = MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
				MHD_destroy_response(response);
				return MHD_YES;
			} else {
				return MHD_NO;
			}
		}
		response = MHD_create_response_from_fd_at_offset64(sbuf.st_size, fd, 0);
	} else {
		// If we're not a direct file to return, we're assuming its a buffer of type char* we're returning.
		response = MHD_create_response_from_buffer (
			strlen (r->GetContent().c_str()),
			(void*) r->GetContent().c_str(), MHD_RESPMEM_MUST_COPY
		);
	}
	MHD_add_response_header(response, "Content-Type", r->GetContentType());

	// TODO: Should support more of these.
	// TODO: Should enumerate the MHD_HTTP_X response codes in the Router class.
	if (r->GetResponseCode() == 200){
		ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
	} else {
		ret = MHD_queue_response (connection, MHD_HTTP_NOT_FOUND, response);
	}

	MHD_destroy_response (response);

	return ret;
}

int main ()
{
	struct MHD_Daemon *daemon;

	daemon = MHD_start_daemon (
		MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
		&answer_to_connection, NULL, MHD_OPTION_END
	);
	if (NULL == daemon) return 1;

	getchar ();

	MHD_stop_daemon (daemon);
	return 0;
}
