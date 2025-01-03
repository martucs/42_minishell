/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <martalop@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:11:35 by martalop          #+#    #+#             */
/*   Updated: 2024/09/09 20:07:38 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	indx;

	if (!s)
		return (0);
	indx = 0;
	while (s[indx] != '\0')
		indx++;
	return (indx);
}

/*#include <stdio.h>

int	main(void)
{
	char	*string;

	string = "holap";
	printf("%zu", ft_strlen(string));
	return (0);
}*/
