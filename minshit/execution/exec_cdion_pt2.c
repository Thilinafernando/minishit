/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cdion_pt2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 22:02:24 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/28 23:00:25 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtout_re(char ***matrix, t_info *info)
{
	int	mat;

	mat = 0;
	while (is_redirection(matrix[mat]))
	{
		if (ft_redirections(matrix[mat], info) == -1)
			return (-2);
		if (if_in(matrix, mat) == 1 && info->fd_in_child != -420)
		{
			close(info->fd_in_child);
			info->fd_in_child = -420;
		}
		if (if_out(matrix, mat) == 1 && info->fd_out_child != -420)
		{
			close(info->fd_out_child);
			info->fd_out_child = -420;
		}
		mat++;
	}
	return (mat);
}

int	is_builtout(char ***matrix, t_info *info)
{
	int	i;
	int	mat;

	i = 0;
	mat = 0;
	while (is_redirection(matrix[mat]))
		mat++;
	if (is_builtin(matrix[mat]) == 1)
	{
		mat = builtout_re(matrix, info);
		ft_remove(matrix);
		i = builtout_process(matrix, info, mat);
	}
	else
		i = -1;
	return (i);
}

int	is_only_redirection(char ***matrix)
{
	int	i;

	i = 0;
	while (matrix[i] && matrix[i][0][0] != '|')
	{
		if (!(matrix[i][0]) ||
			!(ft_strcmp(matrix[i][0], "<") == 0 ||
			ft_strcmp(matrix[i][0], ">") == 0 ||
			ft_strcmp(matrix[i][0], "<<") == 0 ||
			ft_strcmp(matrix[i][0], ">>") == 0))
			return (-1);
		i++;
	}
	return (0);
}

void	block_only_rd(t_info *info)
{
	while (info->exec[info->mat])
	{
		ft_redirections(info->exec[info->mat], info);
		if (info->fd_in_child != -420)
			close(info->fd_in_child);
		if (info->fd_out_child != -420)
			close(info->fd_out_child);
		info->fd_in_child = -420;
		info->fd_out_child = -420;
		info->mat++;
	}
}

void	block_rd(t_info *info)
{
	info->flag = 0;
	while (is_redirection(info->exec[info->mat])
		&& info->exec[info->mat][0][0] != '|')
	{
		if (ft_redirections(info->exec[info->mat], info) == -1)
		{
			info->flag = 1;
			break ;
		}
		if (if_in(info->exec, info->mat) == 1 && info->fd_in_child != -420)
		{
			close(info->fd_in_child);
			info->fd_in_child = -420;
		}
		if (if_out(info->exec, info->mat) == 1 && info->fd_out_child != -420)
		{
			close(info->fd_out_child);
			info->fd_out_child = -420;
		}
		info->mat++;
	}
}
