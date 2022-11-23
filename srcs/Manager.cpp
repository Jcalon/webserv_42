
#include "../includes/utils.hpp"

Manager::Manager(std::vector<Socket> Sockets): _vecSockets(Sockets)
{}

Manager::Manager(): _vecSockets()
{}


Manager::~Manager()
{}



/*
** ------ UTILITY FUNCTIONS ------
*/
void							Manager::addSocket(Socket toAdd)
{ this->_vecSockets.push_back(toAdd); }


/*
** ------ GETTERS | SETTERS ------
*/
std::vector<Socket> &		Manager::getSocket(void)
{ return (_vecSockets); }


/*
** ------ CORE FUNCTIONS ------
*/

int				Manager::setup(void)
{
	FD_ZERO(&_fd_set);
	_fd_size = _vecSockets.size();
	_max_fd = 0;
	std::cout << " SIZE " << _fd_size << std::endl;
	for (std::vector<Socket>::iterator lstn = _vecSockets.begin(); lstn != _vecSockets.end(); lstn++)
	{
		long	fd;
		if ((*lstn).startSocket() != 1)
		{
			fd = (*lstn).getSocket();
			std::cout << fd << std::endl;
			FD_SET(fd, &_fd_set);
			_listen_fds.insert(std::make_pair(fd, &(*lstn)));
			if (fd > _max_fd)
				_max_fd = fd;
			//Console::info("Finished setting up Socket [" + (*lstn).getDefaultVHConfig().getHost() + ":" + Utils::to_string((*lstn).getDefaultVHConfig().getPort()) + "]");
			std::cout << "			This Socket has the following virtual hosts :" << std::endl;
			// for (std::vector<SocketConfiguration>::iterator it = (*lstn).getVirtualHosts().begin(); it != (*lstn).getVirtualHosts().end(); it++)
			// {
			// 	if ((*it).isDefault())
			// 		std::cout << "				> " << (*it).getName() << GREEN << "	(default)" << NC << std::endl;
			// 	else
			// 		std::cout << "				> " << (*it).getName() << std::endl;
			// }
		}
	}
	if (_max_fd == 0)
	{
		//Console::error("Couldn't set up connexion manager");
		return (-1);
	}
	else
		return (0);
}

void			Manager::run(void)
{
	while (1)
	{
		fd_set			reading_set;
		fd_set			writing_set;
		struct timeval	timeout;
		int				ret = 0;
		while (ret == 0)
		{
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;
			memcpy(&reading_set, &_fd_set, sizeof(_fd_set));
			FD_ZERO(&writing_set);
			for (std::vector<int>::iterator it = _write_fds.begin(); it != _write_fds.end(); it++)
				FD_SET(*it, &writing_set);

			ret = select(_max_fd + 1, &reading_set, &writing_set, 0, &timeout);
			this->handleIncompleteRequests(reading_set);
		}

		if (ret > 0)
		{
			for (std::vector<int>::iterator it = _write_fds.begin(); it != _write_fds.end(); it++)
			{
				if (FD_ISSET(*it, &writing_set))
				{
					long	ret = _read_fds[*it]->sendResponse(*it);								// Make the Socket associated with writing fd call send() to this fd (the Socket associated with the fd will be the one that recieved data on this fd, so _read_fds[*it])
					
					if (ret == 0)															// We sent data without errors, and the whole response has been sent
						_write_fds.erase(it);
					else if (ret == -1)
					{
						FD_CLR(*it, &_fd_set);
						FD_CLR(*it, &reading_set);
						_read_fds.erase(*it);
						_write_fds.erase(it);
					}

					ret = 0;
					break ;
				}
			}

			for (std::map<long,Socket *>::iterator it = _read_fds.begin(); ret && it != _read_fds.end(); it++)
			{
				long	socket = it->first;
				
				if (FD_ISSET(socket, &reading_set))
				{
					std::map<long,time_t>::iterator incompleteIt = _incompleteRequests.find(socket);
					long	ret = it->second->receiveMessage(socket);
				
					if (ret == 0)
					{
						_write_fds.push_back(socket);
						if (incompleteIt != _incompleteRequests.end())
							_incompleteRequests.erase(incompleteIt);
					}
					else if (ret == -1)
					{
						FD_CLR(socket, &_fd_set);
						FD_CLR(socket, &reading_set);
						_read_fds.erase(socket);
						it = _read_fds.begin();
						if (incompleteIt != _incompleteRequests.end())
							_incompleteRequests.erase(socket);
					}
					else
						if (incompleteIt == _incompleteRequests.end())
							_incompleteRequests[socket] = time(NULL);

					ret = 0;
					break ;
				}
			}

			for (std::map<long,Socket *>::iterator it = _listen_fds.begin(); ret && it != _listen_fds.end(); it++)
			{
				long	fd = it->first;

				if (FD_ISSET(fd, &reading_set))
				{
					long socket = it->second->acceptConnection();

					if (socket != -1)
					{
						FD_SET(socket, &_fd_set);
						_read_fds.insert(std::make_pair(socket, it->second));
						if (socket > _max_fd)
							_max_fd = socket;
					}
					
					ret = 0;
					break ;
				}
			}
		}
		else
		{
			//Console::error("Connexion manager encountered an error : select() call failed");
			for (std::vector<Socket>::iterator it = _vecSockets.begin(); it != _vecSockets.end(); it++)
				(*it).clean();
			_read_fds.clear();
			_write_fds.clear();
			for (std::map<long, Socket *>::iterator it = _listen_fds.begin() ; it != _listen_fds.end() ; it++)
				FD_SET(it->first, &_fd_set);
		}
	}
}

void			Manager::handleIncompleteRequests(fd_set reading_set)
{
	for(std::map<long,Socket *>::iterator it = _read_fds.begin(); it != _read_fds.end(); it++)
	{
		long	socket = it->first;
		time_t comp = time(NULL);
		if (_incompleteRequests.find(socket) != _incompleteRequests.end() && !FD_ISSET(socket, &reading_set) && difftime(comp, _incompleteRequests[socket]) > 1200)
		{
			//Console::info("Client stopped transmitting data on an incomplete request ; bouncing the client with socket " + Utils::to_string(socket) + " on Socket [" + (it->second)->getVirtualHosts()[0].getHost() + ":" + Utils::to_string((it->second)->getVirtualHosts()[0].getPort()) + "]");
			//(it->second)->resetSocket(socket);
			close(socket);
			FD_CLR(socket, &_fd_set);
			_read_fds.erase(socket);
			_incompleteRequests.erase(socket);
		}
	}
}

void			Manager::clean(void)
{
	for (std::map<long,Socket *>::iterator it = _read_fds.begin(); it != _read_fds.end(); it++)
		close(it->first);
	for (std::vector<Socket>::iterator it = _vecSockets.begin(); it != _vecSockets.end(); it++)
		(*it).clean();
	_vecSockets.clear();
	_listen_fds.clear();
	_read_fds.clear();
	_write_fds.clear();
}
