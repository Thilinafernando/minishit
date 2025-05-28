/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refresh_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 22:55:28 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/28 23:02:27 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_refresh_fd(t_info *info)
{
	dup2(info->fd_in_out[0], STDIN_FILENO);
	dup2(info->fd_in_out[1], STDOUT_FILENO);
	close(info->fd_in_out[0]);
	close(info->fd_in_out[1]);
}
