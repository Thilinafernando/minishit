#include "libft.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	printf("=====[SPLIT]=====\n");
	char	**split;
	int		i;

	split = ft_split(argv[1], *argv[2]);


	i = 0;
	if (split)
	{
		while (split[i])
		{
			printf("checking split[%i]\n", i);
			printf("%s\n", split[i]);
			free(split[i]);
			i++;
		}
		free(split);
	}
	else
		printf("(null)\n");

	memcpy(NULL, NULL, 10);
	ft_memcpy(NULL, NULL, 10);

	printf("=====[STRTRIM]=====\n");
	char	*trim;
	
	trim = ft_strtrim("   \t \v\t  ", " \v\n\t");
	printf("trim='%s'\n", trim);
	free(trim);

	printf("=====[ITOA]=====\n");
	char	*itoa;
	itoa = ft_itoa(atoi(argv[1]));
	printf("%s\n", itoa);
	free(itoa);

	return (0);
}