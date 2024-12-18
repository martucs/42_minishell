/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 15:40:27 by ineimatu          #+#    #+#             */
/*   Updated: 2024/12/18 20:43:23 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"
#include "../includes/builtin.h"
#include "../includes/struct.h"

static void	write_pwd(char *pwd)
{
	write(1, pwd, ft_strlen(pwd));
	write(1, "\n", 1);
}

int	mini_pwd(t_envp *lst)
{
	t_envp	*tmp;
	char	*pwd;

	tmp = lst;
	pwd = NULL;
	pwd = getcwd(pwd, 0);
	if (!pwd)
	{
		while (tmp)
		{
			if (ft_strcmp(tmp->key, "PWD=") == 0)
			{
				ft_putstr_fd(tmp->value, 1);
				return (0);
			}
			else
				tmp = tmp->next;
		}
		return (1);
	}
	else
		write_pwd(pwd);
	return (free(pwd), 0);
}
