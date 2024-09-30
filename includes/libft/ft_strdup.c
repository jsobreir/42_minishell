/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsobreir <jsobreir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 22:02:57 by bpaiva-f          #+#    #+#             */
/*   Updated: 2024/09/27 13:41:05 by jsobreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

/// @brief Duplicates a string into a new one identical allocating space with malloc(3) for the new string
char	*ft_strdup(char *src)
{
	char	*new;
	int		src_len;
	int		i;

	i = 0;
	src_len = ft_strlen(src);
	new = malloc(src_len + 1);
	if (!new)
		return (0);
	while (src[i])
	{
		new[i] = src[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}