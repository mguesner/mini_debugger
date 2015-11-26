
int	search_tab(int *tab, int size, int elem)
{
	int max_pos = size;
	int pos = (pos % 2) ? (pos + 1) / 2 : (size / 2);

	while(size != 0)
	{
		printf("tab[%d] = %d -> %d\n", pos, *(tab + pos), elem);
		if (*(tab + pos) == elem)
			return pos;
		else if (*(tab + pos) > elem)
		{
			size = !(pos % 2) ? (pos / 2) : (pos + 1) / 2;
			max_pos = pos;
			pos = pos - size;
		}
		else
		{
			size = (max_pos - pos) / 2;
			pos = pos + size;
		}
	}
	return -1;
}

void main()
{
	int tab[10] = {10, 25, 28, 39, 45, 54, 61,70 , 83, 95};
	printf("found at %d\n", search_tab(tab, 10, 54));
}
