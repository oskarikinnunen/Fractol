/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sample_image.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 16:48:06 by okinnune          #+#    #+#             */
/*   Updated: 2022/06/17 00:57:16 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	set_img_pixel(t_image_info img, int x, int y, unsigned int color)
{
	x = x * sizeof(int);
	y = y * img.size_line;
	*(unsigned int *)(img.addr + x + y) = color;
}

unsigned int	get_img_pixel(t_image_info img, int x, int y)
{
	x = x * sizeof(int);
	y = y * img.size_line;
	return (*(unsigned int *)(img.addr + x + y));
}

int	pixel_index(int x, int y, int line_width)
{
	return ((y * line_width) + x);
}

int	calculate_color(t_mlx_info info, int *crd, int color_offset)
{
	if (info.colormode == greenhell)
		return ((get_img_pixel(info.img[1], crd[X], crd[Y])
				+ color_offset) & 0xFFFFF16);
	if (info.colormode == blackandwhite)
		return ((get_img_pixel(info.img[1], crd[X], crd[Y]) & 255)
			+ ((get_img_pixel(info.img[1], crd[X], crd[Y]) & 255) << 8)
			+ ((get_img_pixel(info.img[1], crd[X], crd[Y]) & 255) << 16));
	if (info.colormode == sine1)
		return ((int)(sin((double)get_img_pixel(info.img[1], crd[X], crd[Y])
				/ INT_MAX) * color_offset) << info.color_bit_offset);
	if (info.colormode == sine2)
		return ((int)(sin((double)get_img_pixel(info.img[1], crd[X], crd[Y])
				/ INT_MAX) * (color_offset / 150 * color_offset / 150))
					<< info.color_bit_offset);
	return (get_img_pixel(info.img[1], crd[X], crd[Y])
		<< info.color_bit_offset);
}

void	sample_image(t_mlx_info *info)
{
	int			crd[2];
	int			scaled_crd[2];
	int			offset;
	static int	color_offset;

	ft_bzero(crd, sizeof(int [2]));
	ft_bzero(scaled_crd, sizeof(int [2]));
	offset = (WSZ) * (0.5 - info->img_zoom);
	color_offset += 5.0 * info->color_add;
	while (crd[Y] < info->img->size[Y])
	{
		scaled_crd[Y] = (crd[Y] - offset) / info->img_zoom;
		while (crd[X] < info->img->size[X])
		{
			scaled_crd[X] = (crd[X] - offset) / info->img_zoom;
			set_img_pixel(*info->img, crd[X], crd[Y],
				calculate_color(*info, scaled_crd, color_offset));
			crd[X]++;
		}
		crd[X] = 0;
		crd[Y]++;
	}
}
