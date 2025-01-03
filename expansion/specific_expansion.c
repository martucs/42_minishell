/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specific_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 14:19:19 by martalop          #+#    #+#             */
/*   Updated: 2024/12/18 20:48:04 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/expansion.h"
#include "../includes/struct.h"

void	handle_expanded_redir(char **aux, t_redir *tmp_red)
{
	if (!aux[0] || aux[1])
		tmp_red->amb_red = 1;
	else
	{
		free(tmp_red->file_name);
		tmp_red->file_name = ft_strdup(aux[0]);
	}
	free_array(aux);
}

int	expand_files(t_redir *redirs, t_envp *envp, int prev_ex_stat)
{
	t_redir	*tmp_red;
	char	**aux;

	tmp_red = redirs;
	while (tmp_red)
	{
		aux = NULL;
		aux = full_expansion(tmp_red->file_name, envp, prev_ex_stat);
		if (!aux)
			return (1);
		handle_expanded_redir(aux, tmp_red);
		tmp_red = tmp_red->next;
	}
	return (0);
}

char	**cmd_expansion(char **arr_cmd, t_envp *envp, int prev_ex_stat)
{
	int		i;
	int		x;
	char	**aux;
	char	**new_cmd;

	x = -1;
	aux = NULL;
	new_cmd = NULL;
	while (arr_cmd && arr_cmd[++x])
	{
		i = 0;
		aux = full_expansion(arr_cmd[x], envp, prev_ex_stat);
		if (!aux)
			return (NULL);
		while (aux && aux[i])
		{
			new_cmd = add_to_array(aux[i], new_cmd);
			if (!new_cmd)
				return (NULL);
			i++;
		}
		free_array(aux);
	}
	free_array(arr_cmd);
	return (new_cmd);
}
