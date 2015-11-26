#include <string.h>
#include <ctype.h>

char	**explode(char *str)
{
	int i = 0;
	int num_member = 0;
	int is_member = 0;
	while (str[i])
	{
		if (isblank(str[i]))
		{
			if (is_member)
				is_member = 0;
		}
		else if (!is_member)
		{
			is_member = 1;
			num_member++;
		}
		i++;
	}
	// printf("num_member : %d\n", num_member);
	char **ret = malloc((num_member + 1) * sizeof(char *));
	i = 0;
	int j = 0;
	int begin = 0;
	int n = 0;
	is_member = 0;
	while (str[i])
	{
		if (isblank(str[i]))
		{
			if (is_member)
			{
				is_member = 0;
				ret[j] = strndup(str + begin, n);
				n = 0;
				j++;
			}
		}
		else if (!is_member)
		{
			is_member = 1;
			begin = i;
			n++;
		}
		else
			n++;
		i++;
	}
	if (n)
	{
		ret[j] = strndup(str + begin, n);
		j++;
	}
	ret[j] = NULL;
	return ret;
}