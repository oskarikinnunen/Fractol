/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bigfloat.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 09:35:56 by okinnune          #+#    #+#             */
/*   Updated: 2022/05/12 12:12:01 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

#include <limits.h>

t_hugefloat	*hf_new()
{
	t_hugefloat	*result;
	result = ft_memalloc(sizeof(t_hugefloat));

	printf("long_max %li half %li", LONG_MAX, LONG_MAX / 2);
	result->exponent = ft_memalloc(sizeof(long int));
	result->significand = ft_memalloc(sizeof(long int));
	return (result);
}

/*int	bf_mul(t_hugefloat f1, t_hugefloat f2)
{
	return (f1.exponent * f2.exponent);
}*/