
# include <stdio.h>
# include <fcntl.h> // open
# include <unistd.h> // read y close
# include <stdlib.h> // calloc y free. Me dejo todas las defs por aquí temporalmente. Después arreglo el .h
#define BUFFER_SIZE 42

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	if (str == NULL)
		return (0);
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		if ((char)c == s[i])
			return ((char *)&s[i]);
		i++;
	}
	if ((char)c == '\0')
		return ((char *)&s[i]);
	return (NULL);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	s_len;
	char	*substr;
	size_t	i;

	s_len = ft_strlen(s);
	if (s == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
		i++;
	if (start >= s_len)
		len = 0;
	else if (start + len > s_len)
		len = s_len - start;
	substr = (char *)malloc((len + 1) * sizeof(char));
	if (substr == NULL)
		return (NULL);
	i = 0;
	while (i < len && s[start + i] != '\0')
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*p;
	size_t			i;

	p = malloc(size * nmemb);
	if (p == NULL)
		return (NULL);
	i = 0;
	while (i < (size * nmemb))
	{
		p[i] = 0;
		i++;
	}
	return ((void *)p);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int		size_s1;
	int		size_s2;
	char	*str;
	int		i;
	int		j;

	size_s1 = ft_strlen(s1);
	size_s2 = ft_strlen(s2);
	str = malloc(sizeof(char) * (size_s1 + size_s2 + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (i < size_s1)
		str[i++] = s1[j++];
	j = 0;
	while (j < size_s2)
		str[i++] = s2[j++];
	str[i] = '\0';
	free(s1);
	return (str);
}
static void *final_line(char **line, char **previous_line)
{
	int i;
	char * temp;
	i = 0;
	while((*previous_line)[i] != '\n' && (*previous_line)[i] != '\0')
		i++;
	if((*previous_line)[i])
		i++;
	*line = ft_substr(*previous_line, 0, i);
	if((*previous_line)[i])
	{
		temp = ft_substr(*previous_line, i, (ft_strlen(*previous_line) - i));
		free(*previous_line);
		*previous_line = temp;
	}
	else
	{
		free(*previous_line);
		*previous_line = NULL;
	}
	
	
		
}
char *create_line(char* previous_line, int fd)
{
	char *buffer;
	int bytes_read;
	bytes_read = 1;
	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	
	if(!buffer)
		return NULL;
	while(!ft_strchr(previous_line, '\n') && bytes_read != 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if(bytes_read == 0)
			break;
		if(bytes_read == -1)
		{
			free(buffer);
			free(previous_line);
			return NULL;
		}
		buffer[bytes_read] = '\0';
		previous_line=ft_strjoin(previous_line, buffer);
		if(!previous_line)
			return(free(buffer), free(previous_line), NULL);
	}
		free(buffer);
		return (previous_line);
}

static char *get_next_line(int fd)
{
	char *line;
	static char *previous_line;

	line = NULL;
	if(fd == -1)
		return(NULL);
	previous_line = create_line(previous_line, fd);
	if(!previous_line)
		return (NULL);
	final_line(&line, &previous_line);
	return(line);
}

int main()
{
	int fd;
	char *line;
	int i;
	i = 0;

	fd = open("shrek.txt", O_RDONLY);
	if (fd == -1)
	{
		printf("Error abriendo el archivo\n");
		return (1);
	}
	line = get_next_line(fd);
	while (line)
	{
		printf("[%d]%s", i, line);
		free(line);
		line = get_next_line(fd);
		i++;
	}
	close(fd);
	return (0);
}
