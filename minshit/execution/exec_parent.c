/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parent.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 21:56:50 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/28 23:00:41 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parent_block(t_info *info, int *i, pid_t pid, int (*cpipe)[2])
{
	info->pids[info->pid_counts++] = pid;
	while (info->exec[info->mat])
	{
		if (info->exec[info->mat][0][0] == '|')
		{
			info->mat++;
			break ;
		}
		info->mat++;
	}
	if (info->prevpipe != -42)
		close(info->prevpipe);
	if ((*i) != (info->count -1))
	{
		close((*cpipe)[1]);
		info->prevpipe = (*cpipe)[0];
	}
	if (info->fd_in_child != -420)
		close(info->fd_in_child);
	if (info->fd_out_child != -420)
		close(info->fd_out_child);
	info->fd_in_child = -420;
	info->fd_out_child = -420;
}

void	fail_wait(t_info *info)
{
	write(2, "Minishell: error waitpid\n", 25);
	padre(info);
	exit(1);
}

void	wait_block(t_info *info)
{
	int	j;
	int	status;

	j = 0;
	signal(SIGINT, SIG_IGN);
	while (j < info->pid_counts)
	{
		if (waitpid(info->pids[j], &status, 0) == -1)
			fail_wait(info);
		if (info->pids[j] == (info->pids[info->pid_counts - 1]))
		{
			if (WIFEXITED(status))
				info->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGINT)
					info->exit_status = 128 + WTERMSIG(status);
				else if (WTERMSIG(status) == SIGQUIT)
					write(2, "Quit (core dumped)\n", 19);
				info->exit_status = 128 + WTERMSIG(status);
			}
		}
		j++;
	}
}
