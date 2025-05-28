/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftt_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 19:01:54 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/29 00:43:17 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_var_name(const char *line, int *i)
{
	int		start;
	char	*var;

	if (line[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	start = *i;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	var = ft_substr(line, start, *i - start);
	return (var);
}

char	*append_and_free(char *dst, char *segment)
{
	char	*temp;

	if (!segment)
		return (dst);
	if (!dst)
		return (segment);
	temp = ft_strjoin(dst, segment);
	free(dst);
	free(segment);
	return (temp);
}

void	append_variable(char **result, char *line, int *i, int *start, t_info *info)
{
	char	*segment;
	char	*var;
	char	*val;

	if (*i > *start)
	{
		segment = ft_substr(line, *start, *i - *start);
		*result = append_and_free(*result, segment);
	}
	(*i)++;
	var = get_var_name(line, i);
	val = mdollar(var, info);
	free(var);
	*result = append_and_free(*result, val);
	*start = *i;
}

char	*expand_dollar1(char *line, t_info *info)
{
	int		i;
	int		start;
	char	*result;
	char	*segment;

	i = 0;
	start = 0;
	result = NULL;
	while (line[i])
	{
		if (line[i] == '$'
			&& (line[i + 1] == '?' || ft_isalpha(line[i + 1]) || line[i + 1] == '_'))
			append_variable(&result, line, &i, &start, info);
		else
			i++;
	}
	if (i > start)
	{
		segment = ft_substr(line, start, i - start);
		result = append_and_free(result, segment);
	}
	if (!result)
		return (ft_strdup(""));
	return (result);
}

