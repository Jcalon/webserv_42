/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:57:27 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/22 13:51:37 by mbascuna         ###   ########.fr       */
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
		Server server = Server("0.0.0.0", test.get_listen()[0]);
		server.startListen();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}


	return (0);
}
