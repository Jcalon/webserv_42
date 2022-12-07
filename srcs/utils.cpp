/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 10:11:58 by mbascuna          #+#    #+#             */
/*   Updated: 2022/12/07 22:33:28 by jcalon           ###   ########.fr       */
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

std::vector<std::string> ft_cpp_split_str(const std::string &str, const std::string &charset)
{
	std::vector<std::string> res;
	if (str.find(charset) != std::string::npos)
	{
		res.push_back(str.substr(0, str.find(charset)));
		res.push_back(str.substr(str.find(charset) + charset.length(), str.length() - str.find(charset) - charset.length() - 1));
	}
	else
		res.push_back(str);
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

void	ctrl_handler(int sig_num)
{
	if (sig_num == SIGINT)
	{
		running = 0;
		std::cout << std::endl << "Server(s) closed !" << std::endl;
	}
}

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		((unsigned char *)b)[i] = c;
		i++;
	}
	return (b);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dest && !src)
		return (NULL);
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}

void	ft_strcpy(const char *src, char *dest)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}
