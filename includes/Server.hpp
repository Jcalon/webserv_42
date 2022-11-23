#ifndef SERVER_HPP
# define SERVER_HPP

#include "utils.hpp"

class Server
{
        public:
	        Server(std::string ip_adress, int port);
	        ~Server();
	        void startListen();
                int get_socket() const;

	private:
                std::string             _ip_address;
                int		        _port;
                int		        _socket;
                int		        _new_socket;
                long		        _incomingMessage; // la request ?
                struct sockaddr_in	_socketAddress;
                unsigned int		_socketAddress_len;
                std::string		_serverMessage;

                int		startServer();
                void		closeServer();
                void		acceptConnection(int &new_socket);
                std::string	buildResponse(Response &res);
                void		sendResponse();

};

#endif /* ********************************************************** SERVER_H */
