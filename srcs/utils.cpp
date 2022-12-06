/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 10:11:58 by mbascuna          #+#    #+#             */
/*   Updated: 2022/12/06 15:24:46 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

std::vector<std::string> ft_cpp_split(const std::string &str, const std::string &charset)
{
	std::vector<std::string> res;
	std::string			tmp;
	size_t			i;

	i = 0;
	while (i < str.length())
	{
		while (i < str.length() && ft_in_charset(str[i], charset))
			i++;
		if (i < str.length())
		{
			tmp = "";
			while (i < str.length() && !ft_in_charset(str[i], charset))
				tmp += str[i++];
			res.push_back(tmp);
		}
	}
	return res;
}

bool	ft_in_charset(char const c, const std::string &charset)
{
	int	i_charset;

	i_charset = 0;
	while (charset[i_charset])
	{
		if (c == charset[i_charset++])
			return true;
	}
	return false;
}


std::string	ft_to_string(int i)
{
	std::stringstream ss;
	ss << i;
	std::string n = ss.str();
	return n;
}


std::string	ft_to_upper(std::string &str)
{
	for (size_t i = 0; i < str.length(); i++)
		str[i] = std::toupper(str[i]);
	return (str);
}

std::string	replace(std::string src, std::string search, std::string replace)
{
	for (size_t pos = 0; ; pos += replace.length())
	{
		pos = src.find(search, pos);
		if (pos == std::string::npos)
			break ;
		src.erase(pos, search.length());
		src.insert(pos, replace);
	}
	return (src);
}

unsigned long	ft_str_to_int(std::string str)
{
	std::stringstream ss;
	int num;
	ss << str;
	ss >> num;

	return num;
}

bool is_readable(const std::string &file) //indique si un fichier est lisible (et donc si il existe)
{
  std::ifstream fichier(file.c_str());
  return !fichier.fail();
}
