/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rloussig <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 11:58:04 by rloussig          #+#    #+#             */
/*   Updated: 2023/03/21 11:58:05 by rloussig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

char	g_received;

char	*int_to_bin(unsigned char nb)
{
	char	*bbyte;
	int		i;
	char	tmp;

	bbyte = malloc(sizeof(char) * 9);
	if (!bbyte)
		return (0);
	i = 0;
	while (i < 8)
	{
		bbyte[i++] = (nb % 2) + '0';
		nb /= 2;
	}
	bbyte[i] = '\0';
	i = 0;
	while (i < 4)
	{
		tmp = bbyte[i];
		bbyte[i] = bbyte [7 - i];
		bbyte[7 - i++] = tmp;
	}
	return (bbyte);
}

void	send_byte(int pid, char *bbyte)
{
	int	i;

	i = -1;
	while (++i < 8)
	{
		while (g_received == 0)
		{
			if (bbyte[i] == '0')
				kill(pid, SIGUSR1);
			else
				kill(pid, SIGUSR2);
			usleep(10000);
		}
		g_received = 0;
	}
}

void	process_text(int srv_pid, char *msg)
{
	int		n;
	char	*bbyte;

	n = -1;
	while (msg[++n])
	{
		bbyte = int_to_bin((unsigned char)msg[n]);
		send_byte(srv_pid, bbyte);
		free(bbyte);
	}
	bbyte = int_to_bin(0);
	send_byte(srv_pid, bbyte);
	free(bbyte);
}

void	handler(int sig, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	(void)sig;
	g_received = 1;
}

int	main(int argc, char *argv[])
{
	struct sigaction	sa;

	if (argc < 3)
	{
		write(1, "Arguments missing\n", 18);
		return (0);
	}
	else if (argc > 3)
	{
		write(1, "Too many arguments\n", 19);
		return (0);
	}
	g_received = 0;
	sa.sa_sigaction = handler;
	sigemptyset (&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
	write(1, "Sending...\n", 11);
	process_text(ft_atoi(argv[1]), argv[2]);
	return (0);
}
