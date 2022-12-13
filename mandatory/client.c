/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleite-s < rleite-s@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 21:33:52 by rleite-s          #+#    #+#             */
/*   Updated: 2022/12/12 19:14:44 by rleite-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	kill_handle(int pid, int signal)
{
	if (kill(pid, signal))
	{
		write(1, "Error!\n", 1);
		exit(1);
	}
}

int	ft_atoi(const char *c)
{
	short int	signal;
	int			num;

	num = 0;
	while (*c == ' ' || *c == '\f' || *c == '\n' || *c == '\r' || *c == '\t'
		|| *c == '\v')
		c++;
	signal = (*c == '+' || (*c >= '0' && *c <= '9')) - (*c == '-');
	c += (*c == '+' || *c == '-');
	while (*c >= '0' && *c <= '9')
		num = (num * 10) + (*c++ - '0');
	return (num * signal);
}

void	send_message(int server_id, char *message)
{
	static char	*message_copy;
	static int	letter;
	static int	bit;
	static int	server_id_copy;

	if (!message_copy)
		message_copy = message;
	if (!server_id_copy)
		server_id_copy = server_id;
	if (message_copy[letter] >> bit++ & 1)
		kill_handle(server_id_copy, SIGUSR1);
	else
		kill_handle(server_id_copy, SIGUSR2);
	if (bit == 7)
	{
		bit = 0;
		letter++;
	}
	usleep(100);
}

void	next_signal(int signal, siginfo_t *info, void *ucontext)
{	
	if (signal == SIGUSR1)
		send_message(info->si_pid, (char *) ucontext);
	else
		exit(0);
}

int	main(int argc, char **argv)
{
	struct sigaction	action;

	if (argc != 3)
	{
		write(1, "Error!\nPlease, inform just the 2 arguments: ", 44);
		write(1, "[PID of the server] [The message you want]\n", 43);
		return (1);
	}
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	action.sa_sigaction = next_signal;
	if (sigaction(SIGUSR1, &action, NULL))
		write(1, "Fail to use sigaction in SIGUSR1 signal.\n", 41);
	if (sigaction(SIGUSR2, &action, NULL))
		write(1, "Fail to use sigaction in SIGUSR2 signal.\n", 41);
	if (ft_atoi(argv[1]) <= 0)
		write(1, "INVALID PID!\n", 13);
	send_message(ft_atoi(argv[1]), argv[2]);
	while (1)
		pause();
	return (0);
}
