/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:52:49 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/28 23:23:51 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**find_path(char **envp)
{
	int		i;
	char	*str;
	char	**matrix;

	str = NULL;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			str = (envp[i] + 5);
			break ;
		}
		i++;
	}
	if (!str)
		return (NULL);
	matrix = ft_split(str, ':');
	return (matrix);
}

char	*build_full(char *path, char *command)
{
	char	*str;
	char	*tmp;

	tmp = ft_strjoin(path, "/");
	str = ft_strjoin(tmp, command);
	free (tmp);
	return (str);
}

int	is_directory(const char *path)
{
	DIR	*dir;

	if (!path)
		return (0);
	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		write(2, "Minishell: ", 11);
		write(2, path, ft_strlen(path));
		write(2, ": Is a directory\n", 17);
		return (1);
	}
	return (0);
}

char	*abs_path(char *command, t_info *info)
{
	int		i;
	char	**matrix;
	char	*str;

	i = 0;
	matrix = find_path(info->env);
	if (!matrix)
		return (NULL);
	while (matrix[i])
	{
		if (i == 0)
			str = ft_strdup(command);
		else
			str = build_full(matrix[i], command);
		if (access(str, X_OK) == 0)
			return (free_mat(matrix), str);
		i++;
		free(str);
	}
	free_mat(matrix);
	return (NULL);
}
