/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pt2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 21:59:05 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/30 18:50:33 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_count_blocks(char ***exec, int *i, int *count, int *cmd_rd)
{
	if (exec[(*i)][0][0] == '|')
	{
		if ((*cmd_rd))
			(*count)++;
		(*cmd_rd) = 0;
	}
	else if (ft_isalpha(exec[(*i)][0][0])
		|| ft_isascii(exec[(*i)][0][0]) || is_redirection(exec[(*i)]))
		(*cmd_rd) = 1;
	(*i)++;
}

int	count_exec_blocks(char ***exec)
{
	int	i;
	int	count;
	int	cmd_rd;

	i = 0;
	count = 0;
	cmd_rd = 0;
	while (exec[i])
	{
		if (!exec[i][0])
		{
			i++;
			continue ;
		}
		process_count_blocks(exec, &i, &count, &cmd_rd);
	}
	if (cmd_rd)
		count++;
	return (count);
}

void	init_exectution(t_info *info)
{
	info->cpipe[0] = -1;
	info->cpipe[1] = -1;
	info->fd_in_out[0] = dup(STDIN_FILENO);
	info->fd_in_out[1] = dup(STDOUT_FILENO);
	info->count = count_exec_blocks(info->exec);
	info->pid_counts = 0;
	info->prevpipe = -42;
	info->mat = 0;
	info->flag = 0;
}

void	fork_block(t_info *info, pid_t *pid, int *i)
{
	(*pid) = fork();
	if ((*pid) == -1)
	{
		padre(info);
		write(2, "Minishell: error fork\n", 22);
		exit(1);
	}
	if ((*pid) == 0)
		child_block(info, i);
	else
		parent_block(info, i, (*pid));
}

void	final_block(t_info *info)
{
	wait_block(info);
	ft_refresh_fd(info);
	free3(info->exec);
	close_fd(info->fd_in_out);
}
