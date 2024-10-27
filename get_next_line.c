
# include <stdio.h>
# include <fcntl.h> // open
# include <unistd.h> // read y close
# include <stdlib.h> // calloc y free. Me dejo todas las defs por aquí temporalmente. Después arreglo el .h
#define BUFFER_SIZE 42

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		i++;
	}
	return (i);
}
void	*ft_calloc(size_t count, size_t size)
{
	unsigned char *temp;
	size_t i;
	i = 0;
	temp = malloc(count * size);

	if (!temp)
		return (NULL);
	while (i < count * size)
	{
		temp[i++] = 0;
	}
	return (temp);
}
char	*ft_strchr(const char *s, int c)
{
	size_t i;
	i = 0;

	while (s[i])
	{
		if (s[i] == (char)c)
		{
			return ((char *)&s[i]);
		}
		i++;
	}
	if ((char)c == '\0')
		return ((char *)&s[i]);
	else
		return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char *str;
	int i;
	int j;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	j = 0;
	str = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 1, sizeof(char));
	if (!str)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
		str[i++] = s2[j++];
	return (str);
}
void *ft_realloc(char *to_reall, int len)
{
	char *temp;
	unsigned int i;
	i = 0;
	
	if (!to_reall)
		return(to_reall = ft_calloc(len + 1, sizeof(char)));
	temp = ft_calloc(ft_strlen(to_reall) + len + 1, sizeof(char));
	if (!temp)
		return (NULL);
	while(to_reall[i])
	{
		temp[i] = to_reall[i];
		i++;
	}
	temp[i] = 0;
	free(to_reall);
	return(temp);
}

static char *get_next_line(int fd)
{
	ssize_t bytes_read;
	char *next_line;
	static char *part_line;
	char *temp_buffer;
	unsigned int i;
	unsigned int j;

	next_line = NULL;
	temp_buffer = NULL;
	i = 0;
	j = 0;
	if(part_line != NULL)
	{
		temp_buffer = ft_realloc(temp_buffer, ft_strlen(part_line));
		if(!temp_buffer)
			return (NULL);
		temp_buffer = ft_strjoin(temp_buffer, part_line);
		free(part_line);
	}
	part_line = NULL;
	temp_buffer = ft_realloc(temp_buffer, BUFFER_SIZE);
	if(!temp_buffer)
		return (NULL);

	bytes_read = read(fd, temp_buffer, BUFFER_SIZE);
	if(bytes_read <= 0)
	{
		free(temp_buffer);
		return(NULL);
	}
	while(!ft_strchr(temp_buffer, '\n') && bytes_read > 0)
	{
		temp_buffer = ft_realloc(temp_buffer, BUFFER_SIZE);
		if(!temp_buffer)
			return (NULL);
		bytes_read = read(fd, temp_buffer + ft_strlen(temp_buffer), BUFFER_SIZE);
	}

	while(temp_buffer[i] && temp_buffer[i] != '\n')
		i++;

	next_line = ft_calloc(i + 2, sizeof(char));//1 '\n' 1 '\0'
	if(!next_line)
	{
		free(temp_buffer);
		return (NULL);
	}
	i = 0;
	while(temp_buffer[i] && temp_buffer[i] != '\n')
	{
		next_line[i] = temp_buffer[i];
		i++;
	}
	if(temp_buffer[i] == '\n')
		next_line[i++] = '\n';
	next_line[i] = '\0';

	part_line = ft_calloc(ft_strlen(temp_buffer) - i + 1, sizeof(char));
	if(!part_line)
	{
		free(next_line);
		free(temp_buffer);
		return NULL;
	}
	while(temp_buffer[i])
		part_line[j++] = temp_buffer[i++];
	part_line[j] = 0;
	free(temp_buffer);
	return (next_line);	
}


int main ()
{
	int line_count;
	int fd;
	char *line;
	line_count = 0;
	fd = open("shrek.txt", O_RDONLY);
	if(fd == -1)
		return(1);
	while(fd != 0)
	{
		line = get_next_line(fd);
		if(line != NULL)
		{
			printf("[%d]:%s\n", line_count, line);
			free(line);
		}
		line_count++;
	}
	close((int)fd);
	line_count++;
	return (0);
}
