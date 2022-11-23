#include "../includes/utils.hpp"

const int BUFFER_SIZE = 30720;

void log(const std::string &message)
{
	std::cout << message << std::endl;
}

void exitWithError(const std::string &errorMessage)
{
	log("ERROR: " + errorMessage);
	exit(1);
}

int								checkEnd(const std::string& str, const std::string& end)
{
	int	i = str.size();
	int	j = end.size();

	while (j > 0)
	{
		i--;
		j--;
		if (i < 0 || str[i] != end[j])
			return (1);
	}
	return (0);
}

bool						receivedLastChunk(std::string & request)
{
	if (!checkEnd(request, "0\r\n\r\n"))
		return (true);
	return (false);
}

size_t						extractContentLength(std::string const & request)
{
	size_t i = request.find("Content-Length: ");
	i += 16;

	size_t temp(i);
	size_t count(0);
	while (std::isdigit(request[temp]))
	{
		count++;
		temp++;
	}
	size_t result = std::atoi(request.substr(i, count).c_str());
	return (result);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Socket::Socket(Server serv) : _server(serv)
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Socket::~Socket() // remplacer les closes
{
	close(this->_socket);
    close(this->_new_socket);
}

void	Socket::clean(void)
{
	close(this->_socket);
    close(this->_new_socket);
}
/*
** --------------------------------- METHODS ----------------------------------
*/

int Socket::startSocket()
{
	// _socket = socket(AF_INET, SOCK_STREAM, 0);
	// if (_socket < 0)
	// {
	// 	exitWithError("Cannot create socket");
	// 	return 1;
	// }

	// if (bind(_socket, (sockaddr *)&_socketAddress, _socketAddress_len) < 0)
	// {
	// 	exitWithError("Cannot connect socket to address");
	// 	return 1;
	// }

	// return 0;

	int	on = 1;
	int	rc = 0;
	int flags = 0;

	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
	{
		exitWithError("Couldn't create server : socket() call failed");
		return 1;
	}
	rc = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (const void *)&on, sizeof(on));
	if (rc < 0)
	{
		exitWithError("Couldn't create server : setsockopt() call failed");
		close(_socket);
		return 1;
	}
	flags = fcntl(_socket, F_GETFL, 0);
	rc = fcntl(_socket, F_SETFL, flags | O_NONBLOCK);
	if (rc < 0)
	{
		exitWithError("Couldn't create server : fcntl() call failed");
		close(_socket);
		return 1;
	}
	_socketAddress.sin_family = AF_INET;
	_socketAddress.sin_port = htons(_server.get_listen()[0]); // recup le premier port
	_socketAddress.sin_addr.s_addr = inet_addr((_server.get_ip().c_str()));
	rc = bind(_socket, (struct sockaddr *)&_socketAddress, sizeof(_socketAddress));
	if (rc < 0)
	{
		exitWithError("Couldn't create server [] : bind() call failed");
		close(_socket);
		return 1;
	}
	rc = listen(_socket, 1000);
	if (rc < 0)
	{
		exitWithError("Couldn't create server : listen() call failed");
		close(_socket);
		return 1;
	}
	return(0);
}

// void Socket::startListen()
// {
// 	if (listen(_socket, 20) < 0) // 20 doit etre remplacé par les workers
// 	{
// 		exitWithError("Socket listen failed");
// 	}

// 	std::ostringstream ss;
// 	ss << "\n*** Listening on ADDRESS: " << "0.0.0.0" << " PORT: " << ntohs(_socketAddress.sin_port) << " ***\n\n"; // inet ntoa a recoder
// 	log(ss.str());

// 	int bytesReceived;

// 	while (true)
// 	{
// 		log("====== Waiting for a new connection ======\n\n\n");
// 		acceptConnection(_new_socket);

// 		char buffer[BUFFER_SIZE] = {0};
// 		bytesReceived = recv(_new_socket, buffer, BUFFER_SIZE, 0);
// 		if (bytesReceived < 0)
// 		{
// 			exitWithError("Failed to read bytes from client socket connection");
// 		}
// 		std::cout << buffer << std::endl;
// 		try
// 		{
// 			Request request = Request(buffer);
// 			std::cout << request;
// 		}
// 		catch (const std::exception& e)
// 		{
// 			std::cerr << &e << std::endl;
// 		}
// 		std::ostringstream ss;
// 		ss << "------ Received Request from client ------\n\n";
// 		log(ss.str());

// 		sendResponse();

// 		close(_new_socket); // mettre un if avec l'escape
// 	}
// }

