#include "get_next_line.h"

// Alternative strlen, which is not the same as the standard
size_t	strlen_alt(const char *str, const char alt)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] && str[i] != alt)
		++i;
	if (str[i] == alt && str[i] != '\0')
		++i;
	return (i);
}

// Returns true if string has given (c) character
int	str_has_character(const char *str, const char c)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == c)
			return (1);
		++str;
	}
	return (0);
}

// Alternative strcpy, which is not the same as the standard
void	strcpy_alt(char *dest, const char *src, size_t *i)
{
	if (!dest || !src)
		return ;
	while (*src)
	{
		dest[*i] = *src;
		++src;
		++(*i);
	}
}

// Alternative strjoin, which is not the same as the standard
char	*strjoin_to_left(char *s1, const char *s2)
{
	size_t	i;
	char	*str_new;

	if (!s2 && !s2)
		return (NULL);
	if (!s2)
		return (s1);
	str_new = (char *)malloc((sizeof(char)
				* (strlen_alt(s1, '\0') + strlen_alt(s2, '\0'))) + 1);
	if (!str_new)
		return (NULL);
	i = 0;
	strcpy_alt(str_new, s1, &i);
	strcpy_alt(str_new, s2, &i);
	str_new[i] = '\0';
	free(s1);
	return (str_new);
}
