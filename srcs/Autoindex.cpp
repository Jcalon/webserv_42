/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 11:57:18 by mbascuna          #+#    #+#             */
/*   Updated: 2022/12/01 12:33:30 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

Autoindex::Autoindex(std::string const &path)
{
	this->_html = "<h1>Index</h1></br>";
	this->_path = path;
	read_directory();
}

Autoindex::~Autoindex() {}

void Autoindex::read_directory()
{
	DIR				*dir; // pointer to directory
	struct dirent	*entry; //all stuff in the directory
	struct stat info; // info about each entry

	dir = opendir(_path.c_str());
	if (!dir)
		return ;
	file = readdir(dir);
	while (file)
	{
		_list.push_back(file->d_name);
		file = readdir(dir);
	}
	closedir(dir);

}
//CREERVECTOR DE FILE NAME
	//si cest un directory => ouvrir le dossier (opendir / readdir)
	//lire le directory et push tous les filesname dans le tableau
	//close le dir

//CREER DES URLS
	// parcourir les files name
	//si pas de / a la fin de l'url envoyee ajouter un /
	//inserer cette url au html de lauto index