long Socket::acceptConnection()
{
	_new_socket = accept(_socket, (sockaddr *)&_socketAddress, &_socketAddress_len);
	fcntl(_socket, F_SETFL, O_NONBLOCK);
	if (_new_socket < 0)
	{
		std::ostringstream ss;
		// ss << "Socket failed to accept incoming connection from ADDRESS: " << "0.0.0.0" << "; PORT: " << ntohs(_socketAddress.sin_port);
		// exitWithError(ss.str());
		return 1;
	}
	return _new_socket;
}

long Socket::receiveMessage(long socket)
{
	char buffer[BUFFER_SIZE] = {0};
	long ret;

	ret = recv(socket, buffer, BUFFER_SIZE - 1, 0);
	if (ret == 0 || ret == -1)
	{
		close(socket);
		exitWithError("Failed to read bytes from client socket connection");
		return 1;
	}
	_receivedMessage = std::string(buffer);

	ret = 0;
	size_t i = _receivedMessage.find("\r\n\r\n");
	if (i != std::string::npos)															// If we finished to parse the headers...
	{
		if (_receivedMessage.find("Transfer-Encoding: chunked") != std::string::npos)	// If there is a Transfer-Encoding: chunked, it has the priority. If we received the end of the chunked message, we finished receiving the request.
		{
			if (receivedLastChunk(_receivedMessage))
				ret = 0;
			else
				ret = 1;
		}
		else if (_receivedMessage.find("Content-Length: ") != std::string::npos)		// If there is no Transfer-Encoding: chunked, let's check if there is a Content-Length.
		{
			size_t len = extractContentLength(_receivedMessage);
			if (_receivedMessage.size() >= i + 4 + len)
			{
				_receivedMessage = _receivedMessage.substr(0, i + 4 + len);			// Dropping any superfluous data after we reached Content-Length
				ret = 0;
			}
			else
				ret = 1;
		}
		else
			ret = 0;																	// We finished parsing the headers and there is no body : we received the whole request
	}
	else
		ret = 1;

	if (ret == 0 && _receivedMessage.size() < 2000)
		std::cout << std::endl << "------ Received request ------" << std::endl << "[" << std::endl << _receivedMessage << "]" << std::endl << std::endl;
	else if (ret == 0 && _receivedMessage.size() >= 2000)
		std::cout << std::endl << "------ Received request ------" << std::endl << "[" << std::endl << _receivedMessage.substr(0, 500) << "...]" << std::endl << std::endl;
	return (ret);
}

long Socket::sendResponse(long socket)
{
	try
	{
		Request request = Request(_receivedMessage);
		std::cout << request;
		std::ostringstream ss;
		ss << "------ Received Request from client ------\n\n";
		log(ss.str());

		unsigned long bytesSent;

		std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Socket :) </p></body></html>";
		std::ostringstream sts;
		ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
		<< htmlFile;
		bytesSent = send(socket, ss.str().c_str(), ss.str().length(), 0);

		if (bytesSent == _SocketMessage.size())
		{
			log("------ Socket Response sent to client ------\n\n");
		}
		else
		{
			log("Error sending response to client");
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << &e << std::endl;
	}
	return 1;
	// // Response				response;

	// std::string request_str = _requests[socket];
	// _requests.erase(socket);
	// request.load(request_str);

	// ServerConfiguration &	virtualHost = findVirtualHost(request.getHeaders());
	// Route					route = findCorrespondingRoute(request.getURL(), virtualHost);

	// if (_sent.find(socket) == _sent.end())
	// {
	// 	Console::info("Transmitting request to virtual host " + virtualHost.getName() + " with server_root " + virtualHost.getServerRoot());
	// 	if ((route.requireAuth() && !request.hasAuthHeader()) || (route.requireAuth() && request.hasAuthHeader() && !this->credentialsMatch(request.getHeaders()["Authorization"], route.getUserFile())))
	// 		this->handleUnauthorizedRequests(response, virtualHost);
	// 	else if (!this->requestIsValid(response, request, route))
	// 		this->handleRequestErrors(response, route, virtualHost);
	// 	else if (requestRequireRedirection(request, route))
	// 		this->generateRedirection(response, virtualHost);
	// 	else
	// 		this->setResponseBody(response, request, route, virtualHost);
	// 	this->setResponseHeaders(response, route, request);
	// }

	// return (this->sendResponse(response, virtualHost, socket));
}

int			Socket::getSocket() const
{
	return this->_socket;
}

std::string Socket::buildResponse() // La ca va dependre des LOCATIONS etde la methode demandee
{
	std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Socket :) </p></body></html>";
	std::ostringstream ss;
	ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
		<< htmlFile;

	return ss.str();
}


/* ************************************************************************** */
