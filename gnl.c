#include <stdio.h>
#include <fcntl.h> // open
#include <unistd.h> // read y close
#include <stdlib.h> // calloc y free

// Asegúrate de definir BUFFER_SIZE aquí
#define BUFFER_SIZE 2  // Cambia esto a cualquier tamaño deseado

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
size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		i++;
	}
	return (i);
}
char	*ft_strchr(char *s, int c)
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

char	*ft_strdup(char *s1)
{
	char	*cpy;
	size_t	i;

	i = 0;
	cpy = (char *)malloc(ft_strlen(s1) + 1);
	if (!cpy)
		return (0);
	while (s1[i])
	{
		cpy[i] = s1[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}
char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char *str;
	size_t i;

	i = 0;
	if (!s)
		return (0);
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	str = ft_calloc((len + 1), sizeof(char));
	if (!str)
		return (0);
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	return (str);
}
char	*ft_strjoin(char *s1, char *s2)
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
char *next_line(char **memory)
{
	int i;
	char *line;
	char *dup_line;

	if(!*memory)
		return (NULL);
	if(ft_strchr(*memory, '\n') != NULL)
		i = ft_strchr(*memory, '\n') - *memory;
	else
		i = ft_strlen(*memory);
	line = ft_substr(*memory, 0, i);
	if(!line)
		return(NULL);
	dup_line = ft_substr(*memory, i + 1, ft_strlen(*memory) - i);
	free(*memory);
	*memory = dup_line;
	return (line);
	
}
static char *ft_get_next_line(int fd)
{
	static char *memory;
	ssize_t bytes_read;
	char *line;
	char *temp_buf;

	temp_buf = NULL;
	line = NULL;
	if(!memory)
		memory = ft_calloc(BUFFER_SIZE + 1, sizeof(char));

	if(fd < 0 || BUFFER_SIZE <= 0 || !memory)
		return (NULL);
	while(!ft_strchr(memory, '\n'))
	{
		if(ft_strlen(memory) > 0)
		{
			line = ft_strdup(memory);
			free(memory);
			if(!line)
		 		return(NULL);
		}
		temp_buf = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
		if(!temp_buf)
			return(NULL);
		bytes_read = read(fd, temp_buf, BUFFER_SIZE);
		if(bytes_read <= 0)
		{
			free(temp_buf);
			if(line)
				return(line);
			return (NULL);
		}
		memory = ft_strdup(temp_buf);
		free(temp_buf);
	}
	if(!line)
		return(next_line(&memory));
	line = ft_strjoin(line, next_line(&memory));
	if(!line)
		return(NULL);
	return(line);
}

int main ()
{
	int line_count;
	int fd;
	char *line;
	line_count = 0;
	fd = open("tinytest.txt", O_RDONLY);
	if(fd == -1)
		return(1);
	while(fd != 0)
	{
		line = ft_get_next_line(fd);
		if(line != NULL)
		{
			printf("[%d]:%s\n", line_count, line);
			free(line);
		}else{
			close((int)fd);
			return (0);	
		}
		line_count++;
	}
	close((int)fd);
	return (0);
}
