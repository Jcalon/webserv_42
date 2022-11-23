#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "utils.hpp"

class Socket
{

	public:
		Socket(Server server);
		~Socket();
		void startListen();

	private:
		Server 				_server;
        int					_socket;
        int					_new_socket;
        long				_incomingMessage;
        struct sockaddr_in	_socketAddress;
        unsigned int		_socketAddress_len;
        std::string			_SocketMessage;
		std::string			_receivedMessage;
	
	public:
        int			startSocket();
        void		closeSocket();

        long		acceptConnection();
		long			receiveMessage(long socket);
        long		sendResponse(long socket);

        std::string	buildResponse();
		int			getSocket() const;
		void	clean(void);
};

#endif
