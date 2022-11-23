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
													_serverMessage()
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
	//ouverture d'un fd. _socket = fd
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
	ss << BLUE "\n*** Listening on ADDRESS: " << _ip_address << " PORT: " << ntohs(_socketAddress.sin_port) <<" ***\n\n" RESET; // inet ntoa a recoder
	log(ss.str());

	int bytesReceived;

	while (true)
	{
		log(BOLD "  ======= Waiting for a new connection =======\n\n" RESET);
		acceptConnection(_new_socket);

		char buffer[BUFFER_SIZE] = {0};
		bytesReceived = recv(_new_socket, buffer, BUFFER_SIZE, 0);
		if (bytesReceived < 0)
			exitWithError(RED "Failed to read bytes from client socket connection" RESET);
		std::cout << buffer << std::endl;
		// try
		// {
			Request request = Request(buffer);
			std::cout << request;
		// }
		// catch (const std::exception& e)
		// {
			// std::cerr << &e << std::endl;
		// }
		std::ostringstream ss;
		ss << GREEN "------ Received Request from client ------\n\n" RESET;
		log(ss.str());

		Response response(request);
		_serverMessage = buildResponse(response);
		// response.sendResponse();
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

std::string Server::buildResponse(Response &res) // La ca va dependre des LOCATIONS etde la methode demandee
{
	//si la method demandee n'est pas dans allow method > renvoi erreur
	// std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
	// std::cout << "coucouc " << res.get_response() << std::endl;
	res.call_method();
	std::ostringstream ss;
	ss << res.get_header() << "\n\n"
		<< res.get_response();
	std::cout << res.get_header() << std::endl;

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
		//close la socket
		log("Error sending response to client");
	}
}

/* ************************************************************************** */
