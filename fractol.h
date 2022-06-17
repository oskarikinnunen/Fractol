/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 12:08:57 by okinnune          #+#    #+#             */
/*   Updated: 2022/06/17 11:19:34 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include <math.h>
# include "libft.h"
# include <pthread.h>
# include <sys/time.h>

# define WSZ 512
# define MAX_ITERS 220
# define ZOOM_LIMIT 112589990684262400.000000
# define ZOOM_DECELERATION 0.82
# define FALSE 0
# define TRUE 1
# define X 0
# define Y 1
# define INT_MAX 2147483647

/*	KEYCODES */
# ifdef __APPLE__
#  include "mlx/OS_X/mlx.h"
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
#  include "mlx/Linux/mlx.h"
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

# define USAGE_MSG "Usage: fractol [julia/mandelbrot/ship] \n\
	Use Arrow keys/Z/X to control the colors, mouse to move\n"

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

typedef struct s_thread_arg
{
	t_image_info		*img;
	t_image_info		*local_img;
	int					(*fptr)(long double *);
	int					startpixel;
	long double			pixelcrd[2];
	_Bool				finished;
	int					endpixel;
	float				*img_zoom;
	float				julia_pos[2];
	long double			zoom;
	long double			pos[2];
}	t_thread_arg;

typedef struct s_julia
{
	float	pos[2];

}	t_julia;

//Should be e_colormode but norminette is dumb
typedef enum e_colormode
{
	vanilla,
	blackandwhite,
	sine1,
	sine2,
	greenhell
}	t_colormode;

typedef struct s_mlx_info
{
	void				*mlx;
	void				*win;
	t_image_info		*img;
	pthread_t			*threads;
	t_thread_arg		*t_args;
	int					thread_count;
	_Bool				julia_toggle;
	t_colormode			colormode;
	int					color_bit_offset;
	float				color_add;
	int					(*fptr)(long double *);
	float				julia_pos[2];
	long double			pos[2];
	long double			zoom;
	float				zoom_acc;
	float				img_zoom;
	int					action;
}	t_mlx_info;

/* thread_helpers.c */
int				threads_done(t_mlx_info info);
double			time_elapsed(struct timeval t1);
void			cpy_thread_local_image(t_mlx_info info, int action);
void			set_fractal_pixel(long double *crd, t_thread_arg *arg);
void			update_t_args(t_mlx_info info);

/* threading.c */
void			update_t_args(t_mlx_info info);
void			mt_draw(t_mlx_info info, int zoom_in);
void			populate_threadinfo(t_mlx_info *info);
void			set_t_arg_finished(t_mlx_info info, _Bool b);

/* sample_image.c */
void			sample_image(t_mlx_info *info);
unsigned int	get_img_pixel(t_image_info img, int x, int y);
void			set_img_pixel(t_image_info img, int x, int y,
					unsigned int color);

/* loops.c */
int				key_loop(int keycode, void *p);
int				mouse_hook(int button, int x, int y, void *p);
int				loop(void *p);

/* fractals.c */
int				ship(long double *arg);
int				julia(long double *arg);
int				mandelbrot(long double *arg);

/* evaluate_arg.c */
void			error_exit(void);
int				evaluate_arg(char *arg, t_mlx_info *info);

#endif