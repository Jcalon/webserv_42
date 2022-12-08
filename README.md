# webserv_42
This project is about writing your own HTTP server in C++98.

How to launch yout server : make && ./weberv [your_configuration_file.conf]

How to create your own .conf file :

	- There is some sample in the /configuration folder;
	- Each server configuration should be set up in {}, preceded by "server";
	- Each location configuration should be set up in a server, can't be nested and should be followed by a /name;
	- Options :
		- listen				ip:port or listen	port (a default port can be setup with "default" keyword);
		- server_name			name;
		- root					root_of_the_server;
		- error_page			errnum	path_to_error_page;
		- allow_method			list_of_your_allowed_methods(GET, HEAD, POST, PUT, DELETE);
		- autoindex				on/off;
		- index					index_to_load;
		- cgi_ext				extension_for_the cgi(.php is working);
		- cgi_ext				cgi_directory;
		- client_max_body_size	set_a_request_max_body_size;
