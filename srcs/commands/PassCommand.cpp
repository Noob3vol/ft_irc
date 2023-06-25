#include "PassCommand.hpp"

PassCommand::PassCommand(void) :
_name("PASS")
{
	std::cout << "┣⮕ PassCommand." << std::endl;
}

PassCommand::~PassCommand(void)
{}

void PassCommand::execute(int fds, Client *client)
{
	std::string			cmd = client->getCmds().front();
	std::vector<std::string> 	args = splitArguments(cmd);
	Irc				&ircserv = Irc::getInstance();
	std::string			pass;

	if (args.empty())
		return (ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNick(), this->_name))));
	if (client->isRegistered())
		return (ircserv.addReply(Reply(fds, ERR_ALREADYREGISTERED(ircserv.getName(), client->getNick()))));


	pass = args.at(0);
	if (ircserv.getPass() != pass)
		return (ircserv.addReply(Reply(fds, ERR_PASSWDMISMATCH(ircserv.getName(), client->getNick()))));

	client->setAuth(true);
	ircserv.promoteClient(client);
	std::cout << "PassCommand::execute(" << fds << ", " << pass << ")" << std::endl;
}
