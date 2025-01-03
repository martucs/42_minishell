/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_messags.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:18:25 by martalop          #+#    #+#             */
/*   Updated: 2024/12/18 20:37:59 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/execution.h"
#include "../includes/struct.h"
#include "../libft/libft.h"

int	find_slash(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

void	cmd_not_found(char *str)
{
	if (!find_slash(str))
	{
		write(2, str, ft_strlen(str));
		write(2, ": command not found\n", 20);
	}
	else
	{
		write(2, str, ft_strlen(str));
		write(2, ": No such file or directory\n", 28);
	}
}
