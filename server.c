/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarquit <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 19:26:22 by rmarquit          #+#    #+#             */
/*   Updated: 2022/03/28 19:26:24 by rmarquit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
		write(fd, "-2147483648", 11);
	else
	{
		if (n < 0)
		{
			ft_putchar_fd('-', fd);
			n = -n;
		}
		if (n > 9)
			ft_putnbr_fd((n / 10), fd);
		ft_putchar_fd((n % 10) + 48, fd);
	}
}

void	ft_sigaction(int sig, siginfo_t *info, void *ucontent)
{
	static int	s[8];
	static int	i;
	static int	c;
	static int	p;

	(void)ucontent;
	if (p != info->si_pid)
	{
		i = 0;
		c = 0;
	}
	if (sig == SIGUSR1)
		s[i++] = 0;
	else
		s[i++] = 1;
	if (i == 8)
	{
		c = ft_from_binary(s);
		ft_putchar_fd(c, 1);
		while (i != 0)
			s[i--] = 0;
		i = 0;
	}
	p = info->si_pid;
	kill (info->si_pid, SIGUSR1);
}

int	ft_from_binary(int a[8])
{
	int	x;
	int	i;
	int	z;

	x = 0;
	i = 0;
	z = 128;
	while (i < 8)
	{
		x += a[i] * z;
		z = z / 2;
		i++;
	}
	return (x);
}

int	main(void)
{
	struct sigaction	si_action;
	pid_t				pid;

	si_action.sa_sigaction = ft_sigaction;
	si_action.sa_flags = SA_SIGINFO;
	pid = getpid();
	write(1, "pid: ", 4);
	ft_putnbr_fd(pid, 1);
	write(1, "\n", 1);
	while (1)
	{
		sigaction(SIGUSR1, &si_action, 0);
		sigaction(SIGUSR2, &si_action, 0);
		pause();
	}
	return (0);
}
