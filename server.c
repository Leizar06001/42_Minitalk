/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rloussig <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 11:57:55 by rloussig          #+#    #+#             */
/*   Updated: 2023/03/21 11:57:57 by rloussig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_putnbr(int nb)
{
	int	i;

	i = 0;
	if (nb > 9)
	{
		i += ft_putnbr(nb / 10);
		i += ft_putnbr(nb % 10);
	}
	else
		i += ft_putchar(nb + '0');
	return (i);
}

char	*ft_strjoin(char *s1, char c, size_t pos)
{
	char			*str;
	size_t			i;
	static size_t	buf_len;

	if (buf_len < pos + 1)
	{
		str = malloc(sizeof(char) * (buf_len + BUF_SIZE + 1));
		if (!str)
			return (0);
		i = -1;
		while (++i < buf_len)
			str[i] = s1[i];
		free(s1);
		buf_len += BUF_SIZE;
		str[pos] = c;
		str[pos + 1] = '\0';
		return (str);
	}
	s1[pos] = c;
	s1[pos + 1] = '\0';
	return (s1);
}

void	handler(int sig, siginfo_t *info, void *context)
{
	static int		bits;
	static char		c;
	static char		*buf;
	static size_t	pos;

	(void)context;
	if (sig == 10)
		c = (c << 1);
	else if (sig == 12)
		c = (c << 1) + 1;
	bits++;
	if (bits >= 8)
	{
		bits = 0;
		buf = ft_strjoin(buf, c, pos++);
		if (c == 0)
		{
			write(1, buf, pos);
			write(1, "\n", 1);
			buf[0] = '\0';
			pos = 0;
		}
		c = 0;
	}
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = handler;
	sigemptyset (&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	write(1, "Server PID : ", 13);
	ft_putnbr(getpid());
	write(1, "\n", 1);
	while (1)
	{
		usleep(10000);
	}
	return (0);
}
