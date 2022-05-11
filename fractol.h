/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 12:08:57 by okinnune          #+#    #+#             */
/*   Updated: 2022/05/11 11:55:45 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "mlx.h"
# include "libft.h"
# include <pthread.h>

//REMOVE
# include <stdio.h>

# define WSZ 1024
# define MAX_ITERS 4
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

typedef struct s_complex
{
	unsigned long double	real;
	unsigned long double	imaginary;
}	t_complex;

typedef struct s_image_info
{
	void	*ptr;
	char	*addr;
	int		bpp;
	int		size_line;
	int		endian;
}	t_image_info;

typedef struct s_thread_arg
{
	t_image_info	*img;
	int				startpixel;
	int				endpixel;
	float			zoom;
	float			pos[2];
}	t_thread_arg;

typedef struct s_hugefloat
{
	void	*exponent;
	void	*significand;
}	t_hugefloat;

typedef struct s_mlx_info
{
	void				*mlx;
	void				*win;
	t_image_info		*img;
	pthread_t			*threads;
	t_thread_arg		*t_args;
	int					thread_count;
	float				pos[2]; // Convert to origin point coordinate
	float				zoom;
}	t_mlx_info;

int		get_pixel_color(float z);
void	populate_threadinfo(t_mlx_info *info);
void	update_t_args(t_mlx_info info);
void	mt_draw(t_mlx_info info);

# endif