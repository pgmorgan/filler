/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorgan- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 15:56:51 by pmorgan-          #+#    #+#             */
/*   Updated: 2019/03/13 20:19:10 by pmorgan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"


_Bool		flr_populate_map(t_map *a_map, t_player *player, int fd)
{
	char	*str;
	int		i;
	int		j;
	int		gnl;

	while ((gnl = get_next_line(fd, &str)) == 1 && !ft_strstr(str, "    0"))
		ft_strdel(&str);
	ft_strdel(&str);
	if (gnl == -1 || gnl == 0)
		return (0);
	i = 0;
	while (i < a_map->dim.y)
	{
		gnl = get_next_line(fd, &str);
		if (gnl == -1)
			return (0);
		j = -1;
		while (++j < a_map->dim.x)
			a_map->map[i][j] = str[j + 4];
		ft_strdel(&str);
		i++;
	}
	return (1);
}

		



int			main(void)
{
	t_map		r_map;
	t_player	player;
	t_tetro		piece;
	int			fd;
	// STRUCT t_turn_params BEGIN
	int			fd_out;
	int			collisions;
	int			other_collisions;
	char		search_count;
	double		distance1;
	double		distance2;
	t_node		save_best_position;
	int			i;
	int			j;
	_Bool		success;
	// STRUCT t_turn_params END


	fd = 0;
	fd_out = open("output", O_WRONLY);
	if (!flr_retrieve_player_number(&player, fd))
		return (1);
    r_map.dim.x = 16;
    r_map.dim.y = 11;
	if (!flr_generate_empty_map(&r_map))
		return (1);
	search_count = 0;
	while (1)
	{
		//FUNCTION POPULATE MAP AND PIECE BEGIN	
		if (!flr_populate_map(&r_map, &player, fd))
			return (1);
		if (!flr_retrieve_piece(&piece, &player, fd))
			return (1);
		flr_find_piece_edges(&piece, player.move_id);
		//FUNCTION POPULATE MAP AND PIECE END
		distance1 = sqrt(pow((double)r_map.dim.y, 2) + pow((double)r_map.dim.x, 2));
		r_map.pn.y = 0;
		while (r_map.pn.y < r_map.dim.y)
		{
			r_map.pn.x = 0;
			while (r_map.pn.x < r_map.dim.x)
			{
				//FUNCTION COLLISION CHECK BEGIN 26 LINES
				piece.pn.y = piece.tl.y;
				collisions = 0;
				other_collisions = 0;
				while (piece.pn.y <= piece.br.y \
						&& r_map.pn.y + piece.br.y < r_map.dim.y)
				{
					piece.pn.x = piece.tl.x;
					while (piece.pn.x <= piece.br.x \
							&& r_map.pn.x + piece.br.x < r_map.dim.x)
					{
						if ((r_map.map[r_map.pn.y + piece.pn.y]\
								[r_map.pn.x + piece.pn.x] == player.opponent \
								|| r_map.map[r_map.pn.y + piece.pn.y]\
								[r_map.pn.x + piece.pn.x] == player.opponent_move) \
								&& piece.array[piece.pn.y][piece.pn.x] != '.')
							other_collisions++;
						else if ((r_map.map[r_map.pn.y + piece.pn.y]\
								[r_map.pn.x + piece.pn.x] == player.id\
								|| r_map.map[r_map.pn.y + piece.pn.y]\
								[r_map.pn.x + piece.pn.x] == player.move_id)\
								&& piece.array[piece.pn.y][piece.pn.x] != '.')
							collisions++;
						piece.pn.x++;
					}
					piece.pn.y++;
				}
				//FUNCTION COLLISION CHECK END 26 LINES
				//FUNCTION DISTANCE CHECK BEGIN 
				if (collisions == 1 && other_collisions == 0)
				{
					search_count++;
					for (i = 0; i < r_map.dim.y; i++)
					{
						for (j = 0; j < r_map.dim.x; j++)
						{
							if (r_map.map[i][j] == ((search_count <= 1) ? player.opponent : player.opponent))
							{
								distance2 = sqrt(pow((double)i - (double)r_map.pn.y, 2)\
										+ pow((double)j - (double)r_map.pn.x, 2));
								if (distance2 < distance1)
								{
									save_best_position = r_map.pn;
									distance1 = distance2;
                                    success = 1;
								}
							}
						}
					}
				}
				//FUNCTION DISTANCE CHECK END
				r_map.pn.x++;
			}
			r_map.pn.y++;
		}
        if (success)
		{
			success = 0;
			dprintf(fd_out, "OUTPUT WAS %d %d\n", save_best_position.y, save_best_position.x);
			ft_putnbr(save_best_position.y);
			ft_putchar(' ');
		    ft_putnbr(save_best_position.x);
			ft_putchar('\n');
		}
        else
            break;
	}
	return (0);
}
	

