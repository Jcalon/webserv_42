/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:57:27 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/21 17:46:17 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Config/Config.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <iterator>

int main(int ac, char **av)
{
	std::ifstream   ifs(av[ac - 1]);
	std::vector<std::string> file;
	if (!ifs.is_open())
	{
		std::cout << "Error opening infile!" << std::endl;
		return (1);
	}
	std::string	line;
	while (std::getline(ifs, line, '\n'))
	{
		if (line.size())
			file.push_back(line);
	}
	ifs.close();
	// for (std::vector<std::string>::iterator it = file.begin(); it != file.end(); it++)
	// 	std::cout << "line : " << *it << std::endl;
	Config Config(file);

	std::cout << Config << std::endl;
	return (0);
}
