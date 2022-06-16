/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 12:26:44 by okinnune          #+#    #+#             */
/*   Updated: 2022/06/16 13:04:52 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

#include <assert.h>
#include "libft_g.h"
#include <limits.h>
#include <math.h>

# define DARK_ORANGE (255 << 16) + (140 << 8)
# define LGHT_ORANGE (255 << 16) + (180 << 8)
# define WHITE INT_MAX
# define LGHT_BLUE (173 << 16) + (50 << 8) + 245
# define DARK_BLUE 139

int	get_pixel_color(float z)
{
	int	lcolor;
	static int color[5] =
	{
		DARK_ORANGE, LGHT_ORANGE, LGHT_BLUE, DARK_BLUE, WHITE
	};
	
	//printf("lerp test %f \n", ft_lerpf(100,50, 0.2));
	
	//z = ft_clampf(z, 0, 3);
	if (z + 1 > 4.0)
	{
		z -= 4.0;
	}
	//z = ft_clampf(z, 0, 3);
	lcolor = g_colorlerp(color[(int)z], color[((int)z) + 1], z - (int)z);
	return (lcolor);
}

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

static int	loop(void *p)
{
	t_mlx_info			*info;
	bool				zoom_in;

	info = (t_mlx_info *)p;
	if (info->img_zoom > 1.5 && thread_done(*info))
	{
		info->zoom *= 2.0; //Multiply until it's the "final" multiplier, corresponding action in thread done
		info->pos[X] += (WSZ / 2) / info->zoom;
		info->pos[Y] += (WSZ / 2) / info->zoom;
		update_t_args(*info);
		set_t_arg_finished(*info, FALSE);
		info->action = ACTION_ZOOM_IN;
	}
	if (!thread_done(*info))
		mt_draw(*info, info->action);
	sample_image(info);
	mlx_put_image_to_window(info->mlx, info->win, info->img->ptr, 0, 0);
	mlx_do_sync(info->mlx);
	return (1);
}

float	g_color_add;

static int	key_loop(int keycode, void *p)
{
	t_mlx_info		*i;

	if (keycode == KEY_ESC)
		exit(0);
	i = (t_mlx_info *)p;
	if (keycode == KEY_Z)
		g_color_add *= 1.1;
	if (keycode == KEY_X)
		g_color_add *= 0.9;
	update_t_args(*i);
	return (1);
}

int	mouse_hook(int button, int x, int y, void *p)
{
	t_mlx_info		*info;

	info = (t_mlx_info *)p;
	info->julia_pos[X] = x;
	info->julia_pos[Y] = y;
	if (button == SCRL_DOWN)
	{
		info->img_zoom = ft_clampf(info->img_zoom - 0.25, 0.5, 1.5);
		if (info->img_zoom <= 0.5 && info->zoom > 20 && thread_done(*info)) {
			info->pos[X] -= (WSZ / 2) / (info->zoom); // info->img_zoom;
			info->pos[Y] -= (WSZ / 2) / (info->zoom); // info->img_zoom;
			info->zoom *= 0.5;
			info->action = ACTION_ZOOM_OUT;
			update_t_args(*info);
			set_t_arg_finished(*info, FALSE);
		}
	}
	info->img_zoom += 0.5 * (button == SCRL_UP);
	if (button == 1 && thread_done(*info))
	{
		info->pos[X] += ((x - (WSZ / 2)) / info->zoom) / info->img_zoom;
		info->pos[Y] += ((y - (WSZ / 2)) / info->zoom) / info->img_zoom;
		printf("x %Lf y %Lf \n", info->pos[X], info->pos[Y]);
		info->action = ACTION_CLICK;
		set_t_arg_finished(*info, FALSE);
	}
	update_t_args(*info);
	return (1);
}

static void start_mlx(t_mlx_info *info)
{
	//
	info->mlx = mlx_init();
	info->win = mlx_new_window(info->mlx, WSZ, WSZ, "new_window");
	info->img->ptr = mlx_new_image(info->mlx, WSZ, WSZ);
	info->img->addr = mlx_get_data_addr(info->img->ptr, (int *)&(info->img->bpp),
			(int *)&(info->img->size_line), &(info->img->endian));
	ft_memcpy(info->img->size, (int [2]) {WSZ, WSZ}, sizeof (int [2]));
	ft_memcpy(&info->img[1], &info->img[0], sizeof(t_image_info));
	ft_memcpy(info->img[1].size, (int [2]) {WSZ * 2, WSZ * 2}, sizeof (int [2]));
	info->img[1].size_line *= 2;
	info->img[1].addr = ft_memalloc(info->img[1].size_line * WSZ * 4);

	info->zoom = 100.0;
	info->img_zoom = 1.5;
	ft_bzero(info->pos, sizeof(long double [2]));
	populate_threadinfo(info);
	printf("threadcount after pop %i \n", info->thread_count);
	printf("after pop start 1 %i \n", info->t_args[0].endpixel);
	mlx_loop_hook(info->mlx, loop, info);
	mlx_key_hook(info->win, key_loop, info);
	mlx_mouse_hook(info->win, mouse_hook, info);
	mlx_loop(info->mlx);
}

int	main(void)
{
	t_complex	c;
	t_image_info img[2];
	t_mlx_info info;

	ft_bzero(&info, sizeof(t_mlx_info));
	info.img = img;
	
	start_mlx(&info);
}