/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <tkurukul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 22:04:01 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/24 22:53:14 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// if open fails, estat to the right number

#include "minishell.h"

int	ft_input(char **exec, t_info *info)
{
	int	fd;

	fd = open(exec[1], O_RDONLY);
	if (fd == -1)
	{
		write(2, "Minishell: ", 11);
		write(2, exec[1], ft_strlen(exec[1]));
		write(2, ": ", 2);
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2, "\n", 1);
		if (errno == ENOENT)
			return (estat(127 , info), -1);
		else if (errno == EACCES)
			return (estat(126 , info), -1);
		else
			return (estat(1 , info), -1);
	}
	info->fd_in_child = fd;
	return (0);
}

int	ft_output(char **exec, t_info *info)
{
	int	fd;

	fd = open(exec[1], O_WRONLY | O_CREAT| O_TRUNC, 0644);
	if (fd == -1)
	{
		write(2, "Minishell: ", 11);
		write(2, exec[1], ft_strlen(exec[1]));
		write(2, ": ", 2);
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2, "\n", 1);
		if (errno == ENOENT)
			return (estat(127 , info), -1);
		else if (errno == EACCES)
			return (estat(126 , info), -1);
		else
			return (estat(1 , info), -1);
	}
	info->fd_out_child = fd;
	return (0);
}

int	ft_append(char **exec, t_info *info)
{
	int	fd;

	fd = open(exec[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		write(2, "Minishell: ", 11);
		write(2, exec[1], ft_strlen(exec[1]));
		write(2, ": ", 2);
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2, "\n", 1);
		if (errno == ENOENT)
			return (estat(127 , info), -1);
		else if (errno == EACCES)
			return (estat(126 , info), -1);
		else
			return (estat(1 , info), -1);
	}
	info->fd_out_child = fd;
	return (0);
}

char	*heredoc_filename(void)
{
	char	*pid;
	char	*filename;
	char	*tmp;

	pid = ft_itoa(getpid());
	if (!pid)
		return (NULL);
	tmp = ft_strjoin("/tmp/.heredoc_", pid);
	free(pid);
	if (!tmp)
		return (NULL);
	filename = ft_strjoin(tmp, ".tmp");
	free(tmp);
	return (filename);
}



void	ft_heredoc_process(char **exec, t_info *info, char *filename)
{
	char	*str;
	char	*dollar;
	int		fd;
	int		std_in;


	dollar = NULL;
	std_in = open("/dev/tty", O_RDONLY);
	if (std_in == -1)
	{
		write(2, "Minishell: cannot open /dev/tty\n", 32);
		return (free(filename), exit(1));
	}
	if (dup2(std_in, STDIN_FILENO) == -1)
	{
		write(2, "Minishell: dup2 failed\n", 23);
		close(std_in);
		free(filename);
		exit(1);
	}
	close(std_in);
	fd = open(filename, O_CREAT| O_TRUNC | O_WRONLY, 0644);
	if (fd == -1)
	{
		write(2, "Minishell : ", 12);
		write(2, filename, ft_strlen(filename));
		write(2, ": ", 2);
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2, "\n", 1);
		return (free(filename), exit(126));
	}
	while (1)
	{
		str = readline("> ");
		if (!str || !exec[1] || ft_strcmp(str, exec[1]) == 0)
		{
			if (!str && signal_status != 130)
			write(2, "Minishell: warning: here-document delimited by end-of-file (wanted ", 67);
			write(2, exec[1], ft_strlen(exec[1]));
			write(2, ")\n", 2);
			free(str);
			free(filename);
			close(fd);
			free_all(info);
			if (signal_status == 130)
				exit(130);
			exit(0);
		}
		dollar = expand_dollar1(str, info);
		if (dollar)
		{
			write(fd, dollar, ft_strlen(dollar));
			write(fd, "\n", 1);
			free(dollar);
		}
		else
		{
			write(fd, str, ft_strlen(str));
			write(fd, "\n", 1);
		}
		free(str);
	}
}

int	ft_heredoc_parent(int pid,int *status, t_info *info)
{
	int	fd;
	int exit;

	signal(SIGINT, SIG_IGN);
	waitpid(pid, status, 0);
	fd = open (info->heredoc, O_RDONLY);
	if (fd == -1)
	{
		write(2, "Minishell: ", 11);
		write(2, info->heredoc, ft_strlen(info->heredoc));
		write(2, ": ", 2);
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2, "\n", 1);
		return (estat(126, info), -1);
	}
	unlink(info->heredoc);
	free(info->heredoc);
	info->heredoc = NULL;
	info->fd_in_child = fd;
	set_signals();
	if (WIFSIGNALED(*status))
	{
		if (WTERMSIG(*status) == SIGQUIT)
		{
			write(2, "Quit (core dumped)\n", 19);
			estat(131, info);
			return (-1);
		}
	}
	else if (WIFEXITED(*status))
	{
		exit = WEXITSTATUS(*status);
		if (exit == 130)
		{
			estat(130, info);
			return (-1);
		}
	}
	return (estat(0, info), 0);
}

int	ft_heredoc(char **exec, t_info *info)
{
	int		status;
	pid_t	pid;
	char	*filename;

	filename = heredoc_filename();
	if (!filename)
		return (write(2, "Minishell: failed to create heredoc filename\n", 45), -1);
	info->heredoc = filename;
	pid = fork();
	if (pid == -1)
		return (free(filename), -1);
	if (pid == 0)
	{
		signal(SIGINT, ctrl_c_here);
		signal(SIGQUIT, SIG_IGN);
		ft_heredoc_process(exec, info, filename);
	}
	else
	{
		if (ft_heredoc_parent(pid, &status, info) == -1)
			return (-1);
	}
	return (0);
}
