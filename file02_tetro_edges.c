/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file02_tetro_edges.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorgan- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 16:19:56 by pmorgan-          #+#    #+#             */
/*   Updated: 2019/03/11 21:27:18 by pmorgan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

	
	

static int	flr_find_top_bottom(t_tetro piece, char xo, _Bool top)
{
	int	i;

	i = (top) ? 0 : piece.dim.y - 1;
	while (i >= 0 && i < piece.dim.y && !ft_strchr(piece.array[i], xo))
		i = (top) ? i + 1 : i - 1;
	return (i);
}

static int	flr_find_left_edge(t_tetro piece)
{
	int	i;
	int	j;
	int	left_edge;

	j = -1;
	left_edge = piece.dim.x - 1;
	while (++j < piece.dim.x)
	{
		i = -1;
		while (++i < piece.dim.y)
			if (piece.array[i][j] != '.')
				if (j < left_edge)
					left_edge = j;
	}
	return (left_edge);
}

static int	flr_find_right_edge(t_tetro piece)
{
	int	i;
	int	j;
	int	right_edge;

	j = piece.dim.x;
	right_edge = 0;
	while (--j >= 0)
	{
		i = piece.dim.y;
		while (--i >= 0)
			if (piece.array[i][j] != '.')
				if (j > right_edge)
					right_edge = j;
	}
	return (right_edge);
}

void		flr_find_piece_edges(t_tetro *piece, char xo)
{
	piece->tl.y = flr_find_top_bottom(*piece, xo, 1);
	piece->br.y = flr_find_top_bottom(*piece, xo, 0);
	piece->tl.x = flr_find_left_edge(*piece);
	piece->br.x = flr_find_right_edge(*piece);
}
