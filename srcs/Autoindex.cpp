/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 11:57:18 by mbascuna          #+#    #+#             */
/*   Updated: 2022/12/01 18:19:59 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

Autoindex::Autoindex(std::string const &path)
{
	this->_html = "<h1>Index of " + path + "</h1></br>";
	this->_path = path;

	read_directory();

	insert_html();

}

Autoindex::~Autoindex() {}

void	Autoindex::read_directory()
{
	DIR				*dir; // pointer to directory
	struct dirent	*entry; //all stuff in the directory

	dir = opendir(_path.c_str());
	if (!dir)
		return ;
	entry = readdir(dir);
	while (entry)
	{
		_list_name.push_back(entry->d_name);
		entry = readdir(dir);
	}
	closedir(dir);
}

void	Autoindex::creation_href(void)
{
	_list_href = _list_name;
	std::string new_path = "";
	std::vector<std::string> path_noroot = ft_cpp_split(_path, "/");
	for (std::vector<std::string>::iterator it = path_noroot.begin() + 1; it != path_noroot.end(); it++)
	{
		new_path += *it + "/";
	}
	for (std::vector<std::string>::iterator it = _list_href.begin(); it != _list_href.end(); it++)
	{

		*it = (new_path[new_path.size() - 1] == '/') ? new_path + *it : new_path + "/" + *it;
	}

}

void	Autoindex::insert_html(void)
{
	size_t i = 0;

	creation_href();
	while (i < _list_href.size())
	{
		_html += "\n<tr><td><a href=\"" + _list_href[i] + "\">		" + _list_name[i] + "</a></td><td style=\"padding-left: 250px;\">" + "</td></tr></br>";
		i++;
	}
	_html += "</table><br/><hr>";
}

// void			Autoindex::addIndexLine(int line)
// {
// 	_html += "\n<tr><td><a href=\"" + _entries[line] + "\">		" + _list[line] + "</a></td><td style=\"padding-left: 250px;\">" + "</td></tr>";
// }

std::string Autoindex::get_html(void) const { return this->_html; }

//CREERVECTOR DE FILE NAME
	//si cest un directory => ouvrir le dossier (opendir / readdir)
	//lire le directory et push tous les filesname dans le tableau
	//close le dir

//CREER DES URLS
	// parcourir les files name
	//si pas de / a la fin de l'url envoyee ajouter un /
	//inserer cette url au html de lauto index
