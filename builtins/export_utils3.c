/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 16:24:08 by ineimatu          #+#    #+#             */
/*   Updated: 2024/12/18 20:45:29 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"
#include "../includes/builtin.h"
#include "../includes/struct.h"

int	handle_env_copy(t_info *info, char *export, int flag)
{
	int	res;

	res = 0;
	if (flag && flag != 2)
		res = add_node_to_copy(info, info->copy, export);
	if (flag == 2)
		return (2);
	return (add_node_to_copy(info, info->envp, export));
}

int	not_in_env(t_info *info, char *export)
{
	t_envp	*tmp;
	int		i;
	int		flag;

	flag = 1;
	tmp = info->copy;
	i = find_equal(export);
	if (i)
	{
		while (tmp)
		{
			if (ft_strncmp(tmp->key, export, ft_strlen(tmp->key)) == 0)
			{
				flag = check_change_copy(info->copy, export, i);
				if (exists_in_env(info->envp, export))
					return (change_copy_equal(export, info->envp, i));
			}
			tmp = tmp->next;
		}
		return (handle_env_copy(info, export, flag));
	}
	else
		return (add_node_to_copy(info, info->copy, export));
}

int	add_node_to_copy(t_info *info, t_envp *env, char *export)
{
	t_envp	*tmp;
	int		i;

	tmp = env;
	while (tmp->next)
		tmp = tmp->next;
	i = find_equal(export);
	if (i)
	{
		if (add_node_ch_val(tmp, i, export) == 2)
			return (2);
	}
	else
	{
		if (exists_in_copy(info->copy, export) == 0)
		{
			if (add_node_without_val(tmp, export) == 2)
				return (2);
		}
	}
	return (0);
}

int	check_change_copy(t_envp *copy, char *export, int i)
{
	t_envp	*tmp;
	int		e;

	tmp = copy;
	while (tmp)
	{
		e = find_equal(tmp->key);
		if (!e)
		{
			if (ft_strncmp(tmp->key, export, ft_strlen(tmp->key)) == 0)
				return (copy_without_equal(tmp, export, i));
		}
		else
			if (ft_strncmp(tmp->key, export, i) == 0)
				return (copy_with_eq(tmp, export, i));
		tmp = tmp->next;
	}
	return (1);
}

void	print_env_sorted(t_info *info)
{
	t_envp	*sorted;

	sorted = sort_alg(info->copy, NULL);
	info->copy = sorted;
	while (sorted)
	{
		if (sorted->key[0] == '_' && sorted->key[2])
		{
			ft_putstr_fd("declare -x ", 1);
			write(1, sorted->key, ft_strlen(sorted->key));
			write(1, sorted->value, ft_strlen(sorted->value));
			write(1, "\n", 1);
			sorted = sorted->next;
		}
		else if (sorted->key[0] == '_' && !sorted->key[2])
			sorted = sorted->next;
		else
		{
			write_key_value(sorted);
			sorted = sorted->next;
		}
	}
}
