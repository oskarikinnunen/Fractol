/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractals.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 23:36:12 by okinnune          #+#    #+#             */
/*   Updated: 2022/06/17 10:44:36 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	julia(long double *arg)
{
	int			color;
	t_complex	f;

	ft_memcpy(&f, arg, sizeof(long double [2]));
	color = 0;
	while (ft_absd(f.real * f.real + f.imaginary * f.imaginary) < 4
		&& color < MAX_ITERS)
	{
		ft_memcpy(&f, arg, sizeof(long double [2]));
		arg[X] = f.real * f.real - f.imaginary * f.imaginary + (float)arg[2];
		arg[Y] = 2 * f.real * f.imaginary + (float)arg[3];
		color++;
	}
	if (color == MAX_ITERS)
		return (0);
	return (color * ((0xFFFFFF16) / MAX_ITERS));
}

/*
	arg[0] == c.real
	arg[1] == c.imaginary
*/
int	mandelbrot(long double *arg)
{
	int			color;
	t_complex	f;
	t_complex	prev;

	ft_bzero(&f, sizeof(t_complex));
	color = 0;
	while (ft_absd(f.real * f.real + f.imaginary * f.imaginary) < 4
		&& color < MAX_ITERS)
	{
		ft_memcpy(&prev, &f, sizeof(t_complex));
		f.real = (f.real * f.real) - (f.imaginary * f.imaginary) + arg[0];
		f.imaginary = 2 * prev.real * prev.imaginary + arg[1];
		color++;
	}
	if (color == MAX_ITERS)
		return (0);
	return ((int)(color * (0xFFFFFF16) / MAX_ITERS));
}

int	ship(long double *arg)
{
	int			color;
	t_complex	f;
	t_complex	prev;

	ft_bzero(&f, sizeof(t_complex));
	color = 0;
	while (f.real * f.real + f.imaginary * f.imaginary < 4 && color < MAX_ITERS)
	{
		ft_memcpy(&prev, &f, sizeof(t_complex));
		f.real = (f.real * f.real) - (f.imaginary * f.imaginary) + arg[0];
		f.imaginary = 2 * prev.real * prev.imaginary + arg[1];
		f.real = ft_absd(f.real);
		f.imaginary = ft_absd(f.imaginary);
		color++;
	}
	if (color == MAX_ITERS)
		return (0);
	return (color * ((0xFFFFFF16) / MAX_ITERS));
}
