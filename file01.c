/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorgan- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 15:56:51 by pmorgan-          #+#    #+#             */
/*   Updated: 2019/03/11 21:13:32 by pmorgan-         ###   ########.fr       */
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

_Bool		flr_retrieve_player_number(t_player *player, int fd)
{
	char	*str;
	int		gnl;

	while ((gnl = get_next_line(fd, &str)) == 1 && *str != '$')
	{
		if (ft_strstr(str, "launched"))
		{
			if (ft_strstr(str, "pmorgan-"))
			{
				player->move_id = 'o';
				player->id = 'O';
				player->opponent_move = 'X';
				player->opponent = 'X';
			}
			else
			{
				player->move_id = 'x';
				player->id = 'X';
				player->opponent_move = 'o';
				player->opponent = 'O';
			}
		}
		ft_strdel(&str);
	}
	if (gnl == -1)
		return (0);
	return (1);
}

_Bool		flr_retrieve_map_dimensions(t_node *dim, int fd)
{
	char	*str;
	char	*str_copy;
	int		gnl;

	while ((gnl = get_next_line(fd, &str)) == 1 && !ft_strstr(str, "    "))
		ft_strdel(&str);
	if (gnl == -1)
		return (0);
	str_copy = str;
	while (!ft_isdigit(*str))
		str++;
	dim->x = 0;
	while (ft_isdigit(*str) && ++dim->x)
		str++;
	ft_strdel(&str_copy);
	dim->y = 0;
	while ((gnl = get_next_line(fd, &str)) == 1 && ft_isdigit(*str) && ++dim->y)
		ft_strdel(&str);
	if (gnl == -1)
		return (0);
	return (1);
}

_Bool	flr_generate_empty_map(t_map *a_map)
{
	int	i;
	int j;

	a_map->map = (char**)malloc(a_map->dim.y * sizeof(char*));
	if (!a_map->map)
		return (0);
	i = 0;
	while (i < a_map->dim.y)
	{
		a_map->map[i] = (char*)malloc(a_map->dim.x * sizeof(char));
		if (!a_map->map[i])
			return (0);
		//j = 0;
		//while (j < a_map->dim.x)
		//	a_map->map[i][j] = 0;
		i++;
	}
	return (1);
}

_Bool		flr_populate_map(t_map *a_map, t_player *player, int fd)
{
	char	*str;
	int		i;
	int		j;
	int		gnl;

	while ((gnl = get_next_line(fd, &str)) == 1 && !ft_strstr(str, "    0"))
		ft_strdel(&str);
	ft_strdel(&str);
	if (gnl == -1)
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

void		flr_print_map(t_map *a_map)
{
	int	i, j;
	for (i = 0; i < a_map->dim.y; i++)
	{
		for (j = 0; j < a_map->dim.x; j++)
			ft_putchar(a_map->map[i][j]);
		ft_putchar('\n');
	}
}

void		flr_print_piece(t_tetro *piece)
{
	int	i, j;
	for (i = 0; i < piece->dim.y; i++)
	{
		for (j = 0; j < piece->dim.x; j++)
			ft_putchar(piece->array[i][j]);
		ft_putchar('\n');
	}
}


_Bool		flr_retrieve_piece(t_tetro *piece, t_player *player, int fd)
{
	char	*str;
	int		i;
	int		j;
	int		gnl;

	gnl = get_next_line(fd, &str);
	if (gnl == -1)
		return (0);
	i = 0;
	while (!ft_isdigit(str[i]))
		i++;
	piece->dim.y = ft_atoi(&str[i]);
	piece->dim.x = ft_atoi(&str[i + 2]);
	ft_strdel(&str);
	piece->array = (char**)malloc(piece->dim.y * sizeof(char*));
	if (!piece->array)
		return (0);
	i = -1;
	while (++i < piece->dim.y)
		if (!(piece->array[i] = ft_strnew((size_t)piece->dim.x)))
			return (0);
	i = -1;
	while (++i < piece->dim.y)
	{
		if (get_next_line(fd, &str) == -1)
			return (0);
		j = -1;
		while (++j < piece->dim.x)
			piece->array[i][j] = (str[j] == '*') ? player->move_id : '.';
		ft_strdel(&str);
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

	fd = open(argv[1], O_RDONLY);
//	fd = 0;

	if (!flr_retrieve_player_number(&player, fd))
		return (1);
	if (!flr_retrieve_map_dimensions(&(r_map.dim), fd))
		return (1);
	ft_putnbr(r_map.dim.y);
	ft_putchar(' ');
	ft_putnbr(r_map.dim.x);
	ft_putchar('\n');
	if (!flr_generate_empty_map(&r_map))
		return (1);
	close(fd);
	fd = open(argv[1], O_RDONLY);
	if (!flr_populate_map(&r_map, &player, fd))
		return (1);
	flr_print_map(&r_map);
	if (!flr_retrieve_piece(&piece, &player, fd))
		return (1);
	flr_print_piece(&piece);
	flr_find_piece_edges(&piece, player.move_id);

	distance1 = sqrt(pow((double)r_map.dim.x, 2) + pow((double)r_map.dim.x, 2));
	search_count = 0;
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
					if ((r_map.map[r_map.pn.y + piece.pn.y - piece.tl.y]\
							[r_map.pn.x + piece.pn.x - piece.tl.x] == player.opponent\
							|| r_map.map[r_map.pn.y + piece.pn.y - piece.tl.y]\
							[r_map.pn.x + piece.pn.x - piece.tl.x] == player.opponent_move)\
							&& piece.array[piece.pn.y][piece.pn.x] != '.')
						other_collisions++;
					else if (r_map.map[r_map.pn.y + piece.pn.y - piece.tl.y]\
							[r_map.pn.x + piece.pn.x - piece.tl.x] == player.id\
							&& piece.array[piece.pn.y][piece.pn.x] != '.')
						collisions++;
					piece.pn.x++;
				}
				piece.pn.y++;
			}
			if (collisions == 1 && other_collisions == 0)
			{
				ft_putnbr(r_map.pn.y);
				ft_putchar('\t');
				ft_putnbr(r_map.pn.x);
				ft_putchar('\n');
				//CALC distance1
				search_count++;
				for (i = 0; i < r_map.dim.y; i++)
				{
					for (j = 0; j < r_map.dim.x; j++)
					{
						if (r_map.map[i][j] == ((search_count < 2) ? player.opponent : player.opponent_move))
						{
							distance2 = sqrt(pow((double)i - (double)r_map.pn.y, 2)\
									+ pow((double)j - (double)r_map.pn.x, 2));
							if (distance2 < distance1)
							{
								save_best_position = r_map.pn;
								printf("d1\t%f\td2\t%f\n", distance1, distance2);
								distance1 = distance2;
							}
						}
					}
				}
			}
			r_map.pn.x++;
		}
		r_map.pn.y++;
	}
	printf("*** %d %d ***\n", save_best_position.y, save_best_position.x);
			
	return (0);
}


