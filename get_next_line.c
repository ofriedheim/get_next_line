/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oliver <oliver@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 22:59:48 by oliver            #+#    #+#             */
/*   Updated: 2021/04/03 23:11:01 by oliver           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#define TRUE 1
#define FALSE 0

int		check_for_line(char *buf)
{
	int		i;

	i = -1;
	while (buf[++i])
	{
		if (buf[i] == '\n')
			return (TRUE);
	}
	return (FALSE);
}

char	*gnl_cat(char *s1, char *s2)
{
	char	*cat;
	int		i;
	int		j;

	cat = (char*)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	i = 0;
	j = 0;
	while (s1[i])
	{
		cat[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		cat[i] = s2[j];
		i++;
		j++;
	}
	cat[i] = '\0';
	free(s1);
	return (cat);
}

void	split(t_line *lines)
{
	int		i;
	char	*ptr;

	i = -1;
	ptr = lines->temp;
	while (ptr[++i])
		if (ptr[i] == '\n')
			break ;
	lines->content = ft_substr(lines->temp, 0, i);
	lines->excess = ft_substr(lines->temp, i + 1, ft_strlen(lines->temp));
}

void	reading(int fd, char *buf, t_line *lines)
{
	int		read_ret;

	read_ret = 0;
	lines->has_been_read = TRUE;
	while ((read_ret = (read(fd, buf, BUFFER_SIZE))))
	{
		buf[read_ret] = '\0';
		if (lines->temp[0])
			lines->temp = gnl_cat(lines->temp, buf);
		else
			lines->temp = ft_strdup(buf);
		if (check_for_line(lines->temp) == TRUE)
			break ;
	}
	if (read_ret == 0)
		lines->content = ft_strdup(lines->excess);
}

int		get_next_line(int fd, char **line)
{
	static t_line	*lines[1000];
	char			buf[BUFFER_SIZE + 1];

	if (fd < 0 || fd > 4096 || BUFFER_SIZE <= 0)
		return (-1);
	if (!lines[fd])
	{
		lines[fd] = (t_line*)malloc(sizeof(t_line));
		ft_bzero((lines[fd]->content = (char*)malloc(sizeof(char*))), 1);
		ft_bzero((lines[fd]->excess = (char*)malloc(sizeof(char*))), 1);
		ft_bzero((lines[fd]->temp = (char*)malloc(sizeof(char*))), 1);
		lines[fd]->has_been_read = FALSE;
	}
	if (lines[fd]->has_been_read == TRUE)
	{
		ft_bzero(lines[fd]->temp, ft_strlen(lines[fd]->temp));
		lines[fd]->temp = ft_strdup(lines[fd]->excess);
		ft_bzero(lines[fd]->excess, ft_strlen(lines[fd]->excess));
	}
	reading(fd, buf, lines[fd]);
	split(lines[fd]);
	*line = lines[fd]->content;
	if (lines[fd]->excess[0] == '\0')
		return (0);
	return (1);
}