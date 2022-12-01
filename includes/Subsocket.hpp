#ifndef SUBSOCKET_HPP
# define SUBSOCKET_HPP

#include "utils.hpp"

class Subsocket
{

	public:
		Subsocket(int port);
		~Subsocket();

	private:
        std::string			_ip_address;
        int					_port;
        int					_socket;
        int					_new_socket;
        struct sockaddr_in	_socketAddress;
        unsigned int		_socketAddress_len;

		void 		startListen();
        int			startServer();
        void		closeServer();
        void		acceptConnection(int &new_socket);
        std::string	buildResponse();
        void		sendResponse();

};

#endif