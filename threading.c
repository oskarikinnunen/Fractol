/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 15:08:30 by okinnune          #+#    #+#             */
/*   Updated: 2022/06/17 10:35:26 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static void	allocate_thread_value(t_thread_arg *arg, t_mlx_info *info,
	int pixels[2])
{
	arg->startpixel = pixels[0];
	arg->endpixel = pixels[1];
	arg->img = &info->img[1];
	arg->zoom = info->zoom;
	arg->img_zoom = &info->img_zoom;
	arg->fptr = info->fptr;
	arg->finished = FALSE;
	ft_memcpy(arg->pos, info->pos, sizeof (long double [2]));
	arg->local_img = ft_memalloc(sizeof(t_image_info));
	if (arg->local_img == NULL)
		error_exit();
	ft_memcpy(arg->local_img, &info->img[1], sizeof(t_image_info));
	arg->local_img->addr = ft_memalloc(sizeof(int)
			* info->img[1].size[X] * info->img[1].size[Y]);
	if (arg->local_img->addr == NULL)
		error_exit();
}

void	populate_threadinfo(t_mlx_info *info)
{
	int	t_i;
	int	image_length;

	image_length = WSZ * WSZ * 4;
	info->thread_count = (int)sysconf(_SC_NPROCESSORS_ONLN);
	if (info->thread_count > 0)
	{
		info->threads = (pthread_t *)malloc(info->thread_count
				* sizeof(pthread_t));
		info->t_args = (t_thread_arg *)malloc
			(info->thread_count * sizeof(t_thread_arg));
	}
	t_i = 0;
	while (t_i < info->thread_count)
	{
		if (t_i != info->thread_count - 1)
			allocate_thread_value(&info->t_args[t_i], info,
				(int [2]){t_i * (image_length / info->thread_count),
				(t_i + 1) * (image_length / info->thread_count)});
		else
			allocate_thread_value(&info->t_args[t_i], info, (int [2]){t_i
				* (image_length / info->thread_count), image_length});
		t_i++;
	}
}

static void	*fill_fractal_mt(void *v_arg)
{
	long double		crd[2];
	t_thread_arg	*arg;
	struct timeval	tm;

	arg = (t_thread_arg *)v_arg;
	ft_memcpy(crd, arg->pixelcrd, sizeof(long double [2]));
	gettimeofday(&tm, NULL);
	while ((arg->img->size[X] * crd[Y]++) + crd[X] <= arg->endpixel)
	{
		while (crd[X]++ < arg->img->size[X]
			&& (arg->img->size[X] * crd[Y]) + crd[X] <= arg->endpixel)
		{
			if ((arg->img->size[X] * crd[Y]) + crd[X] >= arg->startpixel)
				set_fractal_pixel(crd, arg);
		}
		crd[X] = 0;
		if (time_elapsed(tm) > 0.06 && crd[X]
			+ (crd[Y] * arg->img->size[X]) < arg->endpixel)
			return (ft_memcpy(arg->pixelcrd, crd,
					sizeof(long double [2])));
	}
	ft_bzero(arg->pixelcrd, sizeof(long double [2]));
	arg->finished = TRUE;
	return (NULL);
}

void	set_t_arg_finished(t_mlx_info info, _Bool b)
{
	int	i;

	i = 0;
	while (i < info.thread_count)
	{
		info.t_args[i].finished = b;
		if (b == FALSE)
			ft_bzero(info.t_args[i].pixelcrd, sizeof(long double [2]));
		i++;
	}
}

void	mt_draw(t_mlx_info info, int action)
{
	int				t_i;

	t_i = 0;
	while (t_i < info.thread_count)
	{
		if (pthread_create(&info.threads[t_i], NULL, fill_fractal_mt,
				(void *)&(info.t_args[t_i])) != 0)
			exit(0);
		t_i++;
	}
	t_i = 0;
	while (t_i < info.thread_count)
	{
		pthread_join(info.threads[t_i], NULL);
		t_i++;
	}
	cpy_thread_local_image(info, action);
}
