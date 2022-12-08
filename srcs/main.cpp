
#include "../includes/utils.hpp"

int running;

int main(int ac, char **av)
{
	running = 1;
	if (ac != 2)
		return (0);
	try
	{
		Config Config(av[ac - 1]);
		std::cout << Config << std::endl;
		if (Config.get_error() == true)
		{
			std::cout << RED << "ERROR: " << RESET << "Server can't be configurated, go check your .conf file..." << std::endl;
			return 1;
		}
		std::vector<Server> servs= Config.get_server();
		Manager serv;
		for (std::vector<Server>::iterator it = servs.begin(); it != servs.end(); it++)
		{
			serv.addSocket(Socket(*it));
		}
		if (serv.setup() == -1)
		{
			std::cout << RED << "ERROR: " << RESET << "Couldn't setup servers..." << std::endl;
			return (EXIT_FAILURE);
		}
		std::signal(SIGINT, ctrl_handler);
		serv.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
