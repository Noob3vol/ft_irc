#include "Reply.hpp"

Reply::Reply(int clientFd, std::string message):_message(message), _clientFd(clientFd)
{
}

Reply::~Reply()
{
}

int	Reply::getClientFd(void) const
{
	return (this->_clientFd);
}

std::string	const &Reply::getMessage(void) const
{
	return (this->_message);
}

void		Reply::setMessage(std::string mess)
{
	this->_message = mess;
}

int	Reply::send()
{
	size_t	len = this->_message.size();
	ssize_t	nbyte;

	std::cout << "SEND : " << this->_message << std::endl;
	nbyte = ::send(this->_clientFd, this->_message.c_str(), len, 0);
	if (nbyte == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		return (-1);
	}
	else if (static_cast<size_t>(nbyte) < len)
	{
		std::cout << "Wrong nbyte send" << std::endl;
		return (1);
	}
	return (0);
}

Reply	&Reply::operator=(Reply const &rhs)
{
	this->_clientFd = rhs.getClientFd();
	this->_message = rhs.getMessage();
	return (*this);
}

Reply	&Reply::operator+=(Reply const &rhs)
{
	this->_message += rhs.getMessage();
	return (*this);
}
