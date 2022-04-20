/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 12:26:44 by okinnune          #+#    #+#             */
/*   Updated: 2022/04/20 17:14:51 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

#include <assert.h>
#include "libft_g.h"
#include <limits.h>
# define MAX_ITERS 40

int	get_pixel_color(int z)
{
	int	c1;
	int	c2;
	int	c3;
	int	range;
	int	color;

	range = 100;

	
	//(float)ft_max(0, range - ft_abs(z - 0)) / 255.0;
	/*r = ft_max(0, range - ft_abs(z - 0));
	g = ft_max(0, range - ft_abs(z - 127));
	b = ft_max(0, range - ft_abs(z - 255));*/
	//color = b + (g << 8) + (r << 16);
	return (color);
}

static void	fill_mandelbrot(t_mlx_info info)
{
	int			crd[2];
	t_complex	c;
	t_complex	f;
	t_complex	prev;
	int			color;

	ft_bzero(&c, sizeof(t_complex));
	//ft_bzero(f, sizeof(t_complex));
	ft_bzero(crd, sizeof(int [2]));
	while(crd[Y] < WSZ)
	{
		while(crd[X] < WSZ)
		{
			c.real = crd[X] - (WSZ / 2);
			c.imaginary = crd[Y] - (WSZ / 2);
			c.real *= 0.004;
			c.imaginary *= 0.004;
			ft_bzero(&f, sizeof(t_complex));
			color = 0;
			while (ft_absd(f.real * f.real + f.imaginary * f.imaginary) < 4 && color < MAX_ITERS)
			{
				ft_memcpy(&prev, &f, sizeof(t_complex));
				f.imaginary = 2 * f.real * f.imaginary + c.imaginary;
				f.real = (prev.real * prev.real) - (prev.imaginary * prev.imaginary) + c.real;
				color++;
			}
			color = color * ( (INT_MAX * 0.75) / MAX_ITERS);
			//assert(color >= 0 && color <= 255);
			//color = get_pixel_color(color);
			if (crd[Y] == 10)
				color = 255;
			*(int *)(info.img->addr + (crd[X] * sizeof(int)) + (crd[Y] * info.img->size_line)) = color;
			crd[X]++;
		}
		
		//printf("y %i \n", crd[Y]);
		crd[X] = 0;
		crd[Y]++;
	}
}

static int	loop(void *p)
{
	t_mlx_info			*info;
	

	info = (t_mlx_info *)p;
	ft_bzero(info->img->addr, WSZ * WSZ * sizeof(int));
	fill_mandelbrot(*info);
	printf("mandelbrot done");
	mlx_put_image_to_window(info->mlx, info->win, info->img->ptr, 0, 0);
	mlx_do_sync(info->mlx);
	return (1);
}

static void start_mlx(t_mlx_info *info, t_image_info *img)
{
	info->mlx = mlx_init();
	info->win = mlx_new_window(info->mlx, WSZ, WSZ, "new_window");
	img->ptr = mlx_new_image(info->mlx, WSZ, WSZ);
	img->addr = mlx_get_data_addr(img->ptr, (int *)&(img->bpp),
			(int *)&(img->size_line), &(img->endian));
	info->img = img;
	mlx_loop_hook(info->mlx, loop, info);
	mlx_loop(info->mlx);
}

int	main(void)
{
	t_complex	c;
	t_image_info img;
	t_mlx_info info;

	start_mlx(&info, &img);
	while (1)
		;
}