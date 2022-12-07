/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 13:37:47 by mbascuna          #+#    #+#             */
/*   Updated: 2022/12/07 14:29:52 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
		return (0);
	try
	{
		Config Config(av[ac - 1]);
		// std::cout << Config << std::endl;
		if (Config.get_error() == true)
		{
			std::cout << RED << "error parsing" << RESET << std::endl;
			return 0;
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
