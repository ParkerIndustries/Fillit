/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xxxxxxxx <marvin@student.42.fr>.           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 11:01:48 by cdittric          #+#    #+#             */
/*   Updated: 2017/11/23 15:16:48 by cdittric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>

#define MAXS 24

int		shift(char *t, int x, int y)
{
	int		i;
	int		isdouble;
	int		connecteds;
	int		connections;
	int		count;

	count = 0;
	isdouble = 0;
	connecteds = 0;
	i = 16;
	while (--i >= 0)
	{
		count = (t[i] == '#' ? count + 1 : count);
		x = (t[i] == '#' && x > i % 4 ? i % 4 : x);
		y = (t[i] == '#' && y > i / 4 ? i / 4 : y);
		connections = (t[i] == '#' && i % 4 < 3 && t[i + 1] == '#' ? 1 : 0);
		connections += (t[i] == '#' && i / 4 > 0 && t[i - 4] == '#' ? 1 : 0);
		connections += (t[i] == '#' && i % 4 > 0 && t[i - 1] == '#' ? 1 : 0);
		connections += (t[i] == '#' && i / 4 < 3 && t[i + 4] == '#' ? 1 : 0);
		isdouble = (connections > 1 ? 1 : isdouble);
		connecteds += (connections > 0 ? 1 : 0);
	}
	while (++i < 16)
		t[i] = (i < 16 - (4 * y + x) ? t[i + 4 * y + x] : '.');
	return (isdouble && connecteds == 4 && count == 4 ? 1 : 0);
}

int		read_tetriminos(int fd, char tab[])
{
	char	c;
	int		count;
	int		i;
	int		j;

	count = 0;
	while (count < 26)
	{
		i = -1;
		while (++i < 4)
		{
			j = 0;
			while (j < 4 && read(fd, &c, 1) > 0 && (c == '.' || c == '#'))
				tab[16 * count + 4 * i + j++] = c;
			if (j < 4 || read(fd, &c, 1) <= 0 || c != '\n')
				return (-1);
		}
		i = read(fd, &c, 1);
		if (i < 0 || (i > 0 && c != '\n') || !shift(tab + count++ * 16, 3, 3))
			return (-1);
		if (i == 0)
			break ;
	}
	tab[16 * count] = '\0';
	return (count);
}

int		display_solution(char board[], int size, int n)
{
	int		i;

	if (n == 0)
	{
		i = 0;
		while (i < size * size)
		{
			write(1, board + MAXS * (i / size) + i % size, 1);
			if (i % size == size - 1)
				write(1, "\n", 1);
			i++;
		}
	}
	return (1);
}

int		solve(char board[], char t[], int s, int n)
{
	int		i;
	int		j;
	int		p;

	i = -1;
	while (++i < s * s)
	{
		p = MAXS * (i / s) + i % s;
		j = -1;
		while (++j < 16)
			if (t[j] == '#' && (p % MAXS >= s - j % 4 || p / MAXS >= s - j / 4
						|| board[p + j / 4 * MAXS + j % 4] == '#'))
				break ;
			else if (t[j] == '#')
				board[p + j / 4 * MAXS + j % 4] = '#';
		if (j == 16 && (t[16] == '\0' || solve(board, t + 16, s, n + 1)))
			break ;
		while (j > 0)
			if (t[--j] == '#')
				board[p + j / 4 * MAXS + j % 4] = '.';
	}
	while (i < s * s && --j >= 0)
		if (t[j] == '#')
			board[p + j / 4 * MAXS + j % 4] = 'A' + n;
	return (i == s * s ? 0 : display_solution(board, s, n));
}

int		main(int argc, char *argv[])
{
	int			fd;
	char		tetriminos[26 * 16 + 1];
	int			size;
	char		board[MAXS * MAXS];

	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd < 0 || read_tetriminos(fd, tetriminos) == -1)
		{
			if (fd >= 0)
				close(fd);
			write(1, "error\n", 6);
			return (0);
		}
		close(fd);
		size = MAXS * MAXS;
		while (size > 0)
			board[--size] = '.';
		while (!solve(board, tetriminos, ++size, 0))
			;
	}
	else
		write(1, "usage: fillit input_file\n", 25);
	return (0);
}
