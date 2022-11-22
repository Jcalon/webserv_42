/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 10:10:56 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/22 10:29:35 by mbascuna         ###   ########.fr       */
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
#include <string>

#include "./Config/Location.hpp"
#include "./Config/Server.hpp"
#include "./Config/Config.hpp"

std::vector<std::string> ft_cpp_split(const std::string &str, const std::string &charset);
bool	ft_in_charset(char const c, const std::string &charset);



#endif
