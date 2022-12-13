/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleite-s < rleite-s@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 21:33:41 by rleite-s          #+#    #+#             */
/*   Updated: 2022/12/12 19:07:01 by rleite-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	check_exit(char letter)
{
	static char	text[5];
	static int	position;

	if (!position)
		text[4] = 'Y';
	if (position < 5)
		text[position++] = letter;
	if (letter == '\0')
	{
		position = 0;
		if (((text[0] == 'e' && text[1] == 'x' && text[2] == 'i' && \
		text[3] == 't') || (text[0] == 's' && text[1] == 'a' && text[2] == 'i' \
		&& text[3] == 'r')) && text[4] == '\0')
			exit(0);
	}
}

void	ft_putnbr(int n)
{
	if (n > 9)
		ft_putnbr(n / 10);
	write(1, &"0123456789"[n % 10], 1);
}

void	kill_handle(int pid, int signal)
{
	if (kill(pid, signal))
	{
		write(1, "Error while trying send signal!\n", 32);
		exit(1);
	}
}

void	receive_bits(int signal, siginfo_t *info, void *ucontext)
{
	static char	letter;
	static char	bit;
	int			client_id;

	if (signal == SIGUSR1)
		letter += 1 << bit;
	client_id = info->si_pid;
	if (bit == 6)
	{
		write(1, &letter, 1);
		if (letter == 0)
		{
			write(1, "\n", 1);
			kill_handle(client_id, SIGUSR2);
		}
		else
			kill_handle(client_id, SIGUSR1);
		check_exit(letter);
		letter = 0;
		bit = 0;
		return ;
	}
	bit++;
	kill_handle(client_id, SIGUSR1);
	(void) ucontext;
}

int	main(void)
{
	struct sigaction	server_action;

	sigemptyset(&server_action.sa_mask);
	server_action.sa_flags = SA_SIGINFO;
	server_action.sa_sigaction = receive_bits;
	write(1, "Server pid: ", 12);
	ft_putnbr(getpid());
	write(1, "\n", 1);
	if (sigaction(SIGUSR1, &server_action, NULL))
		write(1, "Fail to use sigaction in SIGUSR1 signal.\n", 41);
	if (sigaction(SIGUSR2, &server_action, NULL))
		write(1, "Fail to use sigaction in SIGUSR2 signal.\n", 41);
	while (1)
		pause();
	return (0);
}
