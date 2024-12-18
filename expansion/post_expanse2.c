/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_expanse2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 01:44:49 by martalop          #+#    #+#             */
/*   Updated: 2024/12/18 20:48:18 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/expansion.h"

int	find_double_quote(char *str)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	while (str && str[i])
	{
		if (str[i] == '\'')
			skip_quote_content((const char *)str, &i);
		if (str[i] == '\"')
		{
			quote++;
			i++;
			while (str[i] && str[i] != '\"')
				i++;
			if (str[i])
				i++;
		}
		if (str[i])
			i++;
	}
	return (quote);
}

int	find_simp_quote(char *str)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	while (str && str[i])
	{
		if (str[i] == '\"')
			skip_quote_content((const char *)str, &i);
		if (str[i] == '\'')
		{
			quote++;
			i++;
			while (str[i] && str[i] != '\'')
				i++;
			if (str[i])
				i++;
		}
		if (str[i])
			i++;
	}
	return (quote);
}
