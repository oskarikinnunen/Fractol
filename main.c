/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 12:26:44 by okinnune          #+#    #+#             */
/*   Updated: 2022/05/11 10:06:56 by okinnune         ###   ########.fr       */
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
# define LGHT_BLUE (173 << 16) + (174 << 8) + 245
# define DARK_BLUE 139

int	get_pixel_color(float z)
{
	int	lcolor;
	static int color[5] =
	{
		DARK_ORANGE, LGHT_ORANGE, LGHT_BLUE, DARK_BLUE, 0
	};

	
	//printf("lerp test %f \n", ft_lerpf(100,50, 0.2));
	//printf("color1 %i color2 %i lerp %f \n", (int)z, (int)z + 1, z- (int)z);
	lcolor = g_colorlerp(color[(int)z], color[((int)z) + 1], z- (int)z);
	return (lcolor);
}

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
		color += 1.0;
	}
	color = color * (255 / MAX_ITERS);
	color = color / 51.0;
	//printf("z lerp %f \n", color);
	//assert(color >= 0 && color <= 255);
	return (get_pixel_color(color));
}

static int	loop(void *p)
{
	t_mlx_info			*info;

	info = (t_mlx_info *)p;
	//ft_bzero(info->img->addr, WSZ * WSZ * sizeof(int));
	//fill_mandelbrot(*info);

	mt_draw(*info);
	//printf("mandelbrot done ?\n");
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
	i->pos[X] -= (keycode == KEY_LEFT) * 800000 * i->zoom;
	i->pos[X] += (keycode == KEY_RGHT) * 800000 * i->zoom;
	i->pos[Y] -= (keycode == KEY_UP) * 800000 * i->zoom;;
	i->pos[Y] += (keycode == KEY_DOWN) * 800000 * i->zoom;
	update_t_args(*i);
	return (1);
}

int	mouse_hook(int button, int x, int y, void *p)
{
	t_mlx_info		*i;

	i = (t_mlx_info *)p;
	printf("mouse button %i \n", button);
	if (button == SCRL_UP)
		i->zoom = i->zoom * 0.9;
	if (button == SCRL_DOWN)
	{
		printf("zoom %f \n", i->zoom);
		i->zoom = i->zoom * 1.1;
	}
		
	if (button == 1)
	{
		i->pos[X] += (x - (WSZ / 2)) / i->zoom;
		i->pos[Y] += (y - (WSZ / 2)) / i->zoom;
		printf("normalized mouse coord x %f \n", i->pos[X]);
		printf("normalized mouse coord y %f \n", i->pos[Y]);
	}
	update_t_args(*i);
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
	info->zoom = 100;
	ft_bzero(info->pos, sizeof(float [2]));
	//info->pos[X] -= WSZ / 2;
	//info->pos[Y] += WSZ / 2;
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
	t_image_info img;
	t_mlx_info info;

	start_mlx(&info, &img);
	while (1)
		;
}