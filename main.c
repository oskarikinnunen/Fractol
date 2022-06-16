/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 12:26:44 by okinnune          #+#    #+#             */
/*   Updated: 2022/06/17 01:01:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static void	start_mlx(t_mlx_info *info)
{
	info->mlx = mlx_init();
	info->win = mlx_new_window(info->mlx, WSZ, WSZ, "new_window");
	info->img->ptr = mlx_new_image(info->mlx, WSZ, WSZ);
	info->img->addr = mlx_get_data_addr(info->img->ptr,
			(int *)&(info->img->bpp),
			(int *)&(info->img->size_line), &(info->img->endian));
	ft_memcpy(info->img->size, (int [2]){WSZ, WSZ}, sizeof (int [2]));
	ft_memcpy(&info->img[1], &info->img[0], sizeof(t_image_info));
	ft_memcpy(info->img[1].size, (int [2]){WSZ * 2, WSZ * 2}, sizeof (int [2]));
	info->img[1].size_line *= 2;
	info->img[1].addr = ft_memalloc(info->img[1].size_line * WSZ * 4);
	info->zoom = 50.0;
	info->img_zoom = 1.51;
	info->pos[X] -= WSZ / 2 / info->zoom;
	info->pos[Y] -= WSZ / 2 / info->zoom;
	populate_threadinfo(info);
	mlx_loop_hook(info->mlx, loop, info);
	mlx_key_hook(info->win, key_loop, info);
	mlx_mouse_hook(info->win, mouse_hook, info);
	mlx_loop(info->mlx);
}

int	main(int argc, char **argv)
{
	t_image_info	img[2];
	t_mlx_info		info;

	ft_bzero(&info, sizeof(t_mlx_info));
	if (argc == 2 && evaluate_arg(argv[1], &info))
	{
		info.img = img;
		info.colormode = vanilla;
		info.color_add = 1;
		start_mlx(&info);
	}
	else
	{
		ft_putstr(USAGE_MSG);
	}
}
