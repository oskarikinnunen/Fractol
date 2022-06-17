/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 23:57:12 by okinnune          #+#    #+#             */
/*   Updated: 2022/06/17 09:57:32 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

double	time_elapsed(struct timeval t1)
{
	struct timeval	t2;

	gettimeofday(&(t2), NULL);
	if (gettimeofday(&(t2), NULL) <= -1)
		exit(0);
	return (t2.tv_sec - t1.tv_sec + ((t2.tv_usec - t1.tv_usec) / 1000000.0));
}

void	set_fractal_pixel(long double *crd, t_thread_arg *arg)
{
	t_complex	c;

	c.real = (arg->pos[X] - ((WSZ / 2) / arg->zoom)) + (crd[X] / arg->zoom);
	c.imaginary = (arg->pos[Y] - ((double)(WSZ / 2) / arg->zoom))
		+ (crd[Y] / arg->zoom);
	set_img_pixel(*arg->local_img, crd[X], crd[Y],
		arg->fptr((long double [4]){c.real, c.imaginary,
			arg->julia_pos[X], arg->julia_pos[Y]}));
}

void	cpy_thread_local_image(t_mlx_info info, int action)
{
	int				t_i;
	t_thread_arg	arg;

	arg = info.t_args[0];
	if (threads_done(info))
	{
		t_i = 0;
		while (t_i < info.thread_count)
		{
			arg = info.t_args[t_i];
			ft_memcpy(arg.img->addr + (sizeof(int) * arg.startpixel),
				arg.local_img->addr + (sizeof(int) * arg.startpixel),
				sizeof(int) * (arg.endpixel - arg.startpixel));
			t_i++;
		}
		if (action == ACTION_ZOOM_IN)
			*arg.img_zoom = ft_clampf(*arg.img_zoom * 0.5, 0.5, 10000.0);
		else if (action == ACTION_ZOOM_OUT)
			*arg.img_zoom = 1.0;
	}
}

int	threads_done(t_mlx_info info)
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

void	update_t_args(t_mlx_info info)
{
	int	i;

	i = 0;
	while (i < info.thread_count)
	{
		info.t_args[i].zoom = info.zoom;
		ft_memcpy(info.t_args[i].julia_pos, info.julia_pos, sizeof(float [2]));
		ft_memcpy(info.t_args[i].pos, info.pos, sizeof(long double [2]));
		i++;
	}
}
