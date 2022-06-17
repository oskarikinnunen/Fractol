/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_colorlerp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 16:53:39 by okinnune          #+#    #+#             */
/*   Updated: 2022/05/05 14:53:24 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

float	ft_lerpf(float f1, float f2, float lerp)
{
	//float	min;
	//float	max;
	int sign = 1 + ((f1 > f2) * -2);

	//min = ft_minf(f1, f2);
	//max = ft_maxf(f1, f2);
	return (f1 + ((f1 - f2) * (lerp * sign)));
}

#include <stdio.h>
int	g_colorlerp(unsigned int color1, unsigned int color2, float lerp)
{
	unsigned int	color;
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;

	ft_clampf(lerp, 0.0, 1.0);
	b = ft_lerpf(color1 & 255, color2 & 255, lerp);
	g = ft_lerpf((color1 >> 8) & 255, (color2 >> 8) & 255, lerp);
	r = ft_lerpf(color1 >> 16, color2 >> 16, lerp);
	//printf("color 1 R: %i color 2 R: %i Lerped: %i\n", color1 >> 16, color2 >> 16, r);
	color = b + (g << 8) + (r << 16);
	return (color);
}