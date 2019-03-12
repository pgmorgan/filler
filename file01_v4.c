/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorgan- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 15:56:51 by pmorgan-          #+#    #+#             */
/*   Updated: 2019/03/12 15:25:44 by pmorgan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

/*
int	main(int argc, char **argv)
{
	int		gnl;
	char	*str;
	int		fd;

	fd = open(argv[1], O_RDONLY);

	while ((gnl = get_next_line(fd, &str)) == 1)
	{
		ft_putchar('P');
		ft_putstr(str);
		ft_putchar('\n');
		ft_strdel(&str);
	}
	return (0);
}
*/

/*
int main()
{
	int y, x;
	scanf("%d", &y);
	scanf("%d", &x);
	printf("%d %d\n", y, x);
	return 0;
}
*/

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
		j = 0;
		while (j < a_map->dim.x)
		{
			a_map->map[i][j] = str[j + 4];
			j++;
		}
		ft_strdel(&str);
		i++;
	}
	return (1);
}

		



int			main(int argc, char **argv)
{
	t_map	r_map;
	t_player	player;
	t_tetro		piece;
	int		fd;
	int		collisions;
	int		other_collisions;
	char	search_count;
	double	distance1;
	double	distance2;
	t_node	save_best_position;
	int		i;
	int		j;
	_Bool	success;

	fd = 0;
	//fd = open(argv[1], O_RDONLY);

	if (!flr_retrieve_player_number(&player, fd))
		return (1);
	//if (!flr_retrieve_map_dimensions(&(r_map.dim), fd))
	//	return (1);
    r_map.dim.x = 9;
    r_map.dim.y = 5;
	//ft_putnbr(r_map.dim.y);
	//ft_putchar(' ');
	//ft_putnbr(r_map.dim.x);
	//ft_putchar('\n');
	if (!flr_generate_empty_map(&r_map))
		return (1);
	//close(fd);
	//fd = open(argv[1], O_RDONLY);
	search_count = 0;
	while (1)
	{
		if (!flr_populate_map(&r_map, &player, fd))
			return (1);
		//flr_print_map(&r_map);
		if (!flr_retrieve_piece(&piece, &player, fd))
			return (1);
		//flr_print_piece(&piece);
		flr_find_piece_edges(&piece, player.move_id);
	
		distance1 = sqrt(pow((double)r_map.dim.y, 2) + pow((double)r_map.dim.x, 2));
        success = 0;
		r_map.pn.y = 0;
		while (r_map.pn.y < r_map.dim.y)
		{
			r_map.pn.x = 0;
			while (r_map.pn.x < r_map.dim.x)
			{
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
								[r_map.pn.x + piece.pn.x] == player.opponent\
								|| r_map.map[r_map.pn.y + piece.pn.y]\
								[r_map.pn.x + piece.pn.x] == player.opponent_move)\
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
				if (collisions == 1 && other_collisions == 0)
				{
		//			ft_putnbr(r_map.pn.y);
		//			ft_putchar('\t');
		//			ft_putnbr(r_map.pn.x);
		//			ft_putchar('\n');
					//CALC distance1
					search_count++;
					for (i = 0; i < r_map.dim.y; i++)
					{
						for (j = 0; j < r_map.dim.x; j++)
						{
							if (r_map.map[i][j] == ((search_count <= 1) ? player.opponent : player.opponent_move))
							{
								distance2 = sqrt(pow((double)i - (double)r_map.pn.y, 2)\
										+ pow((double)j - (double)r_map.pn.x, 2));
								if (distance2 < distance1)
								{
									save_best_position = r_map.pn;
		//							printf("d1\t%f\td2\t%f\n", distance1, distance2);
									distance1 = distance2;
                                    success = 1;
								}
							}
						}
					}
				}
				r_map.pn.x++;
			}
			r_map.pn.y++;
		}
        if (success)
		    printf("%d %d\n", save_best_position.x, save_best_position.y);
        else
            break;
	}
	return (0);
}
	

