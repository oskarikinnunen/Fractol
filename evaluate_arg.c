/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_arg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 23:04:55 by okinnune          #+#    #+#             */
/*   Updated: 2022/06/17 10:25:41 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	evaluate_arg(char *arg, t_mlx_info *info)
{
	if (ft_strcmp(arg, "julia") == 0)
		info->fptr = julia;
	if (ft_strcmp(arg, "mandelbrot") == 0)
		info->fptr = mandelbrot;
	if (ft_strcmp(arg, "ship") == 0)
		info->fptr = ship;
	return (info->fptr != NULL);
}

void	error_exit(void)
{
	ft_putstr_fd("Malloc error \n", 2);
	exit (0);
}
