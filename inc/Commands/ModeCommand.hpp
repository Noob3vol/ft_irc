#ifndef MODECOMMAND_HPP
# define MODECOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include <sstream>

# define MODES "oklit"
# define MODES_WARG "okl"

# define ERR_USERSDONTMATCHVIEW(server, user) (":" + server + " 502 " + user + " :Cant view mode for other users\r\n")
# define ERR_USERSDONTMATCH(server, user) (":" + server + " 502 " + user + " :Cant change mode for other users\r\n")

# define RPL_MODE(userID, channel, mode) (userID + " MODE " + channel + " " + mode + "\r\n")
# define RPL_MODEWITHARG(userID, channel, mode, param) (userID + " MODE " + channel + " " + mode + " :" + param + "\r\n")

//#define ERR_INVALIDMODEPARAM(server, user,)


#define REMOVING 0
#define ADDING 1


class ModeCommand : public Command
{
	private:
		std::string _name;

	//	void	executeUserMode(int fds, User *user, std::vector<std::string> &args);
	//	void	printUserMode(int fds, User *user, std::string nickname);
	//	void	applyUserMode(int fds, User *user, Client *target, std::string &modstr, std::vector<std::string> *modvalue);
		void	applyChannelMode(User *user, Channel *chan, std::string &modstr, std::vector<std::string> &modvalue);
		void	appChannelMode(char mode, int modmode, Channel *chan, std::string *arg);
		void	executeChannelMode(int fds, User *user, std::vector<std::string> &args);
		int	checkOperMode(int fds, User *user, std::string &target);
		void	printChannelMode(int fds, User *user, Channel *chan);
		bool			is_mode(char c);
		bool			needArg(char mode, bool modmode);
		int			checkMode(char mode, bool modmode, User *user, Channel *chan, std::string arg);

		void	printUserMode(int fds, User *user, std::string nickname);
		void	executeUserMode(int fds, User *user, std::vector<std::string> &args);

	public:
		ModeCommand();
		virtual ~ModeCommand();
		virtual void execute(int fds, Client *client);
		bool cantExecute(Client *client);
};


#endif
