/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 10:54:30 by jcalon            #+#    #+#             */
/*   Updated: 2022/12/07 10:54:31 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		Server 						_server;
        int							_socket;
        int							_new_socket;
        long						_incomingMessage;
        struct sockaddr_in			_socketAddress;
        unsigned int				_socketAddress_len;
        std::map<long, std::string>	_socketMessage;
		std::map<long, size_t>		_sent;
		std::map<long, std::string>	_receivedMessage;
	
	public:
        int		startSocket();
        long	acceptConnection();
		long	receiveMessage(long socket);
        long	sendResponse(long socket);
		int		getSocket() const;
		Server 	getServer() const;
		void	clean(void);
		void	resetSocket(long socket);
};

#endif
