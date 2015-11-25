#include <unistd.h>

int putonterm(int c)
{
	write(1, &c, 1);
	return (1);
}