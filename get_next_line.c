
#include "get_next_line.h"

static char	*read_into_stash(int fd, char *stash)
{
	char	*buf;
	ssize_t	bytes_read;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	bytes_read = 1;
	while (!gnl_strchr(stash, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buf);
			free(stash);
			return (NULL);
		}
		buf[bytes_read] = '\0';
		stash = gnl_strjoin(stash, buf);
		if (!stash)
		{
			free(buf);
			return (NULL);
		}
	}
	free(buf);
	return (stash);
}

static char	*extract_line(char *stash)
{
	char	*newline;
	size_t	len;

	if (!stash || !stash[0])
		return (NULL);
	newline = gnl_strchr(stash, '\n');
	if (newline)
		len = (size_t)(newline - stash) + 1;
	else
		len = gnl_strlen(stash);
	return (gnl_substr(stash, 0, len));
}

static char	*trim_stash(char *stash)
{
	char	*newline;
	char	*leftover;

	newline = gnl_strchr(stash, '\n');
	if (!newline)
	{
		free(stash);
		return (NULL);
	}
	leftover = gnl_substr(stash, (unsigned int)(newline - stash) + 1,
			gnl_strlen(newline + 1));
	free(stash);
	if (!leftover || !leftover[0])
	{
		free(leftover);
		return (NULL);
	}
	return (leftover);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_into_stash(fd, stash);
	if (!stash)
		return (NULL);
	line = extract_line(stash);
	stash = trim_stash(stash);
	return (line);
}
