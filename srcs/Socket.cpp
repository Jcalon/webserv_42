#include "../includes/utils.hpp"

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

Socket::Socket(Server serv) : _server(serv), _socket(-1), _new_socket(-1)
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Socket::~Socket() // remplacer les closes
{
	clean();
}

void	Socket::clean(void)
{
	if (this->_socket > -1)
		close(this->_socket);
	if (this->_new_socket > -1)
    	close(this->_new_socket);
}
/*
** --------------------------------- METHODS ----------------------------------
*/

int Socket::startSocket()
{

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
	_socketAddress.sin_port = htons(atoi(_server.get_listen()[0].c_str())); // recup le premier port
	_socketAddress.sin_addr.s_addr = inet_addr((_server.get_ip().c_str()));
	_socketAddress_len = sizeof(_socketAddress);
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
	if (ret == -1)
	{
		close(socket);
		std::cout << "Failed to read bytes from client socket connection" << std::endl;
		return -1;
	}
	else if (ret == 0)
		return -1;
	_receivedMessage += std::string(buffer);

	ret = 0;
	size_t i = _receivedMessage.find("\r\n\r\n");
	if (i != std::string::npos)
	{
		if (_receivedMessage.find("Transfer-Encoding: chunked") != std::string::npos)	// Si on a Transfer-Encoding: chunked, y a une prio. Si on recoit la fin du chunked message, on arrete de receive peinard.
		{
			if (receivedLastChunk(_receivedMessage))
				ret = 0;
			else
				ret = 1;
		}
		else if (_receivedMessage.find("Content-Length: ") != std::string::npos)		//On check la Content-Length.
		{
			size_t len = extractContentLength(_receivedMessage);
			if (_receivedMessage.size() >= i + 4 + len)
			{
				_receivedMessage = _receivedMessage.substr(0, i + 4 + len); // On drop tout le superflu apres la Content-Length
				ret = 0;
			}
			else
				ret = 1;
		}
		else
			ret = 0; // Si on a fini et qu'il y a pas de body
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
		// std::cout << BLUE << request << RESET;
		std::ostringstream ss;
		ss << "------ Received Request from client ------\n\n";
		log(ss.str());


		Response response(request, getServer());
		response.call_method();
		std::ostringstream sts;
		if (request.getRequest()._method == "HEAD")
		{
			sts << response.get_header() << "\r\n";
			std::cout << GREEN << response.get_header() << "\r\n" << response.get_response() << RESET << std::endl;
		}
		else
		{
			sts << response.get_header() << "\r\n" << response.get_response();
			std::cout << GREEN << response.get_header() << "\r\n" << RESET << std::endl;
		}

		unsigned long bytesSent;
		bytesSent = send(socket, sts.str().c_str(), sts.str().length(), 0);

		if (bytesSent == sts.str().size())
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
	_receivedMessage = "";
	return 0;
	// Response				response; a la place de ce qu'il y a dans cette fct

}

int			Socket::getSocket() const
{
	return this->_socket;
}

Server 	Socket::getServer() const
{
	return this->_server;
}
