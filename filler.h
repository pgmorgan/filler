/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filler.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorgan- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 18:53:48 by pmorgan-          #+#    #+#             */
/*   Updated: 2019/03/12 14:38:26 by pmorgan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FILLER_H__
# define __FILLER_H__

# include "./fillit/fillit.h"
# include "./fillit/libft/libft.h"
# include "./fillit/get_next_line.h"
# include <unistd.h>
# include <fcntl.h>
# include <math.h>		//REMEMBER TO DELETE FORBIDDEN SQRT() AND POW()
# include <stdio.h>

typedef struct	s_player
{
	char	move_id;
	char	id;
	char	opponent;
	char	opponent_move;
}				t_player;

//file02

void	flr_find_piece_edges(t_tetro *piece, char xo);

//file03
void		flr_print_map(t_map *a_map);
void		flr_print_piece(t_tetro *piece);
_Bool		flr_retrieve_piece(t_tetro *piece, t_player *player, int fd);
_Bool		flr_retrieve_player_number(t_player *player, int fd);
_Bool		flr_retrieve_map_dimensions(t_node *dim, int fd);
_Bool		flr_generate_empty_map(t_map *a_map);



#endif

