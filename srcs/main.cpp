/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:57:27 by mbascuna          #+#    #+#             */
/*   Updated: 2022/12/01 14:58:44 by jcalon           ###   ########.fr       */
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
		std::vector<Server> servs= Config.get_server();
		Manager serv;
		for (std::vector<Server>::iterator it = servs.begin(); it != servs.end(); it++)
		{
			serv.addSocket(Socket(*it));
		}
		if (serv.setup() == -1)
		{
			std::cout << "Couldn't setup manager" << std::endl;
			exit(1);
		}
		serv.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}


	return (0);
}
