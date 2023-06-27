#include "JoinCommand.hpp"

JoinCommand::JoinCommand(void) :
_name("JOIN")
{
	std::cout << "┣⮕ JoinCommand." << std::endl;
}

JoinCommand::~JoinCommand(void)
{}

std::map<std::string , std::string>
	JoinCommand::_parseCommand(int fds, Client *client, std::string str) {
	std::map<std::string , std::string>	channels;
	std::vector<std::string>			splitArgs;
	std::vector<std::string>			channelName;
	std::vector<std::string>			keys;
	Irc									&ircserv =	Irc::getInstance();

	splitArgs = this->splitStr(str, ' ');
	std::vector<std::string>::iterator it = splitArgs.begin();

	channelName = this->splitStr(*it, ',');
	for (size_t i = 0; i < channelName.size(); i++)
	{
		if (!this->_chanIsValid(channelName[i])) {
			ircserv.addReply(Reply(fds, ERR_BADCHANMASK(ircserv.getName(), client->getNick(), channelName[i])));
			throw (std::exception());
		}
	}
	if (it != splitArgs.end())
	{
		it++;
		keys = this->splitStr(*it, ',');
		for (size_t i = 0; i < keys.size(); i++)
		{
			if (!this->_passwordIsValid(keys[i])) {
				ircserv.addReply(Reply(fds, ERR_BADCHANNELKEY(ircserv.getName(), client->getNick(), channelName[i])));
				throw (std::exception());
			}
		}
	} else {
		for (size_t i = 0; i < channelName.size(); i++)
			keys.push_back("x");
	}
	for (size_t i = 0; i < channelName.size(); i++)
		channels[channelName[i]] = keys[i];

	return (channels);
}

void
	JoinCommand::rplJoin(int fds, User *user, Channel *chan)
{
	Irc &ircserv = Irc::getInstance();

	ircserv.addReply(Reply(fds, RPL_JOIN(ircserv.getName(), user->getNick(), chan->getName())));
	ircserv.addReply(Reply(fds, RPL_TOPIC(ircserv.getName(), user->getNick(), user->getUser(), chan->getTopic())));
	ircserv.addReply(Reply(fds, RPL_NAMREPLY(ircserv.getName(), user->getNick(), "=", chan->getName(), chan->getMemberList())));
	ircserv.addReply(Reply(fds, RPL_ENDOFNAMES(ircserv.getName(), user->getNick(), chan->getName())));

}

void
	JoinCommand::_joinChannel(int fds, Client *client, std::map<std::string , std::string> channels)
{
	Irc			&ircserv =	Irc::getInstance();
	User		*user = ircserv.getUserByNick(client->getNick());

	if (user == NULL)
		return ;
	for (std::map<std::string , std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		std::cout << "Joining " << it->first << " with key " << it->second << std::endl;
		if (ircserv.channelExists(it->first))
		{
			Channel *channel = ircserv.getChannel(it->first);
			std::cout << "Channel" << channel->getName() << " exists" << std::endl;
			if (channel->isInvit() && !user->isInvited(channel))
				return (ircserv.addReply(Reply(fds, ERR_INVITEONLYCHAN(ircserv.getName(), client->getNick(), channel->getName()))));
			if (!channel->getKey().empty() && channel->getKey() != it->second)
				return (ircserv.addReply(Reply(fds, ERR_BADCHANNELKEY(ircserv.getName(), client->getNick(), channel->getName()))));
			user->addChannel(channel);
			channel->addUser(user);
			rplJoin(fds, user, channel);
		}
		else
		{
			std::cout << "Channel" << it->first << " doesn't exists" << std::endl;
			Channel *channel = ircserv.addChannel(it->first);
			if (channel == NULL) {
				ircserv.addReply(Reply(fds, ERR_BADCHANMASK(ircserv.getName(), client->getNick(), it->first)));
				return ;
			}
			if (it->second != "x")
				channel->setKey(it->second);
			user->addChannel(channel);
			channel->addOper(user);
			rplJoin(fds, user, channel);
		}
	}
}

std::map<std::string, std::string>	JoinCommand::parseArg(std::vector<std::string> &args)
{
	std::map<std::string, std::string>	channels;
	std::vector<std::string>		chans = splitStr(args.at(0), ',');
	std::pair<std::string, std::string>	chans_pair;
	std::vector<std::string>		key;
	args.erase(args.begin());
	if (!args.empty())
		key = splitStr(args.at(0), ',');

	std::vector<std::string>::iterator	chans_beg = chans.begin();
	std::vector<std::string>::iterator	chans_end = chans.end();
	while (chans_beg != chans_end)
	{
		if (key.empty())
			chans_pair = make_pair(*chans_beg, "x");
		else
		{	
			chans_pair = make_pair(*chans_beg, *key.begin());
			key.erase(key.begin());
		}
		channels.insert(chans_pair);
		chans_beg++;
	}
	return (channels);
}

void JoinCommand::execute(int fds, Client *client)
{
	Irc					&ircserv =	Irc::getInstance();
	std::string				cmd =	client->getCmds().front();
	std::vector<std::string>		args = splitArguments(cmd);
	std::map<std::string, std::string>	channels;

	if (args.empty())
	{
		return (ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNick(), this->_name))));
	}
	else if (args.at(0) == "0")
	{
		std::cout << "Leave all channel for " << client->getNick() << std::endl;
	} 
	else
	{
		try {
			channels = parseArg(args);
			if (!channels.empty())
				this->_joinChannel(fds, client, channels);
		} catch (std::exception &e) {
			return ;
		}
	}
}

//	void
//		JoinCommand::execute(int fds, Client *client)
//	{
//		std::string	cmd =	client->getCmds().front();
//		std::string	arg =	cmd.substr(cmd.find(" ") + 1);
//		Irc			&ircserv =	Irc::getInstance();
//		
//		if (arg == cmd)
//			arg.clear();
//		if (!arg.empty() && arg[0] == '0')
//			std::cout << "Leave all channel for " << client->getNick() << std::endl;
//		else if (arg.empty())
//		{
//			ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNick(), this->_name)));
//			return ;
//		} else {
//			try {
//				std::map<std::string , std::string> channels = this->_parseCommand(fds, client, arg);
//				if (!channels.empty())
//					this->_joinChannel(fds, client, channels);
//			} catch (std::exception &e) {
//				return ;
//			}
//		}
//	}

/*
 ** #foo -> Public chan - Anyone can join.
 ** &foo -> Private chan - Only invited users can join.
 */

	bool
JoinCommand::_chanIsValid(const std::string &name)
{
	if (name.empty())
		return (false);
	if (name[0] != '#' && name[0] != '&')
		return (false);
	return (true);
}

bool JoinCommand::_passwordIsValid(std::string &password) {
	if (password.empty())
		return (false);
	if (password[0] == ':')
		password.erase(password.begin());
	if (password == "x")
		return (true);
	if (password.size() > 50 || password.size() < 3)
		return (false);
	return (true);
}
