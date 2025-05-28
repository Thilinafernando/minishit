/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_intergration.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:19:42 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/29 00:29:32 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	save_redirections(t_info *info, t_token *token)
{
	t_token *tmp;
	char	**matrix;

	tmp = token;
	while (tmp && tmp->type != 1)
	{
		if (tmp->type == 2 || tmp->type == 3
			|| tmp->type == 4 || tmp->type == 7)
		{
			matrix = malloc(3 * sizeof(char*));
			if (!matrix)
				return ;
			matrix[2] = NULL;
			matrix[0] = ft_strdup(tmp->content);
			matrix[1] = ft_strdup(tmp->next->content);
			info->exec[info->pos] = matrix;
			info->pos++;
			tmp = tmp->next;
		}
		tmp = tmp->next;
	}
}

int	size_command(t_token **token)
{
	int i;
	t_token	*tmp;

	i = 0;
	tmp = (*token);
	while (tmp && tmp->type != 1)
	{
		if (tmp->type != 2 && tmp->type != 4 && tmp->type != 3 && tmp->type != 7)
			i++;
		if (tmp->type == 2 || tmp->type == 4 || tmp->type == 3 || tmp->type == 7)
			tmp = tmp->next->next;
		else
			tmp = tmp->next;
	}
	return (i);
}

void	copy_command(t_info *info, t_token **token, char ***matrix)
{
	int	j;

	j = 0;
	while ((*token) && (*token)->type != 1)
	{
		if ((*token)->type != 2 && (*token)->type != 4
			&& (*token)->type != 3 && (*token)->type != 7)
		{
			if (((*token)->type == 5 || (*token)->type == 6) && ((ft_strcmp((*token)->content, "<") == 0
				|| ft_strcmp((*token)->content, ">") == 0
				|| ft_strcmp((*token)->content, ">>") == 0
				|| ft_strcmp((*token)->content, "<<") == 0)))
				(*matrix)[j] = ft_strjoin(";", (*token)->content);
			else
				(*matrix)[j] = ft_strdup((*token)->content);
			j++;
			(*token) = (*token)->next;
		}
		else
			(*token) = (*token)->next->next;
	}
	(*matrix)[j] = NULL;
}

void	save_command(t_info *info, t_token **token)
{
	int		i;
	char	**matrix;

	i = size_command(token);
	save_redirections(info, (*token));
	if (i == 0)
	{
		while ((*token) && (*token)->type != 1)
			(*token) = (*token)->next;
		return ;
	}
	matrix = malloc((i + 1) * sizeof(char *));
	if (!matrix)
		return ;
	copy_command(info, token, &matrix);
	info->exec[info->pos] = matrix;
	info->pos++;
	return ;
}

void	save_other(t_info *info, t_token **token)
{
	char	**matrix;

	matrix = malloc(2 * sizeof(char *));
	if (!matrix)
		return ;
	matrix[0] = ft_strdup((*token)->content);
	matrix[1] = NULL;
	(*token) = (*token)->next;
	info->exec[info->pos] = matrix;
	info->pos++;
}

void	ft_conditions(t_info *info, t_token **token)
{
	if ((*token)->type != 1)
		save_command(info, token);
	else
		save_other(info, token);
}

int	size_main_matrix(t_token *token)
{
	int		size;
	t_token	*tmp;

	size = 0;
	tmp = token;
	while (tmp)
	{
		if (tmp->type == 0 || tmp->type == 5 || tmp->type == 6 || tmp->type == 8)
			size++;
		if ((tmp->type == 2 || tmp->type == 3 || tmp->type == 4 || tmp->type == 7) &&
			tmp->next != NULL)
		{
			size++;
			tmp = tmp->next->next;
			continue;
		}
		if (tmp->type == 1)
			size++;
		tmp = tmp->next;
	}
	return (size);
}

void	form_main(t_token *token, t_info *info)
{
	int		i;
	int		flag;
	int		size;
	char	***matrix;

	flag = 0;
	size = size_main_matrix(token);
	matrix = malloc((size + 1) * sizeof(char**));
	if (!matrix)
		return ;
	i = size;
	while (i >= 0)
	{
		matrix[i] = NULL;
		i--;
	}
	info->exec = matrix;
	info->pos = 0;
	while (token)
		ft_conditions(info, &token);
}

// int	main(int ac, char **av, char **env)
// {
// 	t_token		*token;
// 	t_token		*token1;
// 	t_info		info;
// 	int	i;
// 	int	j;

// 	(void)ac;
// 	(void)av;
// 	form_env(env, &info);
// 	ft_env(&info);
// 	form_main(token, &info);
// 	i = 0;
// 	while (info.exec[i])
// 	{
// 		j = 0;
// 		arg_execve(&info.exec[i], &info);
// 		while (info.exec[i][j])
// 		{
// 			printf("%s\n", info.exec[i][j]);
// 			j++;
// 		}
// 		printf("----------------------\n");
// 		i++;
// 		fflush(stdout);
// 	}
// 	ft_execution(&info);
// 	// ft_env(&info);
// 	ft_printf(1, "%d: exit_status\n", exit_status);
// }
