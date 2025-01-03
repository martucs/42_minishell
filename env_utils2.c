/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 21:51:04 by martalop          #+#    #+#             */
/*   Updated: 2024/12/18 20:36:11 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "includes/minishell.h"

int	fill_env_node(char *env_line, t_envp *node, t_envp **env_lst)
{
	int	found_at;

	found_at = 0;
	found_at = find_equal(env_line);
	node->key = ft_substr(env_line, 0, found_at + 1);
	if (!node->key)
	{
		free_envlst(*env_lst);
		return (1);
	}
	node->value = ft_substr(env_line, found_at + 1,
			ft_strlen(env_line) - found_at);
	if (!node->value)
	{
		free(node->key);
		free_envlst(*env_lst);
		return (1);
	}
	node->next = NULL;
	return (0);
}

void	free_envlst(t_envp *envp)
{
	t_envp	*tmp;

	tmp = envp;
	while (tmp)
	{
		tmp = tmp->next;
		free(envp->key);
		free(envp->value);
		free(envp);
		envp = tmp;
	}
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	cmdlst_size(t_cmd *cmds)
{
	int		i;
	t_cmd	*tmp;

	i = 0;
	tmp = cmds;
	while (tmp)
	{
		if (tmp->next != NULL)
		{
			tmp = tmp->next;
			i++;
		}
		else
		{
			i++;
			break ;
		}
	}
	return (i);
}
