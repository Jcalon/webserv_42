#include "../includes/utils.hpp"

const int BUFFER_SIZE = 30720; // a remplacer

void log(const std::string &message)
{
	std::cout << message << std::endl;
}

void exitWithError(const std::string &errorMessage)
{
	log("ERROR: " + errorMessage);
	exit(1);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server(std::string ip_adress, int port) : _ip_address(ip_adress), _port(port),
													_socket(), _new_socket(), _incomingMessage(),
													_socketAddress(), _socketAddress_len(sizeof(_socketAddress)),
													_serverMessage(buildResponse())
{
	_socketAddress.sin_family = AF_INET;
	_socketAddress.sin_port = htons(_port);
	_socketAddress.sin_addr.s_addr = inet_addr(_ip_address.c_str());

	if (startServer() != 0)
	{
		std::ostringstream ss;
		ss << "Failed to start serve with PORT: " << ntohs(_socketAddress.sin_port);
		log(ss.str());
	}
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server() // remplacer les closes
{
	close(this->_socket);
    close(this->_new_socket);
}

/*
** --------------------------------- METHODS ----------------------------------
*/

int Server::startServer()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
	{
		exitWithError("Cannot create socket");
		return 1;
	}

	if (bind(_socket, (sockaddr *)&_socketAddress, _socketAddress_len) < 0)
	{
		exitWithError("Cannot connect socket to address");
		return 1;
	}

	return 0;
}

void Server::startListen()
{
	if (listen(_socket, 20) < 0) // 20 doit etre remplacé par les workers
	{
		exitWithError("Socket listen failed");
	}

	std::ostringstream ss;
	ss << "\n*** Listening on ADDRESS: " << "0.0.0.0" << " PORT: " << ntohs(_socketAddress.sin_port) << " ***\n\n"; // inet ntoa a recoder
	log(ss.str());

	int bytesReceived;

	while (true)
	{
		log("====== Waiting for a new connection ======\n\n\n");
		acceptConnection(_new_socket);

		char buffer[BUFFER_SIZE] = {0};
		bytesReceived = recv(_new_socket, buffer, BUFFER_SIZE, 0);
		if (bytesReceived < 0)
		{
			exitWithError("Failed to read bytes from client socket connection");
		}
		std::cout << buffer << std::endl;
		try
		{
			Request request = Request(buffer);
			std::cout << request;
		}
		catch (const std::exception& e)
		{
			std::cerr << &e << std::endl;
		}
		std::ostringstream ss;
		ss << "------ Received Request from client ------\n\n";
		log(ss.str());

		sendResponse();

		close(_new_socket); // mettre un if avec l'escape
	}
}

void Server::acceptConnection(int &new_socket)
{
	new_socket = accept(_socket, (sockaddr *)&_socketAddress, &_socketAddress_len);
	if (new_socket < 0)
	{
		std::ostringstream ss;
		ss << "Server failed to accept incoming connection from ADDRESS: " << "0.0.0.0" << "; PORT: " << ntohs(_socketAddress.sin_port);
		exitWithError(ss.str());
	}
}

std::string Server::buildResponse() // La ca va dependre des LOCATIONS etde la methode demandee
{
	std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
	std::ostringstream ss;
	ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
		<< htmlFile;

	return ss.str();
}

void Server::sendResponse()
{
	unsigned long bytesSent;

	bytesSent = send(_new_socket, _serverMessage.c_str(), _serverMessage.size(), 0);

	if (bytesSent == _serverMessage.size())
	{
		log("------ Server Response sent to client ------\n\n");
	}
	else
	{
		log("Error sending response to client");
	}
}

/* ************************************************************************** */
