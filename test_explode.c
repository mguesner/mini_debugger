char	**explode(char *str);

int main()
{
	char **array = explode(" info   prout   ");
	int i = 0;
	while (array[i])
	{
		printf("%d -> %s\n", i, array[i]);
		i++;
	}
}