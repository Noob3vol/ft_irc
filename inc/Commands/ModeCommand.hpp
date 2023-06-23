#ifndef MODECOMMAND_HPP
# define MODECOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include <sstream>

#define RPL_UMODEIS(server, user, modes) (":" + server + "221 "+ user + " :+" + modes)
#define RPL_CHANNELMODIS(server, user, channel, modes, modsarg) (":" + server + " 324 " + user + " " + channel + " " + modes + " " + modsarg)

#define ERR_NOSUCHNICK(server, user) (":" + server + " 401 " + user + " :No such nick")
#define ERR_NOSUCHCHANNEL(server, user, channel) (":" + server + " 403 " + user + " " + channel + " :No such channel")

#define ERR_CHANOPRIVSNEEDED(server, user, channel) ( ":" + server + " 482 " + user + " " + channel + " :You'not channel operator")

#define ERR_USERSDONTMATCHVIEW(server, user) (":" + server + " 502 " + user + " :Cant view mode for other users\r\n")
#define ERR_USERSDONTMATCH(server, user) (":" + server + " 502 " + user + " :Cant change mode for other users\r\n")


#define ADDING 1
#define REMOVING 2



class ModeCommand : public Command
{
	private:
		std::string _name;
	public:
		ModeCommand();
		virtual ~ModeCommand();
		void execute(int fds, Client *client);
		void	executeUserMode(int fds, User *user, std::vector<std::string> &args);
		void	printUserMode(int fds, User *user, std::string nickname);
		void	changeUserMode(int fds, User *user, Client *target, std::vector<std::string> &args);
		void	executeChannelMode(int fds, User *user, std::vector<std::string> &args);
		void	printChannelMode(int fds, User *user, Channel *chan);
};

#endif
