/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:57:27 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/22 10:15:35 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

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

	Config Config(file);
	std::cout << Config << std::endl;

	return (0);
}
