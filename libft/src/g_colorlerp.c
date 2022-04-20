/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_colorlerp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 16:53:39 by okinnune          #+#    #+#             */
/*   Updated: 2022/04/20 17:09:35 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static float	ft_lerpf(float f1, float f2, float lerp)
{
	float	min;
	float	max;

	min = ft_minf(f1, f2);
	max = ft_maxf(f1, f2);
	return (min + ((max - min) * lerp));
}

int	g_colorlerp(int color1, int color2, float lerp)
{
	int	r;
	int	g;
	int	b;

	ft_clampf(lerp, 0.0, 1.0);
	b = ft_lerpf(color1 & 255, color2 & 255, lerp);
	g = ft_lerpf((color1 << 8) & 255, (color2 << 8) & 255, lerp);
	r = ft_lerpf(color1 << 16, color2 << 16, lerp);
}