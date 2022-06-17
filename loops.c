/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loops.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 23:58:16 by okinnune          #+#    #+#             */
/*   Updated: 2022/06/17 10:28:05 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static void	update_julia_cursor(t_mlx_info *info)
{
	int	mouse[2];

	if (info->julia_toggle)
	{
		ft_bzero(mouse, sizeof(int [2]));
		mlx_mouse_get_pos(info->win, &mouse[X], &mouse[Y]);
		info->julia_pos[X] = (float)mouse[X] * 2 / (float)WSZ;
		info->julia_pos[Y] = (float)mouse[Y] * 2 / (float)WSZ;
		info->action = ACTION_CLICK;
		set_t_arg_finished(*info, FALSE);
	}
}

int	loop(void *p)
{
	t_mlx_info	*info;

	info = (t_mlx_info *)p;
	update_julia_cursor(info);
	info->img_zoom += info->zoom_acc;
	info->zoom_acc *= (info->zoom_acc >= 0.02) * ZOOM_DECELERATION;
	if (info->img_zoom > 1.5 && threads_done(*info))
	{
		info->zoom *= 2.0;
		info->pos[X] += (WSZ / 2) / info->zoom;
		info->pos[Y] += (WSZ / 2) / info->zoom;
		set_t_arg_finished(*info, FALSE);
		info->action = ACTION_ZOOM_IN;
	}
	update_t_args(*info);
	if (!threads_done(*info))
		mt_draw(*info, info->action);
	sample_image(info);
	mlx_put_image_to_window(info->mlx, info->win, info->img->ptr, 0, 0);
	mlx_do_sync(info->mlx);
	return (1);
}

int	key_loop(int keycode, void *p)
{
	t_mlx_info		*i;

	if (keycode == KEY_ESC)
		exit(0);
	i = (t_mlx_info *)p;
	i->colormode += (i->colormode < greenhell && keycode == KEY_RGHT);
	i->colormode -= (i->colormode > vanilla && keycode == KEY_LEFT);
	if (keycode == KEY_UP)
		i->color_bit_offset += 8;
	if (keycode == KEY_DOWN)
		i->color_bit_offset -= 8;
	if (keycode == KEY_Z)
		i->color_add -= 5;
	if (keycode == KEY_X)
		i->color_add += 1;
	i->color_bit_offset = ft_clamp(i->color_bit_offset, 0, 16);
	i->color_add = ft_clamp(i->color_add, 0, 10);
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
		if (info->img_zoom <= 0.5 && info->zoom > 20 && threads_done(*info))
		{
			info->pos[X] -= (WSZ / 2) / (info->zoom);
			info->pos[Y] -= (WSZ / 2) / (info->zoom);
			info->zoom *= 0.5;
			info->action = ACTION_ZOOM_OUT;
			set_t_arg_finished(*info, FALSE);
		}
	}
	info->zoom_acc += 0.025 * (button == SCRL_UP && info->zoom < ZOOM_LIMIT);
	if (button == 1 && threads_done(*info))
	{
		info->pos[X] += ((x - (WSZ / 2)) / info->zoom) / info->img_zoom;
		info->pos[Y] += ((y - (WSZ / 2)) / info->zoom) / info->img_zoom;
		info->action = ACTION_CLICK;
		set_t_arg_finished(*info, FALSE);
	}
	update_t_args(*info);
	return (1);
}
