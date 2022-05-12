/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 15:08:30 by okinnune          #+#    #+#             */
/*   Updated: 2022/05/12 13:16:22 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	populate_threadinfo(t_mlx_info *info)
{
	int	t_i;
	int	image_length;

	image_length = WSZ * WSZ;
	info->thread_count = (int)sysconf(_SC_NPROCESSORS_ONLN);
	printf("thread count %i \n", info->thread_count);
	if (info->thread_count > 0)
	{
		info->threads = (pthread_t *)malloc(info->thread_count * sizeof(pthread_t));
		info->t_args = (t_thread_arg *)malloc
			(info->thread_count * sizeof(t_thread_arg));
	}
	t_i = 0;
	while (t_i < info->thread_count)
	{
		info->t_args[t_i].startpixel = t_i * (image_length / info->thread_count);
		info->t_args[t_i].endpixel = (t_i + 1) * (image_length / info->thread_count);
		printf("targ %i start %i end %i \n", t_i, info->t_args[t_i].startpixel, info->t_args[t_i].endpixel);
		info->t_args[t_i].img = info->img;
		info->t_args[t_i].zoom = info->zoom; //TODO: update in loop
		ft_memcpy(info->t_args[t_i].pos, info->pos, sizeof (long double [2]));
		t_i++;
	}
}

float	g_color_add = 0.05;

static int	mandelbrot(t_complex c)
{
	float			color;
	t_complex	f;
	t_complex	prev;

	ft_bzero(&f, sizeof(t_complex));
	color = 0;
	while (ft_absd(f.real * f.real + f.imaginary * f.imaginary) < 4 && color < MAX_ITERS)
	{
		ft_memcpy(&prev, &f, sizeof(t_complex));
		f.real = (f.real * f.real) - (f.imaginary * f.imaginary) + c.real;
		f.imaginary = 2 * prev.real * prev.imaginary + c.imaginary;
		color += g_color_add;
	}
	return (get_pixel_color(color));
}

#include <assert.h>

static void	*fill_fractal_mt(void *v_arg)
{
	long double		crd[2];
	int				pixelcount;
	t_thread_arg	*arg;
	t_complex		c;

	arg = (t_thread_arg *)v_arg;
	ft_bzero(&c, sizeof(t_complex));
	ft_bzero(crd, sizeof(long double [2]));
	pixelcount = 0;
	float g_scale = 0.1;
	while(crd[Y] < WSZ && pixelcount <= arg->endpixel)
	{
		while(crd[X] < WSZ && pixelcount <= arg->endpixel)
		{
			pixelcount++;
			if (pixelcount < arg->startpixel)
			{
				crd[X]++; //Fix for norminette, make more compact
				continue;
			}
			c.real = (arg->pos[X] - ((double)(WSZ / 2) / arg->zoom)) + (crd[X]  / arg->zoom);
			c.imaginary = (arg->pos[Y] - ((double)(WSZ / 2) / arg->zoom)) + (crd[Y]  / arg->zoom);
			c.real *= g_scale;
			c.imaginary *= g_scale;
			*(unsigned int *)(arg->img->addr + (((int)crd[X]) * sizeof(int)) + (((int)crd[Y])
					* arg->img->size_line)) = mandelbrot(c);
			crd[X]++;
		}
		crd[X] = 0;
		pixelcount++;
		crd[Y]++;
	}
	return (NULL);
}

void	update_t_args(t_mlx_info info)
{
	int	i;

	i = 0;
	while (i < info.thread_count)
	{
		info.t_args[i].zoom = info.zoom;
		ft_memcpy(info.t_args[i].pos, info.pos, sizeof(long double [2]));
		i++;
	}
}

void	mt_draw(t_mlx_info info)
{
	int	t_i;

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
	//printf("join \n");
}