#include "../includes/utils.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CGI::CGI(Request const &request, Server const &server)
{
	// std::string contentlength;
	// std::vector<std::string>::iterator it = request.getFields().begin();
	// for (; it != request.getFields().end(); it++)
	// {
	// 	if (*it->find("Content-Length: ") != std::string::npos)
	// 	{
	// 		contentlength == **it + 16;
	// 		break ;
	// 	}
	// }

	// std::string contenttype;
	// std::vector<std::string>::iterator it = request.getFields().begin();
	// for (; it != request.getFields().end(); it++)
	// {
	// 	if (*it.find("Content-Type: ") != std::string::npos)
	// 	{
	// 		contenttype == **it + 14;
	// 		break ;
	// 	}
	// }

	// _env["REDIRECT_STATUS"] = "200";
	// _env["GATEWAY_INTERFACE"] = "CGI/1.1";
	// _env["SCRIPT_NAME"] = request.getRequest()._target;
	// _env["SCRIPT_FILENAME"] = request.getRequest()._target;
	// _env["REQUEST_METHOD"] = request.getRequest()._method;
	// _env["CONTENT_LENGTH"] = contentlength;
	// _env["CONTENT_TYPE"] = contenttype;
	// _env["PATH_INFO"] = request.getRequest()._target;
	// _env["PATH_TRANSLATED"] = request.getRequest()._target;
	// _env["QUERY_STRING"] = request.getRequest()._query;
	// _env["REMOTEaddr"] = server.get_ip();
	// _env["AUTH_TYPE"] = "";
	// _env["REMOTE_IDENT"] = "";
	// _env["REMOTE_USER"] = "";
	// _env["REQUEST_URI"] = request.getRequest()._target;
	// _env["SERVER_NAME"] = server.get_name()[0];
	// _env["SERVER_PORT"] = server.get_listen()[0];
	// _env["SERVER_PROTOCOL"] = "HTTP/1.1";
	// _env["SERVER_SOFTWARE"] = "webserv";
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CGI::~CGI()
{
}


/*
** --------------------------------- METHODS ----------------------------------
*/

char **CGI::mapEnvToTab(void)
{
	char **env = new char *[_env.size() + 1];
	std::string tmp;
	int i = 0;

	for (stringMap::iterator it = _env.begin(); it != _env.end(); it++, i++)
	{
		tmp = it->first + "=" + it->second;
		env[i] = new char[tmp.size() + 1];
		env[i] = std::strcpy(CgiEnv[i], tmp.c_str());
	}
	env[i] = NULL;
	return env;
}

std::string		CGI::interpreter(void)
{
	char	**env = mapEnvToTab();
	std::string	body;
	int tube[2];
	int status;

	int fdin = dup(STDIN_FILENO);
	if (fdin < 0)
	{
		std::cerr << "Error: dup\n";
		return "ERROR";
	}

	if (pipe(tube) < 0)
	{
		std::cerr << "Error: pipe\n";
		return "ERROR";
	}

	pid_t cgi_pid = fork();
	if (cgi_pid == -1)
		return ("Status: 500\r\n");
	else if (cgi_pid == 0)
	{
		dup2(tube[0], STDIN_FILENO);
		dup2(tube[1], STDOUT_FILENO);
		close(fdin);
		close(tube[0]);
		close(tube[1]);
		//execve(INTERPRETER, NULL, env)
		write(STDOUT_FILENO, "Status: 500\r\n", 15);
		exit(0);
	}
	else
	{
		waitpid(cgi_pid, &status, 0);

		char	buffer[BUFFER_SIZE + 1];
		int ret = 1;
		while (ret > 0)
		{
			ret = read(tube[0], buffer, BUFFER_SIZE);
			buffer[ret] = '\0';
			body += buffer;
		}
	}
	close(tube[1]);	//close write side
	if (dup2(fdin, STDIN_FILENO) < 0)
		std::cerr << "Error execute_cgi: dup2 failed\n" << std::endl;
	close(tube[0]);
	close(fdin);
	return body;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
