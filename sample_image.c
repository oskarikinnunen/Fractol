/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sample_image.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 16:48:06 by okinnune          #+#    #+#             */
/*   Updated: 2022/05/25 13:25:05 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	set_img_pixel(t_image_info img, int x, int y, unsigned int color)
{
	x = x * sizeof(int); //TODO: bpp * sizeof char?
	y = y * img.size_line;
	*(unsigned int *)(img.addr + x + y) = color;
}

unsigned int	get_img_pixel(t_image_info img, int x, int y)
{
	x = x * sizeof(int); //TODO: bpp * sizeof char?
	y = y * img.size_line;
	return (*(unsigned int *)(img.addr + x + y));
}

int	pixel_index(int x, int y, int line_width)
{
	return ((y * line_width) + x);
}

void	sample_image(t_mlx_info *info)
{
	int	crd[2];
	int	scaled_crd[2];
	float	ogzoom = info->img_zoom;

	ft_bzero(crd, sizeof(int [2]));
	ft_bzero(scaled_crd, sizeof(int [2]));
	int offset = (WSZ) * (0.5 - info->img_zoom);
	while (crd[Y] < info->img->size[Y])
	{
		while (crd[X] < info->img->size[X])
		{
			scaled_crd[X] =  (crd[X] - offset) / info->img_zoom;
			scaled_crd[Y] = (crd[Y] - offset) / info->img_zoom;
			set_img_pixel(*info->img, crd[X], crd[Y], get_img_pixel(info->img[1], scaled_crd[X], scaled_crd[Y]));
			if (scaled_crd[X] == info->img[1].size[X] / 2 && scaled_crd[Y] == info->img[1].size[Y] / 2)
				set_img_pixel(*info->img, crd[X], crd[Y], INT_MAX);
			crd[X]++;
		}
		crd[X] = 0;
		crd[Y]++;
	}
	printf("offset %i zoom %f\n", offset, info->img_zoom);
}