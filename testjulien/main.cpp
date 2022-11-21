#include "Server.hpp"

int main()
{
	Server server = Server("0.0.0.0", 8083);

	server.startListen();

	return 0;
}