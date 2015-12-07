#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
// void prout()
// {
// 	char *caca;
// 	while(1)
// 		*caca = 0;
// }
void caca()
{
	while(1);
}

void prout()
{
		// void *array[10];
  // size_t size;
  // char **strings;
  // size_t i;

  // size = backtrace (array, 10);
  // strings = backtrace_symbols (array, size);

  // printf ("Obtained %zd stack frames.\n", size);

  // for (i = 0; i < size; i++)
  //    printf ("%s\n", strings[i]);

  // free (strings);
  // while (1)
	caca();
}


void main(int ac, char **av, char **env)
{
  int i = 0;
  while (env[i])
  {
    printf("%s\n", env[i]);
    i++;
  }
  printf("%d env var\n", i);
	prout();
}

// #include <stdio.h>

// void main()
// {
// 	char *temp = "Paras";

// 	int i;
// 	i=0;

// 	temp[3]='F';

// 	for (i =0 ; i < 5 ; i++ )
// 		printf("%c\n", temp[i]);

	
// }

// int main(int argc, char ** argv)
// {
// 	int i = 5;
// 	while (*argv)
// 	{
// 		printf("%s\n", *argv);
// 		argv++;
// 	}
// 	printf("%d\n", sizeof(long long));
// 	while(1)
// 		prout();
// }

// int main()
// {
// 	return 0;
// }