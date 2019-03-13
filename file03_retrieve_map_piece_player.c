/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file03_retrieve_map_piece_player.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorgan- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 12:21:12 by pmorgan-          #+#    #+#             */
/*   Updated: 2019/03/13 19:46:35 by pmorgan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

void		flr_print_map(t_map *a_map, int fd_out)
{
	int	i, j;
	for (i = 0; i < a_map->dim.y; i++)
	{
		for (j = 0; j < a_map->dim.x; j++)
			ft_putchar_fd(a_map->map[i][j], fd_out);
		ft_putchar_fd('\n', fd_out);
	}
}

void		flr_print_piece(t_tetro *piece, int fd_out)
{
	int	i, j;
	for (i = 0; i < piece->dim.y; i++)
	{
		for (j = 0; j < piece->dim.x; j++)
			ft_putchar_fd(piece->array[i][j], fd_out);
		ft_putchar_fd('\n', fd_out);
	}
}


_Bool		flr_retrieve_piece(t_tetro *piece, t_player *player, int fd)
{
	char	*str;
	int		i;
	int		j;
	int		gnl;

	gnl = get_next_line(fd, &str);
	if (gnl == -1 || gnl == 0)
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

_Bool		flr_retrieve_player_number(t_player *player, int fd)
{
	char	*str;
	int		gnl;

	while ((gnl = get_next_line(fd, &str)) == 1 && *str != '$')
		ft_strdel(&str);
	if (gnl == -1)
		return (0);
	if (ft_strstr(str, "$$$ exec p1") && ft_strstr(str, "pmorgan-"))
	{
		player->move_id = 'o';
		player->id = 'O';
		player->opponent_move = 'x';
		player->opponent = 'X';
	}
	else if (ft_strstr(str, "$$$ exec p2") && ft_strstr(str, "pmorgan-"))
	{
		player->move_id = 'x';
		player->id = 'X';
		player->opponent_move = 'o';
		player->opponent = 'O';
	}
	else
		return (0);
	ft_strdel(&str);
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
