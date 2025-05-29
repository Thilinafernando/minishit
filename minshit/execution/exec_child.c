/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 21:54:04 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/29 23:11:46 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	one_exec_cond(char **command, t_info *info, int **fd, char **str)
{
	if (ft_strcmp("GG", (*str)) == 0)
	{
		free((*str));
		close_fd((*fd));
		free_all(info);
		exit(127);
	}
	if (is_directory((*str)) == 1 || is_val((*str), command[0]) == -1)
	{
		free((*str));
		close_fd((*fd));
		free_all(info);
		exit(126);
	}
}

void	one_exec(char **command, t_info *info, int fd[2])
{
	char	*str;

	if (is_directory(command[0]) == 1)
	{
		close_fd(fd);
		free_all(info);
		exit(126);
	}
	str = NULL;
	str = abs_path(command[0], info);
	if (!str)
	{
		failure_command(fd, command, info);
		free_all(info);
		exit (info->exit_status);
	}
	one_exec_cond(command, info, &fd, &str);
	execve(str, command, info->env);
	failure(fd, info);
	free_all(info);
	free(str);
	exit (126);
}

void	child_pt2(t_info *info)
{
	if (info->flag == 1)
	{
		close(info->fd_in_out[0]);
		close(info->fd_in_out[1]);
		free_all(info);
		exit(info->exit_status);
	}
	if (info->fd_in_child != -420)
	{
		if (dup2(info->fd_in_child, 0) == -1)
			return (write(2, "Minishell: error dup2\n", 22)
				, free_all(info), exit(1));
		close(info->fd_in_child);
	}
}

void	child_pt1(t_info *info, int *i, int (*cpipe)[2])
{
	if ((*i) != 0 && info->prevpipe != -42)
	{
		if (dup2(info->prevpipe, 0) == -1)
			return (write(2, "Minishell: error dup2\n", 22)
				, free_all(info), exit(1));
		close(info->prevpipe);
	}
	if ((*i) != (info->count - 1))
	{
		close((*cpipe)[0]);
		if (dup2((*cpipe)[1], 1) == -1)
			return (write(2, "Minishell: error dup2\n", 22)
				, free_all(info), exit(1));
		close((*cpipe)[1]);
	}
}

void	child_block(t_info *info, int *i, int (*cpipe)[2])
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	child_pt1(info, i, cpipe);
	child_pt2(info);
	if (info->fd_out_child != -420)
	{
		if (dup2(info->fd_out_child, 1) == -1)
			return (write(2, "Minishell: error dup2\n", 22)
				, free_all(info), exit(1));
		close(info->fd_out_child);
	}
	if (is_builtin(info->exec[info->mat]))
	{
		exec_builtin(info->exec[info->mat], info);
		free_all(info);
		exit(info->exit_status);
	}
	else
	{
		close(info->fd_in_out[0]);
		close(info->fd_in_out[1]);
		one_exec(info->exec[info->mat], info, (*cpipe));
	}
}
