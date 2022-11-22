#ifndef SERVER_HPP
# define SERVER_HPP

#include "utils.hpp"

class Server
{

	public:
		Server(std::string ip_adress, int port);
		~Server();
		void startListen();

	private:
        std::string			_ip_address;
        int					_port;
        int					_socket;
        int					_new_socket;
        long				_incomingMessage;
        struct sockaddr_in	_socketAddress;
        unsigned int		_socketAddress_len;
        std::string			_serverMessage;

        int			startServer();
        void		closeServer();
        void		acceptConnection(int &new_socket);
        std::string	buildResponse();
        void		sendResponse();

};

#endif /* ********************************************************** SERVER_H */
