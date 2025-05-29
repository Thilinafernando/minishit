/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:10:33 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/05/29 17:21:41 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int signal_status = 0;

int main(int ac, char **av, char **env)
{
	t_token	*token;
	t_info	info;
	char	*line;
	int		i;
	int		j;

	if (ac != 1 || av[1])
	{
		write(2, "Error: Invalid arguments\n", 25);
		return (1);
	}
	form_env(env, &info);
	using_history();
	info.exit_status = 0;
	while (1)
	{
		set_signals();
		info.pos = 0;
		info.fd_in_child = -420;
		info.fd_out_child = -420;
		info.exec = NULL;
		info.tmp = NULL;
		info.dollar = NULL;
		// info.oldpwd = NULL;
		//signal(SIGINT, ctrl_c);
		// signal(SIGQUIT, estat);
		line = readline("\001\033[1;32m\002minishell$ \001\033[0m\002");
		if (!line)
		{
			write(1, "exit\n", 5);  // Print exit message on Ctrl+D
			break;
		}
		if (signal_status == 131)
		{
			info.exit_status = 130;
			signal_status = 0;
		}
		if (line[0] != '\0')
		{
			add_history(line);
			token = tokenize_input(line, &info);  // Tokenize the input line with info
			// debug_print_tokens(token);
			if (!token)
			{
				free(line);
				continue;
			}
			if (!validate_syntax(token, &info, NULL))
			{
				free_tokens(token);
				continue;
				//return (0); // or skip execution
			}
			if (token)
			{
				printf("%d\n", token->type);
				form_main(token, &info);
				i = 0;
				while (info.exec[i])
				{
					j = 0;
					//arg_execve(&info.exec[i], &info);
					while (info.exec[i][j])
					{
						printf("%s\n", info.exec[i][j]);
						j++;
					}
					printf("----------------------\n");
					i++;
					fflush(stdout);
				}
				free_tokens(token);  // Use your token free function
				ft_execution(&info);
			}
		}
		free(line);  // Free the line after processing
	}
	clear_history();
	free_mat(info.env);
	close (1);
	close (2);
	close (0);
	return (0);
}
