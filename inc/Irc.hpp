#pragma once
#include "ft_irc.hpp"

#include	"Command.hpp"
#include 	"AwayCommand.hpp"
#include	"CapCommand.hpp"
#include	"InviteCommand.hpp"
#include	"JoinCommand.hpp"
#include	"KickCommand.hpp"
#include	"ModeCommand.hpp"
#include	"NickCommand.hpp"
#include	"PartCommand.hpp"
#include	"PassCommand.hpp"
#include	"PingCommand.hpp"
#include	"PrivMsgCommand.hpp"
#include	"QuitCommand.hpp"
#include	"TopicCommand.hpp"
#include	"UserCommand.hpp"
#include	"WhoCommand.hpp"

class Command;
class Client;
class User;
class Channel;
class Reply;

class	Irc {
	Command *command;
	public:
		//Const & destr
		static Irc &getInstance(std::string port = "ircd", std::string pass = "123", std::string name = "ircserv") // Init and retrieve server instance
		{
			static Irc	instance(port, pass, name);
			return instance;
		}
		~Irc();

		//Utils
		void			printAi() const;

		//Setter
		void			addClient(int const &sfd);
		void			removeClient(int const &sfd, std::string const &reason);

		//Getter
		const	int		&getSocket() const;
		const	struct addrinfo	*getAi() const;
		const	std::string	&getName() const;
		const	std::string	&getPass() const;
		std::map<int, Client *>	&getClients();
		User	 		*getUserByNick(std::string const nick) const;


		int	computeFdMax() const;
		int	main_loop();


		// Command manager
		void					initCommand();
		std::map<std::string, Command*>		getCommandList();
		// Reply
		void			addReply(Reply reply);


		// Channel Manager
		std::map<std::string, Channel *>	&getChannelList(void);
		Channel					*getChannel(std::string name);
		Channel					*addChannel(std::string name);
		void					addUserToChannel(User *user, Channel *chan);
		bool					channelExists(std::string name);
		void					removeChannel(Channel *channel);


		//Client Manager
		void		promoteClient(Client *client);
		void		addLeaveUser(Client *client, std::string const &reason);
		void		leaveUsers();

	private:
		// Const & destr
		Irc(std::string port, std::string passwdm, std::string name);


		// Private Copy and assignement to enforce singularity
		Irc(const Irc &);
		Irc &operator=(const Irc &);


		// Client, User 
		std::map<std::string, Channel *>	_channels;
		std::map<int, Client *>				_clients;
		std::map<int, std::string>			_clientsWantLeave;


		// Client Management
		int			setup_fds();


		//Commands
		std::map<std::string, Command*>	commandList;


		// Server info
		struct	addrinfo	*_net;
		std::string			_name;
		std::string			_pass;
		std::string			_oper_pass;
		int					_sockfd;

		// Receiving data
		int			accept_client();
		int			receive_client_data(Client *user);
		int			data_reception_handler();
		int			manage_incoming_connection();
		int			retrieve_clients_packet();
		int			read_client_socket(Client &user);
		int			manageCommand();

		// Send data
		std::vector<Reply>	_replies;
		void			mergeReplies();
		Reply			mergeReply();
		int				sendReplies();

		// Setup Server interface
		void			setupAddrInfo(std::string port);

		// Setup socket interface
		int			setup_socket();
		int			set_socket_option();

		// Operation on fds
		void		setupFds();
		fd_set		fds[4];

		// Utils
		bool		_isClientWantLeave(Client *client);

		void		_removeAllChannel();
		void		_removeAllClient();
		void		_removeAllCommands();
};
