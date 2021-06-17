/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmonroy- <mmonroy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 10:58:16 by mmonroy-          #+#    #+#             */
/*   Updated: 2021/06/17 12:21:34 by mmonroy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"


// Constructor + Destructor
Command::Command(std::string str, Server server, User commander) : _server(server), _commander(commander)
{
	if (!parseStr(str))
		return ;						// Parse error
	initCommands();
//	execute();						// Should I?
	return;
}

Command::~Command(void)
{
	return ;
}

// Init
void		Command::initCommands(void)
{
 	this->_commandList.insert(std::pair<std::string, commandFt>("NICK", this->ftNICK()));
 	this->_commandList.insert(std::pair<std::string, commandFt>("USER", this->ftUSER()));
 	this->_commandList.insert(std::pair<std::string, commandFt>("OPER", this->ftOPER()));
 	this->_commandList.insert(std::pair<std::string, commandFt>("QUIT", this->ftQUIT()));
 	this->_commandList.insert(std::pair<std::string, commandFt>("JOIN", this->ftJOIN()));
 	this->_commandList.insert(std::pair<std::string, commandFt>("PART", this->ftPART()));
 	this->_commandList.insert(std::pair<std::string, commandFt>("TOPIC", this->ftTOPIC()));
 	this->_commandList.insert(std::pair<std::string, commandFt>("NAMES", this->ftNAMES()));
 	this->_commandList.insert(std::pair<std::string, commandFt>("LIST", this->ftLIST()));
 	this->_commandList.insert(std::pair<std::string, commandFt>("KICK", this->ftKICK()));
}

// Parser
int		Command::parseStr(std::string str)
{
	char *aux;
	if ((aux = strtok((char*)str.c_str(), " ")) == 0)
		return (-1);							// Error, the string is empty.
	if (str[0] == ':')					// If there is a prefix. Save prefix + command.
	{
		this->_prefix = aux;
		if ((aux = strtok(0, " ")) == 0)
			return (-1);						// Error, the string doesn't have a command.
		this->_command = aux;
	}
	else									// If there isn't a prefix. Save command.
	{
		this->_prefix = nullptr;
		this->_command = aux;
	}
	int i = 0;
	while (aux != 0 && i < 5)				// While there are parameters, save them.
	{
		aux = strtok(0, "0");
		this->_params[i++] = aux;
	}
	this->_paramsNum = i;
	return (0);
}

// Execute
int			Command::execute(void)
{
	std::map<std::string, commandFt>::iterator it;
	for (it = this->_commandList.begin(); it != this->_commandList.end(); ++it)
		if (it->first == this->_command)
			return (it->second());
	if (it == this->_commandList.end())
		return (-1);						// Error, bad command.
}

// Commands
int			Command::ftNICK()
{
	// Checking if a nickname has been provided.					(ERR_NONICKNAMEGIVEN)
	if (this->_paramsNum == 0)
		return (431);

	// Checking that the nickname isn't erroneus.					(ERR_ERRONEUSNICKNAME)
	int i = 0;
	if (!isalpha(this->_params[0][i]))
		return (432);
	while (this->_params[0][++i])
		if (!isalnum(this->_params[0][i]))
			return (432);

	// Checking if the nickname isn't being used by another user.	(ERR_NICKNAMEINUSE)
	std::vector<User *>::iterator it;
	for (it = this->_server.getUsers().begin(); it != this->_server.getUsers().end(); ++it)
		if ((*it)->getNickname() == this->_params[0])
			return (433);

	// Not needed if we dont have server to server connection.		(ERR_NICKCOLLISION) (436)

	// Changing the nickname
	this->_commander.setNickname(this->_params[0]); 
	return (0);
}
commandFt		ftUSER()
{
	return (0);
}
commandFt		ftOPER()
{
	return (0);
}
commandFt		ftQUIT()
{
	return (0);
}
commandFt		ftJOIN()
{
	return (0);
}
commandFt		ftPART()
{
	return (0);
}
commandFt		ftTOPIC()
{
	return (0);
}
commandFt		ftNAMES()
{
	return (0);
}
commandFt		ftLIST()
{
	return (0);
}
commandFt		ftKICK()
{
	return (0);
}
