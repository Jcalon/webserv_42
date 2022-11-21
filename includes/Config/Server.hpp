/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:36:00 by mbascuna          #+#    #+#             */
/*   Updated: 2022/11/21 12:18:46 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include "Location.hpp"

class Server
{
	public:
		Server(void);
		~Server(void);

	private:
		std::vector<int>					_listen;
		std::string							_name;
		std::string							_root;
		std::map<int, std::string>			_error_pages; // Change les pages d'erreur des codes pour le fichier path
		bool								_auth_basic; // Change le nom de la popup d'authentification pour les ressources protégées par un mot de passe
		std::string							_auth_basic_user_file; // Définit le fichier contenant les logins et mot de passes pour accéder à la ressourc
		int									_max_client_body_size; // Taille maximale du body d'une requête
		std::string							_cgi_dir; //chemin du dossier contenant les executables CGI
		std::map<std::string, std::string>	_cgi_ext; // CGI binary that will be executed for the given extension.
		std::string 						_index; //defines files that will be used as an index.
		bool								_autoindex; // Si une requête est faite sur un dossier, l'autoindex liste les fichiers présents dans le dossier

		std::vector<Location>	_location;
};

#endif
