/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <tkurukul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:37:00 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/24 22:51:11 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	in_out_1(int pipefd[2])
{
	if (dup2(pipefd[1], 1) == -1)
	{
		perror("FAILED DUP2 1");
		return(-1);
	}
	close(pipefd[1]);
	return (0);
}

void	close_fd(int *ar)
{
	if (ar)
	{
		if (ar[0] >= 0)
			close(ar[0]);
		if (ar[1] >= 0)
			close(ar[1]);
	}
}

void	failure(int fd[2], t_info *info)
{
	close_fd(fd);
	estat(1, info);
	write(2, "minishell: execve failed!\n", 26);
}
void	failure_command(int fd[2], char **str, t_info *info)
{
	write(2, "minishell: ", 11);
	write(2, *str, ft_strlen(*str));
	write(2, ": command not found\n", 20);
	estat(127, info);
	close_fd(fd);
}
