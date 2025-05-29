/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 01:31:10 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/28 23:01:09 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free3(char ***matrix)
{
	int	i;

	if (!matrix)
		return ;
	i = 0;
	while (matrix[i])
	{
		free_mat(matrix[i]);
		i++;
	}
	free(matrix);
}

void	free_all(t_info *info)
{
	if (info->fd_in_child != -420)
		close(info->fd_in_child);
	if (info->fd_out_child != -420)
		close(info->fd_out_child);
	info->fd_in_child = -420;
	info->fd_out_child = -420;
	free3(info->exec);
	free_mat(info->env);
	close_fd(info->fd_in_out);
	close(0);
	close(1);
	close(2);
}

void	padre(t_info *info)
{
	if (info->fd_in_child != -420)
		close(info->fd_in_child);
	if (info->fd_out_child != -420)
		close(info->fd_out_child);
	info->fd_in_child = -420;
	info->fd_out_child = -420;
	free3(info->exec);
	free_mat(info->env);
	close_fd(info->fd_in_out);
}
