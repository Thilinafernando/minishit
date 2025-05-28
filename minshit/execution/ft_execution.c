/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:29:40 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/28 23:00:50 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execution_half(t_info *info, int (*cpipe)[2], int *i)
{
	if (is_only_redirection(info->exec) == 0)
	{
		block_only_rd(info);
		return (-1);
	}
	if (info->count == 1)
	{
		info->flag = is_builtout(info->exec, info);
		if (info->flag == -2)
		{
			ft_refresh_fd(info);
			return (-1);
		}
		if (info->flag == 0)
			return (-1);
	}
	if ((*i) != (info->count - 1))
	{
		if (pipe((*cpipe)) == -1)
		{
			write(2, "Minishell: error pipe\n", 22);
			return (estat(1, info), -2);
		}
	}
	return (0);
}

void	ft_execution(t_info *info)
{
	int		i;
	int		f;
	int		cpipe[2];
	pid_t	pid;

	i = 0;
	init_exectution(&cpipe, info);
	while (i < info->count)
	{
		f = execution_half(info, &cpipe, &i);
		if (f == -1)
			break ;
		if (f == -2)
			return ;
		block_rd(info);
		ft_remove(info->exec);
		if (!info->exec[info->mat] || info->exec[info->mat][0][0] == '|')
			break ;
		fork_block(info, &pid, &cpipe, &i);
		i++;
	}
	final_block(info);
}
