/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 22:13:33 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/28 23:01:23 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	checks(char **args, int i, int flag)
{
	if ((ft_strncmp(args[i], "-n", 2) != 0))
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		flag = 0;
	}
	if ((ft_strncmp(args[i], "-n", 2) == 0) && flag != 1)
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
	}
	return (flag);
}

int	ft_echo(char **args, t_info *info)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	if (!args[1])
		return (write(1, "\n", 1), 0);
	while (args[i])
	{
		if (ft_strncmp(args[1], "-n", 2) != 0 && i == 1)
		{
			write(1, args[i], ft_strlen(args[i]));
			if (args[i + 1])
				write(1, " ", 1);
		}
		else if ((ft_strncmp(args[1], "-n", 2) == 0 && i == 1))
			flag = 1;
		if (i > 1)
			flag = checks(args, i, flag);
		i++;
	}
	if (args[1] && ft_strncmp(args[1], "-n", 2) != 0)
		write(1, "\n", 1);
	return (estat(0, info), 0);
}
