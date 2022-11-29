#ifndef CGI_HPP
# define CGI_HPP

# include "utils.hpp"

class CGI
{
	public:

		CGI(Request const &request, Server const &server);
		~CGI();
		std::string	interpreter(void);

	private:
		std::map<std::string, std::string>	_env;
		char **mapEnvToTab(void);

};
#endif