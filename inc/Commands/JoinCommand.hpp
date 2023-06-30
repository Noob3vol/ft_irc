#ifndef JOINCOMMAND_HPP
# define JOINCOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"
# include "Channel.hpp"

#define RPL_JOIN(userid, nick, channel) (userid + " " + nick + " JOIN :" +  channel + "\r\n")
#define ERR_BANNEDFROMCHAN(server, client, channel) (":" + server + " 474 " + client + " " + channel + " :Cannot join channel (+b)\r\n")
#define ERR_BADCHANMASK(server, client, channel) (":" + server + " 476 " + client + " " + channel + " :Bad Channel Mask\r\n")
# define ERR_BADCHANNELKEY(server, client, channel) (":" + server + " 475 " + client + " " + channel + " :Cannot join channel (+k)\r\n")


class User;
class Channel;

class JoinCommand : public Command
{
	private:
		std::string _name;

		void	rplJoin(int fds, User *user, Channel *chan);
		std::map<std::string, std::string>	parseArg(std::vector<std::string> &args);
		void								_joinChannel(int fds, Client *client, std::map<std::string , std::string> channels);
		bool								_chanIsValid(const std::string &name);
		bool								_passwordIsValid(std::string &password);

	public:
		JoinCommand();
		virtual ~JoinCommand();
		void execute(int fds, Client *client);
};

#endif
