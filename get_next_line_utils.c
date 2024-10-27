#include "get_next_line.h"
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