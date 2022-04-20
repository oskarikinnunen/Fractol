/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 12:08:57 by okinnune          #+#    #+#             */
/*   Updated: 2022/04/20 15:31:42 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "mlx.h"
# include "libft.h"

//REMOVE
# include <stdio.h>

# define WSZ 1024
# define X 0
# define Y 1

typedef struct s_complex
{
	double	real;
	double	imaginary;
}	t_complex;

typedef struct s_image_info
{
	void	*ptr;
	char	*addr;
	int		bpp;
	int		size_line;
	int		endian;
}	t_image_info;

typedef struct s_mlx_info
{
	void				*mlx;
	void				*win;
	t_image_info		*img;
}	t_mlx_info;

# endif