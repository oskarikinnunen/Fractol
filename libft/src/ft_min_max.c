/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_min_max.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 02:41:42 by okinnune          #+#    #+#             */
/*   Updated: 2022/04/20 17:06:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_min(int i1, int i2)
{
	if (i1 < i2)
		return (i1);
	return (i2);
}

int	ft_max(int i1, int i2)
{
	if (i1 > i2)
		return (i1);
	return (i2);
}

float	ft_minf(float f1, float f2)
{
	if (f1 < f2)
		return (f1);
	return (f2);
}

float	ft_maxf(float f1, float f2)
{
	if (f1 > f2)
		return (f1);
	return (f2);
}
