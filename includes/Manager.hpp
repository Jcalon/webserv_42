#ifndef MANAGER_HPP
# define MANAGER_HPP

#include "utils.hpp"

class	Manager
{
	public:
		Manager(std::vector<Socket>	Sockets);
		Manager();
		~Manager();

		int						setup(void);
		void					run(void);
		void					clean(void);

		void							addSocket(Socket toAdd);

		std::vector<Socket> &			getSocket(void);

	private:
		void						handleIncompleteRequests(fd_set reading_set);

		std::vector<Socket>			_vecSockets;
		std::map<long, Socket *>		_listen_fds;
		std::map<long, Socket *>		_read_fds;
		std::vector<int>			_write_fds;
		std::map<long, time_t>		_incompleteRequests;

		fd_set						_fd_set;
		unsigned int				_fd_size;
		long						_max_fd;

};

#endif
