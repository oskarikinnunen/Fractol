/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sample_image.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 16:48:06 by okinnune          #+#    #+#             */
/*   Updated: 2022/05/12 19:08:51 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	sample_image(t_mlx_info *info)
{
	int	crd[2];
	int	scaled_crd[2];

	ft_bzero(crd, sizeof(int [2]));
	ft_bzero(scaled_crd, sizeof(int [2]));
	while (crd[Y] < info->img->size[Y])
	{
		while (crd[X] < info->img->size[X])
		{
			scaled_crd[X] = crd[X] / info->img_zoom; //PLus centered screen offset
			//scaled_crd[X] += info->img[1].size_line;
			scaled_crd[Y] = crd[Y] / info->img_zoom;
			
			*(unsigned int * )(info->img->addr + (crd[X] * sizeof(int)) + (crd[Y] * info->img->size_line)) = *(unsigned int *)(info->img[1].addr + (scaled_crd[X] * sizeof(int) + info->img[1].size_line / 4) + (scaled_crd[Y] * info->img[1].size_line));
			crd[X]++;
		}
		crd[X] = 0;
		crd[Y]++;
	}
}