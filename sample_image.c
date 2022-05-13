/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sample_image.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 16:48:06 by okinnune          #+#    #+#             */
/*   Updated: 2022/05/13 12:20:50 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

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
			
			
			scaled_crd[X] = /*((WSZ / 4) * (info->img_zoom - 1.0)) +*/ (crd[X] - offset) / info->img_zoom; //PLus centered screen offset
			//scaled_crd[X] -= offset;
			//scaled_crd[X] += info->img[1].size_line;
			scaled_crd[Y] = /*((WSZ / 4) * (info->img_zoom - 1.0)) +*/ (crd[Y] - offset) / info->img_zoom;
			*(unsigned int * )(info->img->addr + (crd[X] * sizeof(int)) + (crd[Y] * info->img->size_line)) = *(unsigned int *)(info->img[1].addr + (scaled_crd[X] * sizeof(int)) + (scaled_crd[Y] * info->img[1].size_line));

			if (scaled_crd[X] == info->img[1].size[X] / 2 && scaled_crd[Y] == info->img[1].size[Y] / 2)
			{
				*(unsigned int * )(info->img->addr + (crd[X] * sizeof(int)) + (crd[Y] * info->img->size_line)) = INT_MAX;
			}
			//*(unsigned int * )(info->img->addr + (crd[X] * sizeof(int)) + (crd[Y] * info->img->size_line)) = *(unsigned int *)(info->img[1].addr + (scaled_crd[X] * sizeof(int)) + (scaled_crd[Y] * info->img[1].size_line));
			crd[X]++;
		}
		crd[X] = 0;
		crd[Y]++;
	}
	printf("offset %i zoom %f\n", offset, info->img_zoom);
}