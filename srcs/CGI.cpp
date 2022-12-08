#include "../includes/utils.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CGI::CGI(Request const &request, Server const &server, std::string binary, std::string target)
{
	std::string contenttype = "text/html";
	std::vector<std::string> tmp = request.getFields();
	for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
	{
		if (it->find("Content-Type: ") != std::string::npos)
		{
			contenttype = it->substr(14, it->length() - 14);
			break ;
		}
	}
	_binary = binary;
	_target = target;
	_inputbody = request.getBody();

	std::vector<std::string> tmp1 = request.getFields();
	for (std::vector<std::string>::iterator it = tmp1.begin(); it != tmp1.end(); it++)
	{
		if (it->find(":") != std::string::npos)
		{
			std::vector<std::string> split = ft_cpp_split_str(*it, ": ");
			ft_to_upper(split[0]);
			split[0] = replace(split[0], "-", "_");
			_env["HTTP_" + split[0]] = split[1];
		}
	}
	_env["REDIRECT_STATUS"] = "200";
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["SCRIPT_NAME"] = _binary;
	_env["REQUEST_METHOD"] = request.getRequest()._method;
	_env["CONTENT_LENGTH"] = ft_to_string(request.getBody().length());
	_env["CONTENT_TYPE"] = contenttype;
	_env["REQUEST_URI"] = request.getRequest()._target; //URL
	_env["PATH_INFO"] = request.getRequest()._target; //URL
	_env["PATH_TRANSLATED"] = _target; // NBUILD TARGET LOCAL PATH
	_env["QUERY_STRING"] = request.getRequest()._query;
	if (_env["QUERY_STRING"] == "" && _inputbody.find("=") != std::string::npos)
		_env["QUERY_STRING"] = _inputbody;
	_env["REMOTE_ADDR"] = server.get_ip();
	_env["AUTH_TYPE"] = "";
	_env["REMOTE_IDENT"] = "";
	_env["REMOTE_USER"] = "user";
	_env["REQUEST_URI"] = request.getRequest()._target;
	_env["SERVER_NAME"] = server.get_name()[0];
	_env["SERVER_PORT"] = server.get_listen()[0];
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_SOFTWARE"] = "webserv";
// 	std::map<std::string, std::string> tmp2 = _env;
// 	for (std::map<std::string, std::string>::iterator it = tmp2.begin(); it != tmp2.end(); it++)
// 		std::site/test.php << it->first << ": " << it->second << std::endl << std::endl;
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

	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++, i++)
	{
		tmp = it->first + "=" + it->second;
		env[i] = new char[tmp.size() + 1];
		ft_strcpy(tmp.c_str(), env[i]);
	}
	env[i] = NULL;
	return env;
}

std::string		CGI::interpreter(void)
{
	std::string	body;
	pid_t					pid;
	int						std_fd[2];
	char					tmp[BUFFER_SIZE + 1];
	int						ret = 1;

	std_fd[0] = dup(STDIN_FILENO);
	std_fd[1] = dup(STDOUT_FILENO);

	FILE	*input_tmpfile = tmpfile();
	FILE	*output_tmpfile = tmpfile();
	int		input_fd = fileno(input_tmpfile);
	int		output_fd = fileno(output_tmpfile);

	write(input_fd, _inputbody.c_str(), _inputbody.length());
	lseek(input_fd, 0, SEEK_SET);

	pid = fork();
	if (pid == -1)
		return "500";
	else if (pid == 0)
	{
		char **av = new char * [3];
		av[0] = new char [_binary.length() + 1];
		av[1] = new char [_target.length() + 1];

		dup2(output_fd, STDOUT_FILENO);
		dup2(input_fd, STDIN_FILENO);

		ft_strcpy(_binary.c_str(), av[0]);
		ft_strcpy(_target.c_str(), av[1]);
		av[2] = NULL;
		execve(_binary.c_str(), av, mapEnvToTab());
		std::cout << "500";
	}
	else
	{
		waitpid(-1, NULL, 0);
		lseek(output_fd, 0, SEEK_SET);
		while (ret > 0)
		{
			ft_memset(tmp, 0, BUFFER_SIZE);
			ret = read(output_fd, tmp, BUFFER_SIZE);
			tmp[BUFFER_SIZE] = '\0';
			body += tmp;
		}

		close(output_fd);
		close(input_fd);
		dup2(std_fd[0], STDIN_FILENO);
		dup2(std_fd[1], STDOUT_FILENO);
	}
	return body;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
