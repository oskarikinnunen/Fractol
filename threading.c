/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 15:08:30 by okinnune          #+#    #+#             */
/*   Updated: 2022/06/16 16:24:26 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	populate_threadinfo(t_mlx_info *info)
{
	int	t_i;
	int	image_length;

	image_length = WSZ * WSZ * 4;
	info->thread_count = (int)sysconf(_SC_NPROCESSORS_ONLN) /** 2*/;
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
		if (t_i == info->thread_count - 1)
			info->t_args[t_i].endpixel = image_length;
		printf("targ %i start %i end %i \n", t_i, info->t_args[t_i].startpixel, info->t_args[t_i].endpixel);
		info->t_args[t_i].img = &info->img[1];
		info->t_args[t_i].zoom = info->zoom;
		info->t_args[t_i].img_zoom = &info->img_zoom;
		ft_memcpy(info->t_args[t_i].pos, info->pos, sizeof (long double [2]));
		//Malloc local cpy image
		info->t_args[t_i].local_img = ft_memalloc(sizeof(t_image_info)); //TODO: protec
		ft_memcpy(info->t_args[t_i].local_img, &info->img[1], sizeof(t_image_info));
		info->t_args[t_i].local_img->addr = ft_memalloc(sizeof(int) * info->img[1].size[X] * info->img[1].size[Y]);
		t_i++;
	}
}

static int	julia(t_complex c, float *pos)
{
	int			color;
	t_complex	f;
	t_complex	prev;

	ft_memcpy(&f, &c, sizeof(t_complex));
	color = 0;
	while (ft_absd(f.real * f.real + f.imaginary * f.imaginary) < 8 && color < MAX_ITERS)
	{
		ft_memcpy(&f, &c, sizeof(t_complex));
		//ft_memcpy(&prev, &f, sizeof(t_complex));
		c.real = f.real * f.real - f.imaginary * f.imaginary + pos[X];
		c.imaginary = 2 * f.real * f.imaginary + pos[Y];
		color++;
	}

	//return ((c.real / c.imaginary) / WSZ * color * ((INT_MAX * 0.75) / MAX_ITERS));
	return (color * ((INT_MAX * 0.75) / MAX_ITERS));
	//return (get_pixel_color(color));
}

static int	mandelbrot(t_complex c)
{
	int			color;
	t_complex	f;
	t_complex	prev;

	ft_bzero(&f, sizeof(t_complex));
	color = 0;
	while (ft_absd(f.real * f.real + f.imaginary * f.imaginary) < 4 && color < MAX_ITERS)
	{
		ft_memcpy(&prev, &f, sizeof(t_complex));
		f.real = (f.real * f.real) - (f.imaginary * f.imaginary) + c.real;
		f.imaginary = 2 * prev.real * prev.imaginary + c.imaginary;
		//color += g_color_add;
		color++;
	}
	return (color);
}

static int	ship(t_complex c)
{
	int			color;
	t_complex	f;
	t_complex	prev;

	ft_bzero(&f, sizeof(t_complex));
	color = 0;
	while (f.real * f.real + f.imaginary * f.imaginary < 4 && color < MAX_ITERS)
	{
		ft_memcpy(&prev, &f, sizeof(t_complex));
		f.real = (f.real * f.real) - (f.imaginary * f.imaginary) + c.real;
		f.imaginary = 2 * prev.real * prev.imaginary + c.imaginary;
		f.real = ft_absd(f.real);
		f.imaginary = ft_absd(f.imaginary);
		color++;
	}
	return (color * ((INT_MAX * 0.75) / MAX_ITERS));
}

static double	time_elapsed(struct timeval t1)
{
	struct timeval	t2;

	gettimeofday(&(t2), NULL);
	if (gettimeofday(&(t2), NULL) <= -1)
		exit(0);
	return (t2.tv_sec - t1.tv_sec + ((t2.tv_usec - t1.tv_usec) / 1000000.0));
}

static void	*fill_fractal_mt(void *v_arg) //Use local image instead
{
	long double		crd[2];
	int				pixelcount;
	t_thread_arg	*arg;
	t_complex		c;
	struct timeval	tm;

	arg = (t_thread_arg *)v_arg;
	ft_bzero(crd, sizeof(long double [2]));
	ft_memcpy(crd, arg->pixelcrd, sizeof(long double [2]));
	gettimeofday(&tm, NULL);
	while ((arg->img->size[X] * crd[Y]) + crd[X] <= arg->endpixel)
	{
		while (crd[X] < arg->img->size[X] &&
			(arg->img->size[X] * crd[Y]) + crd[X] <= arg->endpixel)
		{
			if ((arg->img->size[X] * crd[Y]) + crd[X] >= arg->startpixel)
			{
				c.real = (arg->pos[X] - ((double)(WSZ / 2) / arg->zoom)) + (crd[X]  / arg->zoom);
				c.imaginary = (arg->pos[Y] - ((double)(WSZ / 2) / arg->zoom)) + (crd[Y]  / arg->zoom);
				//set_img_pixel(*arg->local_img, crd[X], crd[Y], mandelbrot(c));
				set_img_pixel(*arg->local_img, crd[X], crd[Y], julia(c, arg->julia_pos));
				//set_img_pixel(*arg->local_img, crd[X], crd[Y], ship(c));
			}
			crd[X]++;
		}
		crd[X] = 0;
		crd[Y]++;
		if (time_elapsed(tm) > 0.06 && crd[X] + (crd[Y] * arg->img->size[X]) < arg->endpixel) // Just save the pixel coordinate
		{
			ft_memcpy(arg->pixelcrd, crd, sizeof(long double [2]));
			return (NULL);
		}
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

void	update_t_args(t_mlx_info info)
{
	int	i;

	i = 0;
	while (i < info.thread_count)
	{
		info.t_args[i].zoom = info.zoom;
		ft_memcpy(info.t_args[i].julia_pos, info.julia_pos, sizeof(float [2]));
		ft_memcpy(info.t_args[i].pos, info.pos, sizeof(long double [2]));
		
		//info.t_args[i].curpixel = 0;
		//ft_bzero(info.t_args[i].pixelcrd, sizeof(long double [2]));
		i++;
	}
}

void	mt_draw(t_mlx_info info, int action)
{
	int	t_i;
	t_thread_arg arg;

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
	
	if (thread_done(info))
	{
		t_i = 0;
		while (t_i < info.thread_count) {
			arg = info.t_args[t_i];
			ft_memcpy(arg.img->addr + (sizeof(int) * arg.startpixel),
				arg.local_img->addr + (sizeof(int) * arg.startpixel),
				sizeof(int) * (arg.endpixel - arg.startpixel));
			t_i++;
		}
		if (action == ACTION_ZOOM_IN) 
		{
			*arg.img_zoom = ft_clampf(*arg.img_zoom * 0.5, 0.5, 10000.0);
		}
			
		else if (action == ACTION_ZOOM_OUT)
			*arg.img_zoom = 1.0;
	}
}