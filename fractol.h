/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 12:08:57 by okinnune          #+#    #+#             */
/*   Updated: 2022/06/16 13:00:36 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "mlx.h"
# include <math.h>
# include "libft.h"
# include <pthread.h>
# include <sys/time.h>

//REMOVE
# include <stdio.h>

# define WSZ 512
# define MAX_ITERS 3
# define FALSE 0
# define TRUE 1
# define X 0
# define Y 1
# define INT_MAX 2147483647

/*	KEYCODES */
# ifdef __APPLE__
#  define KEY_LEFT 123
#  define KEY_RGHT 124
#  define KEY_DOWN 125
#  define KEY_UP 126
#  define KEY_Z 6
#  define KEY_X 7
#  define KEY_W 13
#  define KEY_ESC 53
# else
/* ASSUMED LINUX */
#  define KEY_LEFT 65361
#  define KEY_RGHT 65363
#  define KEY_DOWN 65364
#  define KEY_UP 65362
#  define KEY_Z 122
#  define KEY_X 120
#  define KEY_W 119
#  define KEY_ESC 65307
# endif

# define SCRL_DOWN 5
# define SCRL_UP 4
# define ACTION_ZOOM_IN 0
# define ACTION_ZOOM_OUT 1
# define ACTION_CLICK 2


typedef struct s_complex
{
	long double	real;
	long double	imaginary;
}	t_complex;

typedef struct s_image_info
{
	void	*ptr;
	char	*addr;
	int		bpp;
	int		size_line;
	int		endian;
	int		size[2];
}	t_image_info;

//Threads populate the fake image, which then gets sample with pos and zoom to the "real image"

//TODO: draw zoom image target on screen as a square, redraw the actual 2x image once we reach that
//2 zoom variables, one for the actual mandelbrot zoom and one for the "digital" zoom that we do on the copy image

// Full size image, actually sampled image, and the blurry copy
typedef struct s_thread_arg
{
	t_image_info		*img;
	t_image_info		*local_img;
	int					startpixel;
	long double			pixelcrd[2];
	_Bool				finished;
	int					endpixel;
	float				*img_zoom;
	float				julia_pos[2];
	long double			zoom;
	long double 		pos[2];
}	t_thread_arg;

typedef struct s_mlx_info
{
	void				*mlx;
	void				*win;
	t_image_info		*img;
	pthread_t			*threads;
	t_thread_arg		*t_args;
	int					thread_count;
	float				julia_pos[2];
	long double			pos[2];
	long double			zoom;
	long double			target_zoom;
	float				img_zoom;
	bool				anim_toggle;
	int					action;
}	t_mlx_info;

int				get_pixel_color(float z);
void			populate_threadinfo(t_mlx_info *info);
void			set_t_arg_finished(t_mlx_info info, _Bool b);
int				thread_done(t_mlx_info info);
void			update_t_args(t_mlx_info info);
void			mt_draw(t_mlx_info info, int zoom_in);
void			sample_image(t_mlx_info *info);
unsigned int	get_img_pixel(t_image_info img, int x, int y);
void			set_img_pixel(t_image_info img, int x, int y, unsigned int color);

# endif