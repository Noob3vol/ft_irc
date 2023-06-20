#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <utility>

# define ERR_NEEDMOREPARAMS(serv, nickname, command) (":" + serv + " 461 " + nickname + " " + command + " :Not enough parameters.\r\n")
# define ERR_ALREADYREGISTERED(serv, nickname) (":" + serv + " 462 " + nickname + " :You may not reregister.\r\n")


class Client;
class Reply;

class Command
{
protected:
	std::string _name;

public:

	Command();
	virtual ~Command();
	virtual void execute(int fds, Client *client) = 0;
};

std::vector<std::string> splitArguments(const std::string &cmd);

#endif
