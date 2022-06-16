/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 12:26:44 by okinnune          #+#    #+#             */
/*   Updated: 2022/06/16 16:23:33 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	thread_done(t_mlx_info info)
{
	int		i;
	_Bool	result;

	result = TRUE;
	i = 0;
	while (i < info.thread_count)
	{
		if (!info.t_args[i].finished)
			result = FALSE;
		i++;
	}
	return (result);
}

static void	update_julia_cursor(t_mlx_info *info)
{
	int	mouse[2];

	if (info->julia_toggle)
	{
		mlx_mouse_get_pos(info->win, &mouse[X], &mouse[Y]);
		info->julia_pos[X] = (float)mouse[X] * 2 / (float)WSZ;
		info->julia_pos[Y] = (float)mouse[Y] * 2 / (float)WSZ;
		info->action = ACTION_CLICK;
		set_t_arg_finished(*info, FALSE);
	}
}

static int	loop(void *p)
{
	t_mlx_info	*info;

	info = (t_mlx_info *)p;
	update_julia_cursor(info);
	info->img_zoom += info->zoom_acc;
	info->zoom_acc *= (info->zoom_acc >= 0.02) * ZOOM_DECELERATION;
	if (info->img_zoom > 1.5 && thread_done(*info))
	{
		info->zoom *= 2.0;
		info->pos[X] += (WSZ / 2) / info->zoom;
		info->pos[Y] += (WSZ / 2) / info->zoom;
		set_t_arg_finished(*info, FALSE);
		info->action = ACTION_ZOOM_IN;
	}
	update_t_args(*info);
	if (!thread_done(*info))
		mt_draw(*info, info->action);
	sample_image(info);
	mlx_put_image_to_window(info->mlx, info->win, info->img->ptr, 0, 0);
	mlx_do_sync(info->mlx);
	return (1);
}

static int	key_loop(int keycode, void *p)
{
	t_mlx_info		*i;

	if (keycode == KEY_ESC)
		exit(0);
	i = (t_mlx_info *)p;
	if (keycode == KEY_Z)
		i->color_offset += 2047;
	if (keycode == KEY_X)
		i->color_offset -= 2048;
	if (keycode == KEY_W)
		i->julia_toggle = !i->julia_toggle;
	update_t_args(*i);
	return (1);
}

int	mouse_hook(int button, int x, int y, void *p)
{
	t_mlx_info		*info;

	info = (t_mlx_info *)p;
	if (button == SCRL_DOWN)
	{
		info->img_zoom = ft_clampf(info->img_zoom - 0.25, 0.5, 1.5);
		if (info->img_zoom <= 0.5 && info->zoom > 20 && thread_done(*info))
		{
			info->pos[X] -= (WSZ / 2) / (info->zoom);
			info->pos[Y] -= (WSZ / 2) / (info->zoom);
			info->zoom *= 0.5;
			info->action = ACTION_ZOOM_OUT;
			set_t_arg_finished(*info, FALSE);
		}
	}
	info->zoom_acc += 0.025 * (button == SCRL_UP && info->zoom < ZOOM_LIMIT);
	if (button == 1 && thread_done(*info))
	{
		info->pos[X] += ((x - (WSZ / 2)) / info->zoom) / info->img_zoom;
		info->pos[Y] += ((y - (WSZ / 2)) / info->zoom) / info->img_zoom;
		info->action = ACTION_CLICK;
		set_t_arg_finished(*info, FALSE);
	}
	update_t_args(*info);
	return (1);
}

static void	start_mlx(t_mlx_info *info)
{
	info->mlx = mlx_init();
	info->win = mlx_new_window(info->mlx, WSZ, WSZ, "new_window");
	info->img->ptr = mlx_new_image(info->mlx, WSZ, WSZ);
	info->img->addr = mlx_get_data_addr(info->img->ptr, (int *)&(info->img->bpp),
			(int *)&(info->img->size_line), &(info->img->endian));
	ft_memcpy(info->img->size, (int [2]){WSZ, WSZ}, sizeof (int [2]));
	ft_memcpy(&info->img[1], &info->img[0], sizeof(t_image_info));
	ft_memcpy(info->img[1].size, (int [2]){WSZ * 2, WSZ * 2}, sizeof (int [2]));
	info->img[1].size_line *= 2;
	info->img[1].addr = ft_memalloc(info->img[1].size_line * WSZ * 4);
	info->zoom = 100.0;
	info->img_zoom = 1.5;
	ft_bzero(info->pos, sizeof(long double [2]));
	populate_threadinfo(info);
	mlx_loop_hook(info->mlx, loop, info);
	mlx_key_hook(info->win, key_loop, info);
	mlx_mouse_hook(info->win, mouse_hook, info);
	mlx_loop(info->mlx);
}

int	main(void)
{
	t_complex		c;
	t_image_info	img[2];
	t_mlx_info		info;

	ft_bzero(&info, sizeof(t_mlx_info));
	info.img = img;
	start_mlx(&info);
}
