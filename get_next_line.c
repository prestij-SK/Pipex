#include "get_next_line.h"

// Reads from file and returns dynamically allocated string which contains/doesn't contain '\n'
static char	*read_from_file(const int fd)
{
	char	*buffer;
	char	*final_text;
	ssize_t	read_bytes;

	final_text = NULL;
	read_bytes = 1;
	while (read_bytes > 0)
	{
		buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			free(final_text);
			return (NULL);
		}
		buffer[read_bytes] = '\0';
		if (read_bytes == 0 && !buffer[0])
		{
			free(buffer);
			break ;
		}
		final_text = strjoin_to_left(final_text, buffer);
		if (str_has_character(buffer, '\n'))
			read_bytes = 0;
		free(buffer);
	}
	return (final_text);
}

// Reads from string and returns dynamically allocated string which contains/doesn't contain '\n'
static char	*take_next_line(const char *scanned_text)
{
	char	*next_line;
	size_t	len;
	size_t	i;

	if (!scanned_text)
		return (NULL);
	len = strlen_alt(scanned_text, '\n');
	next_line = (char *)malloc(sizeof(char) * (len + 1));
	if (!next_line)
		return (NULL);
	i = 0;
	while (i < len)
	{
		next_line[i] = scanned_text[i];
		++i;
	}
	next_line[i] = '\0';
	return (next_line);
}

// Remove from string first line that contains/doesn't contain '\n'.
// Returns dynamically allocated string after removing.
static char *cut_next_line(char *scanned_text)
{
	char	*new_text;
	char	*temp;
	size_t	i;

	if (!scanned_text)
		return (NULL);
	i = strlen_alt(scanned_text, '\n');
	temp = scanned_text;
	new_text = (char *)malloc(sizeof(char) * (strlen_alt(scanned_text, '\0') - i + 1));
	if (!new_text)
		return (NULL);
	scanned_text += i;
	i = 0;
	strcpy_alt(new_text, scanned_text, &i);
	new_text[i] = '\0';
	if (!new_text[0])
	{
		free(new_text);
		new_text = NULL;
	}
	free(temp);
	return (new_text);
}

// Returns first line/string from file, which ends with '\n'.
// Returns string without '\n' if it is the end of the file.
char	*get_next_line(int fd)
{
	static char	*scanned_text;
	char		*next_line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) == -1)
	{
		free(scanned_text);
		scanned_text = NULL;
		return (NULL);
	}
	if (!(str_has_character(scanned_text, '\n')))
	{
		next_line = read_from_file(fd);
		if (next_line)
		{
			scanned_text = strjoin_to_left(scanned_text, next_line);
			free(next_line);
		}
	}
	next_line = take_next_line(scanned_text);
	scanned_text = cut_next_line(scanned_text);
	return (next_line);
}