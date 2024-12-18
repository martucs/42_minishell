/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_clean2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:07:58 by ineimatu          #+#    #+#             */
/*   Updated: 2024/12/18 20:49:07 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/execution.h"
#include "../includes/struct.h"
#include "../includes/signal.h"
#include "../includes/expansion.h"

void	mult_cmd_signal(t_cmd *aux, t_info *info, t_exec *exec_info)
{
	while (aux)
	{
		waitpid(aux->pid, &info->ex_stat, 0);
		aux = aux->next;
	}
	signal_handle(info->ex_stat);
	info->ex_stat = g_global;
	if (g_global == 130 || g_global == 131)
	{
		if (g_global == 131)
			write(2, "Quit: \n", 7);
		else
			write(2, "\n", 1);
		g_global = 0;
	}
	free_exec_info(exec_info);
}

int	start_heredoc(t_cmd *segmts, t_info *info, t_exec *exec_info)
{
	g_global = 0;
	find_heredocs(segmts);
	if (g_global == 130 || g_global == 131)
	{
		info->ex_stat = g_global;
		g_global = 0;
		free_exec_info(exec_info);
		return (0);
	}
	return (1);
}

void	simpl_cmd_signal(t_cmd *cmd, t_info *info)
{
	waitpid(cmd->pid, &(info->ex_stat), 0);
	if (cmd->pid != -1)
	{
		signal_handle(info->ex_stat);
		info->ex_stat = g_global;
	}
	if (g_global == 130 || g_global == 131)
	{
		if (g_global == 131)
			write(2, "Quit: \n", 7);
		else
			write(2, "\n", 1);
		g_global = 0;
	}
}

int	builtin_cmd_protect(t_cmd *cmd, t_info *info)
{
	if (open_redir(cmd) == 1)
	{
		info->ex_stat = 1;
		return (1);
	}
	if (redirect(cmd) == 1)
	{
		info->ex_stat = 1;
		return (1);
	}
	return (0);
}
