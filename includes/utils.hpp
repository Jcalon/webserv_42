/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 10:10:56 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/30 11:52:40 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <iterator>
#include <cstdlib>
#include <exception>
#include <string>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <cstring>

#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <sys/time.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>


#include "Location.hpp"
#include "Server.hpp"
#include "Config.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "CGI.hpp"

#include "Socket.hpp"
#include "Manager.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 30720

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

std::vector<std::string> 	ft_cpp_split(const std::string &str, const std::string &charset);
bool						ft_in_charset(char const c, const std::string &charset);
std::string					ft_to_string(int i);


#endif
