#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h> // for tests
#include <stdio.h> // for tests

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 420
# endif

char	*get_next_line(int fd);
char	*strjoin_to_left(char *s1, const char *s2);
size_t	strlen_alt(const char *str, const char alt);
int		str_has_character(const char *str, const char c);
void	strcpy_alt(char *dest, const char *src, size_t *i);

#endif