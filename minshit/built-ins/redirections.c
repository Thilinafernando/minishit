/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 22:04:01 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/29 00:55:03 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	static int	counter;
	char		*number;
	char		*filename;
	char		*tmp;

	counter = 0;
	number = ft_itoa(counter++);
	if (!number)
		return (NULL);
	tmp = ft_strjoin("/tmp/.heredoc_", number);
	free(number);
	if (!tmp)
		return (NULL);
	filename = ft_strjoin(tmp, ".tmp");
	free(tmp);
	return (filename);
}

int	setting_stdin(int *std_in)
{
	(*std_in) = open("/dev/tty", O_RDONLY);
	if ((*std_in) == -1)
	{
		write(2, "Minishell: cannot open /dev/tty\n", 32);
		return (-1);
	}
	if (dup2((*std_in), STDIN_FILENO) == -1)
	{
		write(2, "Minishell: dup2 failed\n", 23);
		close((*std_in));
		return (-1);
	}
	close((*std_in));
	return (0);
}

void	err_fd_heredoc(char *filename)
{
	write(2, "Minishell : ", 12);
	write(2, filename, ft_strlen(filename));
	write(2, ": ", 2);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, "\n", 1);
}

void	readline_child(char **filename, t_info *info, int fd, char **str)
{

	free((*str));
	free((*filename));
	close(fd);
	free_all(info);
	if (signal_status == 130)
		exit(130);
	exit(0);
}

void	ft_heredoc_process(char **exec, t_info *info, char *filename)
{
	char	*dollar;
	int		fd;
	int		std_in;

	dollar = NULL;
	if (setting_stdin(&std_in) == -1)
		return (free(filename), exit(1));
	fd = open(filename, O_CREAT| O_TRUNC | O_WRONLY, 0644);
	if (fd == -1)
		return (err_fd_heredoc(filename), free(filename), exit(126));
	while (1)
	{
		str = readline("> ");
		if (!str || !exec[1] || ft_strcmp(str, exec[1]) == 0)
		{
			readline_child(&filename, info, &str);
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
