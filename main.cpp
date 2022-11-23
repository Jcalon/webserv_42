/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:57:27 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/22 16:01:30 by mbascuna         ###   ########.fr       */
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
		std::cout << Config << std::endl;
		Socket test = Config.get_server()[0];
		Server server = Server(test.get_ip(), test.get_listen()[0]);
		std::cout << server.get_socket() << std::endl;
		server.startListen();
		// close(server.get_socket());
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}


	return (0);
}
