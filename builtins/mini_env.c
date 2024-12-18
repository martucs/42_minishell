/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 15:41:11 by ineimatu          #+#    #+#             */
/*   Updated: 2024/12/18 20:42:25 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"
#include "../includes/builtin.h"
#include "../includes/struct.h"

int	mini_env(char **argv, t_info *info)
{
	t_envp	*tmp;

	tmp = info->envp;
	if (argv[1])
	{
		ft_putstr_fd("env: `", 2);
		write(2, argv[1], ft_strlen(argv[1]));
		ft_putstr_fd("': No such file or directory\n", 2);
		return (1);
	}
	while (tmp)
	{
		write(1, tmp->key, ft_strlen(tmp->key));
		write(1, tmp->value, ft_strlen(tmp->value));
		write(1, "\n", 1);
		tmp = tmp->next;
	}
	return (0);
}
