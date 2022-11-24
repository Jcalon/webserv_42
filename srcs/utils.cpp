/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 10:11:58 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/22 10:12:26 by mbascuna         ###   ########.fr       */
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
