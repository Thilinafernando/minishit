/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <tkurukul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 01:47:10 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/24 22:58:39 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//  more args

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

char	**matrix_join(t_info *info, char **args, int size)
{
	int		i;
	int		x;
	int		j;
	char	**new;

	i = 0;
	x = 0;
	new = malloc((size + 1) * sizeof(char *));
	while (x <= size)
	{
		new[x] = NULL;
		x++;
	}
	j = 0;
	while(info->tmp[j])
	{
		new[i] = ft_strdup(info->tmp[j]);
		i++;
		j++;
	}
	j = 0;
	while(args[j])
	{
		if (append(new, args[j]) != 1)
		{
			new[i] = ft_strdup(args[j]);
			i++;
		}
		j++;
	}
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

void	print_export(t_info *info)
{
	int	i;
	int	j;
	int	flag;

	i = -1;
	sorting(info);
	while (info->tmp[++i])
	{
		if (ft_strncmp(info->tmp[i], "_=", 2) == 0)
			continue;
		flag = 0;
		j = -1;
		write(1, "declare -x ", 11);
		while (info->tmp[i][++j])
		{
			write(1, &info->tmp[i][j], 1);
			if (info->tmp[i][j] == '=' && flag == 0)
			{
				write(1, "\"", 1);
				flag = 1;
			}
			if (!info->tmp[i][j + 1] && flag != 0)
				write(1, "\"", 1);
		}
		write(1, "\n", 1);
	}
	free_mat(info->tmp);
}


void	ft_export(t_info *info, char **args)
{
	int	i;
	int	x;
	int	j;
	int	count;
	char	**tmp;

	i = 1;
	x = 0;
	count = 0;
	if (!args[i])
	{
		print_export(info);
		return (estat(0, info));
	}
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
			count++;
		i++;
	}
	tmp = malloc((count + 1) * sizeof(char *));
	while (x <= count)
	{
		tmp[x] = NULL;
		x++;
	}
	i = 1;
	j = 0;
	while (args[i])
	{
		if((verify(args[i]) == 0))
		{
			tmp[j] = ft_strdup(args[i]);
			j++;
		}
		i++;
	}
	tmp[j] = NULL;
	matrix_tmp(info);
	free_mat(info->env);
	info->size += count;
	info->env = matrix_join(info, tmp, info->size);
	free_mat(tmp);
	return (estat(info->exit_status, info));
}


/* int	main(int ac, char **av, char **env)
{
	t_info	info;
	char	*args[4];
	// int	i;

	// i = 0;
	(void)ac;
	(void)av;
	args[0] = NULL;
	args[1] = "1fa== 2";
	args[2] = "fic=1";
	args[3] = NULL;
	info.env = NULL;
	form_env(env, &info);
	ft_export(&info, args);
	// while (info.env[i])
	// {
	// 	printf("%s\n", info.env[i]);
	// 	i++;
	// }
	free_mat(info.env);
	// printf("\nBEFORE\n\n\nAFTER\n");
	// i = 0;
	// while (info.env[i])
	// {
	// 	printf("%s\n", info.env[i]);
	// 	fflush(stdout);
	// 	i++;
	// }
} */
//first character has to be a-z
// later it can be a combo of characters numerals and under4score
// "" and '' allowed any where
// try to save env in a ***matrix
// variables that are correct will work
