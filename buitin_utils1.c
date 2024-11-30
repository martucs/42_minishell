/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buitin_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:53:34 by ineimatu          #+#    #+#             */
/*   Updated: 2024/10/15 18:15:39 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "builtin.h"
#include "struct.h"

void	free_pwd(char *pwd, char *path, t_info *info)
{
	ft_putstr_fd("cd: error retrieving current directory\n", 2);
	path = ft_getenv("HOME=", info->envp);
	if (chdir(path) == -1)
	{
		if (path == ft_getenv("HOME=", info->envp))
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			free(pwd);
			return ;
		}
	}
	free(pwd);
}

void	write_key_value(t_envp *sorted)
{
	ft_putstr_fd("declare -x ", 1);
	write(1, sorted->key, ft_strlen(sorted->key));
	if (sorted->value != NULL && sorted->value[0] != '\0')
	{
		write(1, "\"", 1);
		write(1, sorted->value, ft_strlen(sorted->value));
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}

int	exists_in_copy(t_envp *env, char *export)
{
	t_envp	*tmp;

	tmp = env;
	if (find_equal(export))
	{
		while (tmp)
		{
			if (ft_strncmp(tmp->key, export, ft_strlen(tmp->key)) == 0)
				return (1);
			tmp = tmp->next;
		}
	}
	else
	{
		while (tmp)
		{
			if (ft_strncmp(tmp->key, export, ft_strlen(export)) == 0)
				return (1);
			tmp = tmp->next;
		}
	}
	return (0);
}

t_envp	*sort_alg(t_envp *lst_export, t_envp *sorted)
{
	t_envp	*current;
	t_envp	*the_next;
	t_envp	*tmp;

	current = lst_export;
	while (current)
	{
		the_next = current->next;
		if (!sorted || ft_strcmp(current->key, sorted->key) < 0)
		{
			current->next = sorted;
			sorted = current;
		}
		else
		{
			tmp = sorted;
			while (tmp->next && ft_strcmp(current->key,
					tmp->next->key) >= 0)
				tmp = tmp->next;
			current->next = tmp->next;
			tmp->next = current;
		}
		current = the_next;
	}
	return (sorted);
}
