#include "UserCommand.hpp"


UserCommand::UserCommand(void) :
_name("USER")
{
	std::cout << "┣⮕ UserCommand." << std::endl;
}

UserCommand::~UserCommand(void)
{}

bool
	UserCommand::cantExecute(Client *client)
{
	Irc	 &ircserv = Irc::getInstance();

	if (client->isRegistered()) {
		ircserv.addReply(Reply(client->getSockfd(), ERR_ALREADYREGISTERED(ircserv.getName(), client->getNickname())));
		return (false);
	}
	return (true);
}

void UserCommand::execute(int fds, Client *client)
{
	std::string	cmd = client->getCmds().front();
	std::vector<std::string> arguments = this->splitArguments(cmd);
	Irc	&ircserv = Irc::getInstance();

	if (arguments.size() != 4)
		ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNickname(), this->_name)));
	else {
		client->setUser(arguments[0]);
		client->setHost(arguments[2]);
		client->setRealname(arguments[3]);
		std::cout << "UserCommand::execute(" << fds << ", " << arguments.at(0) << ", " << arguments.at(1) << ", " << arguments.at(2) << ", " << arguments.at(3) << ")" << std::endl;
		if (!client->isRegistered())
			ircserv.promoteClient(client);
	}
}
