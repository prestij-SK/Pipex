#include "pipex.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	char	*str_new;

	if (!s1 || !s2)
		return (NULL);
	str_new = (char *)malloc((sizeof(char)
				* (ft_strlen(s1) + ft_strlen(s2))) + 1);
	if (!str_new)
		return (NULL);
	i = 0;
	while (*s1)
	{
		str_new[i] = *s1;
		++s1;
		++i;
	}
	while (*s2)
	{
		str_new[i] = *s2;
		++s2;
		++i;
	}
	str_new[i] = '\0';
	return (str_new);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while ((i < n - 1) && s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	needle_len;

	if (!(*needle))
		return ((char *)haystack);
	needle_len = ft_strlen(needle);
	while (len > 0 && *haystack)
	{
		if (len < needle_len)
			return (NULL);
		if (!ft_strncmp(haystack, needle, needle_len))
			return ((char *)haystack);
		++haystack;
		--len;
	}
	return (NULL);
}

static size_t	word_counter(const char *str, char del)
{
	size_t	count;
	int		in_word;

	count = 0;
	in_word = 0;
	while (*str != '\0')
	{
		if (*str == del)
		{
			in_word = 0;
		}
		else if (in_word == 0)
		{
			in_word = 1;
			count++;
		}
		str++;
	}
	return (count);
}

static char	*get_word(const char *str, char del, size_t *index)
{
	char	*word;
	size_t	i;

	while (str[*index] == del)
		++(*index);
	i = 0;
	while (str[i + *index] && str[i + *index] != del)
		++i;
	word = (char *)malloc(sizeof(char) * (i + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (str[i + *index] && str[i + *index] != del)
	{
		word[i] = str[i + *index];
		++i;
	}
	*index += i;
	word[i] = '\0';
	return (word);
}

void	ft_free_split(char **mat)
{
	int	i;

	if (!mat)
		return ;
	i = 0;
	while (mat[i])
	{
		free(mat[i]);
		mat[i] = NULL;
		++i;
	}
	free(mat[i]);
	mat[i] = NULL;
	free(mat);
	mat = NULL;
}

char	**ft_split(char const *s, char c)
{
	char	**mat;
	size_t	word_count;
	size_t	word_i;
	size_t	i;

	if (!s)
		return (NULL);
	word_count = word_counter(s, c);
	mat = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!mat)
		return (NULL);
	i = 0;
	word_i = 0;
	while (word_i < word_count)
	{
		mat[word_i] = get_word(s, c, &i);
		if (!mat[word_i])
		{
			ft_free_split(mat);
			return (NULL);
		}
		++word_i;
	}
	mat[word_i] = NULL;
	return (mat);
}