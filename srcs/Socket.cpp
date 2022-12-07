#include "../includes/utils.hpp"

/*
** ------------------------------- UTILS --------------------------------
*/

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
		std::cout << RED << "ERROR: " << RESET << "Couldn't create server : socket() call failed" << std::endl;
		return 1;
	}
	rc = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (const void *)&on, sizeof(on));
	if (rc < 0)
	{
		std::cout << RED << "ERROR: " << RESET << "Couldn't create server : setsockopt() call failed" << std::endl;
		close(_socket);
		return 1;
	}
	flags = fcntl(_socket, F_GETFL, 0);
	rc = fcntl(_socket, F_SETFL, flags | O_NONBLOCK);
	if (rc < 0)
	{
		std::cout << RED << "ERROR: " << RESET << "Couldn't create server : fcntl() call failed" << std::endl;
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
		std::cout << RED << "ERROR: " << RESET << "Couldn't create server [] : bind() call failed" << std::endl;
		close(_socket);
		return 1;
	}
	rc = listen(_socket, 1000);
	if (rc < 0)
	{
		std::cout << RED << "ERROR: " << RESET << "Couldn't create server : listen() call failed" << std::endl;
		close(_socket);
		return 1;
	}
	return(0);
}

long Socket::acceptConnection()
{
	_new_socket = accept(_socket, NULL, NULL);
	fcntl(_socket, F_SETFL, O_NONBLOCK);
	return _new_socket;
}

long Socket::receiveMessage(long socket)
{
	char buffer[BUFFER_SIZE] = {0};
	long ret;

	ret = recv(socket, buffer, BUFFER_SIZE - 1, 0);
	if (ret == 0 || ret == -1)
	{
		_receivedMessage.erase(socket);
		close(socket);
		if (ret == -1)
			std::cout << "Failed to read bytes from client socket connection" << std::endl;
		return -1;
	}
	_receivedMessage[socket] += std::string(buffer);

	ret = 0;
	size_t i = _receivedMessage[socket].find("\r\n\r\n");
	if (i != std::string::npos)
	{
		if (_receivedMessage[socket].find("Transfer-Encoding: chunked") != std::string::npos)	// Si on a Transfer-Encoding: chunked, y a une prio. Si on recoit la fin du chunked message, on arrete de receive peinard.
		{
			if (receivedLastChunk(_receivedMessage[socket]))
				ret = 0;
			else
				ret = 1;
		}
		else if (_receivedMessage[socket].find("Content-Length: ") != std::string::npos)		//On check la Content-Length.
		{
			size_t len = extractContentLength(_receivedMessage[socket]);
			if (_receivedMessage[socket].size() >= i + 4 + len)
			{
				_receivedMessage[socket] = _receivedMessage[socket].substr(0, i + 4 + len); // On drop tout le superflu apres la Content-Length
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

	if (ret == 0 && _receivedMessage[socket].size() < 2000)
		std::cout << std::endl << "Received request on: " << _server.get_ip() << ":" << _server.get_listen()[0] << std::endl << std::endl << BLUE << "[" << _receivedMessage[socket] << "]" << RESET << std::endl << std::endl;
	else if (ret == 0 && _receivedMessage[socket].size() >= 2000)
		std::cout << std::endl << "Received request on: " << _server.get_ip() << ":" << _server.get_listen()[0] << std::endl << std::endl << BLUE << "[" << _receivedMessage[socket].substr(0, 500) << "...]" << RESET << std::endl << std::endl;
	return (ret);
}

long Socket::sendResponse(long socket)
{
	if (_sent.find(socket) == _sent.end())
	{
		Request request = Request(_receivedMessage[socket]);
		_receivedMessage.erase(socket);
		_sent[socket] = 0;
		if (request.getError() != 0)
		{
			Response response(request, request.getError());
			std::ostringstream sts;
			if (request.getRequest()._method == "HEAD")
				sts << response.get_header() << "\r\n\r\n";
			else
				sts << response.get_header() << "\r\n\r\n" << response.get_response();
			_socketMessage[socket] = sts.str();
		}
		else
		{
			Response response(request, getServer());
			response.call_method();
			std::ostringstream sts;
			if (request.getRequest()._method == "HEAD")
				sts << response.get_header() << "\r\n\r\n";
			else
				sts << response.get_header() << "\r\n\r\n" << response.get_response();
			_socketMessage[socket] = sts.str();
		}
	}
	std::string chunked = _socketMessage[socket].substr(_sent[socket], SOCKET_MAX);
	long bytesSent;
	bytesSent = send(socket, chunked.c_str(), chunked.length(), 0);
	if (bytesSent == -1)
	{
		close(socket);
		_sent[socket] = 0;
		_socketMessage.erase(socket);
		return (-1);
	}
	else
	{
		_sent[socket] += bytesSent;
		if (_sent[socket] >= _socketMessage[socket].size())
		{
			if (_socketMessage[socket].size() < 2000)
				std::cout << std::endl << "Send response on: " << _server.get_ip() << ":" << _server.get_listen()[0] << std::endl << GREEN << std::endl << "[" << _socketMessage[socket] << "]" << std::endl << RESET << std::endl;
			else
				std::cout << std::endl << "Send response on: " << _server.get_ip() << ":" << _server.get_listen()[0] << std::endl << GREEN << std::endl<< "[" << _socketMessage[socket].substr(0, 500) << "]" << std::endl << RESET << std::endl;
			_socketMessage.erase(socket);
			_sent.erase(socket);
			return (0);
		}
		else
			return (1);
	}
}

int			Socket::getSocket() const
{
	return this->_socket;
}

Server 	Socket::getServer() const
{
	return this->_server;
}

void	Socket::resetSocket(long socket)
{
	_receivedMessage.erase(socket);
}
