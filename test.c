
// void prout()
// {
// 	char *caca;
// 	while(1)
// 		*caca = 0;
// }


// void main()
// {
// 	prout();
// }

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

void prout()
{

}

int main(int argc, char ** argv)
{
	int i = 5;
	while (*argv)
	{
		printf("%s\n", *argv);
		argv++;
	}
	printf("%d\n", sizeof(long long));
	while(1)
		prout();
}

// int main()
// {
// 	return 0;
// }