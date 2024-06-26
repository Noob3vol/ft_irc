#include "NickCommand.hpp"

NickCommand::NickCommand(void) :
_name("NICK")
{
	std::cout << "┣⮕ NickCommand." << std::endl;
}

NickCommand::~NickCommand(void)
{}

bool
	NickCommand::cantExecute(Client *client)
{
	(void)client;
	return (true);
}

void NickCommand::execute(int fds, Client *client)
{
	std::string	cmd = client->getCmds().front();
	std::string	nick = cmd.substr(cmd.find(" ") + 1);
	Irc		&ircserv = Irc::getInstance();

	if (nick.empty())
	{
		ircserv.addReply(Reply(fds, ERR_NONICKNAMEGIVEN(ircserv.getName(), client->getUsername())));
		return ;
	}
	if (ircserv.getUserByNick(nick))
	{
		ircserv.addReply(Reply(fds, ERR_NICKNAMEINUSE(ircserv.getName(), client->getUsername(), nick)));
		return ;
	}
	if (nick.length() > 9 || nick.length() < 2)
	{
		ircserv.addReply(Reply(fds, ERR_ERRONEUSNICKNAME(ircserv.getName(), client->getUsername(), nick)));
		return ;
	}
	ircserv.addReply(Reply(fds, RPL_NICK(user_id(ircserv.getName(), client->getNickname(), client->getUsername()), nick)));
	client->setNick(nick);
	std::cout << "NickCommand::execute(" << fds << ", " << client->getNickname() << ")" << std::endl;
	if (!client->isRegistered())
		ircserv.promoteClient(client);
}
