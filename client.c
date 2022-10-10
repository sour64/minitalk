/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarquit <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 19:26:38 by rmarquit          #+#    #+#             */
/*   Updated: 2022/03/28 19:26:41 by rmarquit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static int	g_a = 0;

int	ft_atoi(const char *str)
{
	unsigned long			i;
	int						m;
	unsigned long			res;

	res = 0;
	i = 0;
	m = 1;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\r' || str[i] == '\v' || str[i] == '\f')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			m = -m;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
		res = res * 10 + str[i++] - '0';
	if (res >= 2147483649 && m == -1)
		return (0);
	else if (res >= 2147483648 && m == 1)
		return (-1);
	return (res * m);
}

static void	ft_confirm(int signal)
{
	if (signal == SIGUSR1)
		write (1, "я сияю ярко\n", 30);
	g_a = 1;
}

void	ft_send(unsigned char c, pid_t pid)
{
	int	s[8];
	int	i;

	i = 7;
	while (c > 0)
	{
		if (c % 2 == 1)
			s[i--] = 1;
		else
			s[i--] = 0;
		c = c / 2;
	}
	while (i >= 0)
		s[i--] = 0;
	while (++i < 8)
	{
		usleep(100);
		if (s[i] == 0)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		while (g_a != 1)
			usleep(1);
		g_a = 0;
	}
}

int	main(int argc, char **argv)
{
	pid_t	pid;
	int		i;

	i = 0;
	if (argc != 3)
	{
		write (1, "Error! Enter \"PID\" \"You message\"\n", 33);
		return (0);
	}
	pid = ft_atoi(argv[1]);
	if (pid <= 0)
	{
		write (1, "Error! Wrong PID!\n", 18);
		return (0);
	}
	signal(SIGUSR1, ft_confirm);
	while (argv[2][i])
	{
		ft_send (argv[2][i], pid);
		i++;
	}
}
