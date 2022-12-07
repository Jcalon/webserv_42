/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 13:37:40 by mbascuna          #+#    #+#             */
/*   Updated: 2022/12/07 22:36:00 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <csignal>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <exception>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <string>

#include "Request.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "Autoindex.hpp"
#include "Config.hpp"
#include "CGI.hpp"
#include "Response.hpp"
#include "Manager.hpp"

#define BUFFER_SIZE 30720
#define SOCKET_MAX	1000000

#define BOLD "\033[1m"
#define RESET "\033[0m"
#define UL "\033[4m"
#define IT "\033[3m"
#define ULL "\033[7m"
#define TAB "\t"
#define YELLOW "\033[93m"
#define YELLDARK "\033[0;33;2m"
#define RED "\033[91m"
#define REDARK "\033[0;31;2m"
#define BLUE "\033[96m"
#define BLUEDARK "\033[7;0;44;2m"
#define GREEN "\033[92m"
#define GREENDARK "\033[0;32;2m"
#define PINK "\033[95m"
#define PINKDARK "\033[0;35;2m"
#define STAR "\033[93m > \033[0m"
#define WHITESPACES " \t;"

extern int	running;

std::vector<std::string> 	ft_cpp_split(const std::string &str, const std::string &charset);
std::vector<std::string> 	ft_cpp_split_str(const std::string &str, const std::string &charset);
bool						ft_in_charset(char const c, const std::string &charset);
std::string					ft_to_string(int i);
std::string					ft_to_upper(std::string &str);
std::string					replace(std::string src, std::string search, std::string replace);
unsigned long				ft_str_to_int(std::string str);
bool 						is_readable(const std::string &file);
void						ctrl_handler(int sig_num);
void						*ft_memcpy(void *dest, const void *src, size_t n);
void						*ft_memset(void *b, int c, size_t len);
void						ft_strcpy(const char *src, char *dest);

#endif
