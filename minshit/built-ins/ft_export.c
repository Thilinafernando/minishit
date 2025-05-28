/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 01:47:10 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/28 23:40:58 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	verify(char	*str)
{
	int	i;

	i = 0;
	if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || str[i] == '_'))
		return (-1);
	else
		i++;
	while(str[i] && str[i] != '=')
	{
		if ((str[i] >= 'a' && str[i] <= 'z') || ((str[i] >= 'A' && str[i] <= 'Z')) || str[i] == '_'
		|| (str[i] >= '0' && str[i] <= '9') ||  str[i] == 61 || str[i] == '"' || str[i] == 39)
			i++;
		else
			return (-1);
	}
	return (0);
}

int	verify_equal(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == 61)
			count = 1;
		i++;
	}
	if (count != 1)
		return (-1);
	return (0);
}

int	exisit(char **matrix, char *arg)
{
	int	i;
	int	len;
	int	flag;

	i = 0;
	flag = 0;
	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	while (matrix[i])
	{
		if (ft_strncmp(matrix[i], arg, len) == 0 && (matrix[i][len] == '=' || matrix[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

void	matrix_tmp(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (info->env[i])
		i++;
	info->size = i;
	info->tmp = malloc((i + 1) * sizeof(char *));
	while (info->env[j])
	{
		info->tmp[j] = ft_strdup(info->env[j]);
		j++;
	}
	info->tmp[j] = NULL;
	return ;
}

int	append(char **matrix, char *args)
{
	int	pos;
	int	flag;

	flag = 0;
	pos = exisit(matrix, args);
	if (pos != -1)
	{
		if ((verify_equal(args) == -1) && (verify_equal(matrix[pos]) == 0))
		{
			free(matrix[pos]);
			matrix[pos] = ft_strjoin(args, "=");
		}
		else
		{
			free(matrix[pos]);
			matrix[pos] = ft_strdup(args);
		}
		flag = 1;
	}
	return (flag);
}

void	process_join(t_info *info, char **args, char ***new)
{
	int		j;
	int		i;

	j = 0;
	i = 0;
	while(info->tmp[j])
	{
		(*new)[i] = ft_strdup(info->tmp[j]);
		i++;
		j++;
	}
	j = 0;
	while(args[j])
	{
		if (append((*new), args[j]) != 1)
		{
			(*new)[i] = ft_strdup(args[j]);
			i++;
		}
		j++;
	}
}

char	**matrix_join(t_info *info, char **args, int size)
{
	int		x;
	char	**new;

	x = 0;
	new = malloc((size + 1) * sizeof(char *));
	if (!new)
		return (NULL);
	while (x <= size)
	{
		new[x] = NULL;
		x++;
	}
	process_join(info, args, &new);
	free_mat(info->tmp);
	new[size] = NULL;
	return (new);
}

void	swap(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 && !s2)
		return (0);
	if (!s1)
		return (-1);
	if (!s2)
		return (1);
	while (*s1 && *s2 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

void	sorting(t_info *info)
{
	int	i;
	int	flag;

	matrix_tmp(info);
	flag = 1;
	while (flag)
	{
		i = 0;
		flag = 0;
		while (info->tmp[i + 1])
		{
			if (ft_strcmp(info->tmp[i], info->tmp[i + 1]) > 0)
			{
				swap(&info->tmp[i], &info->tmp[i + 1]);
				flag = 1;
			}
			i++;
		}
	}
}

void	printing_process(t_info *info, int *i)
{
	int	j;
	int	flag;

	flag = 0;
	j = -1;
	while (info->tmp[(*i)][++j])
	{
		write(1, &info->tmp[(*i)][j], 1);
		if (info->tmp[(*i)][j] == '=' && flag == 0)
		{
			write(1, "\"", 1);
			flag = 1;
		}
		if (!info->tmp[(*i)][j + 1] && flag != 0)
			write(1, "\"", 1);
	}
}

void	print_export(t_info *info)
{
	int	i;

	i = -1;
	sorting(info);
	while (info->tmp[++i])
	{
		if (ft_strncmp(info->tmp[i], "_=", 2) == 0)
			continue;
		write(1, "declare -x ", 11);
		printing_process(info, &i);
		write(1, "\n", 1);
	}
	free_mat(info->tmp);
}

void	verify_loop(char **args, t_info *info,int *count)
{
	int i;

	i = 1;
	while (args[i])
	{
		if((verify(args[i]) != 0))
		{
			write(2, "Minishell: export: '", 20);
			write(2, args[i], ft_strlen(args[i]));
			write(2, "' : not a valid identifier\n", 27);
			estat(1, info);
		}
		else
			(*count)++;
		i++;
	}
}

void	copy_loop(char **args, t_info *info, char ***tmp)
{
	int	j;
	int	i;

	i = 1;
	j = 0;
	while (args[i])
	{
		if((verify(args[i]) == 0))
		{
			(*tmp)[j] = ft_strdup(args[i]);
			j++;
		}
		i++;
	}
	(*tmp)[j] = NULL;
}

void	ft_export(t_info *info, char **args)
{
	int	x;
	int	count;
	char	**tmp;

	x = 0;
	count = 0;
	if (!args[1])
	{
		print_export(info);
		return (estat(0, info));
	}
	verify_loop(args, info, &count);
	tmp = malloc((count + 1) * sizeof(char *));
	while (x <= count)
	{
		tmp[x] = NULL;
		x++;
	}
	copy_loop(args, info, &tmp);
	matrix_tmp(info);
	free_mat(info->env);
	info->size += count;
	info->env = matrix_join(info, tmp, info->size);
	free_mat(tmp);
	return (estat(info->exit_status, info));
}
