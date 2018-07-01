#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct data
{
	char word[256];
	int cnt;
};


struct data DATA[1000];
int total;


void insert_data(char *str)
{
	int i;
	for(int i=0; i<total;i++)
	{
		if(strcmp(str, DATA[i].word) == 0)
		{
			DATA[i].cnt++;
			return;
		}
	}

	strcpy(DATA[total].word, str);
	DATA[total].cnt = 1;
	total++;
}


int compare_cnt( const void* a, const void* b)
{
	const struct data* d1 = (const struct data*)a;
	const struct data* d2 = (const struct data*)b;

	if(d1->cnt > d2->cnt)
		return -1;
	else if(d1->cnt < d2->cnt)
		return 1;
	else
		return strcmp(d1->word, d2->word);

}



int main()
{
	int n, i;
	char str[256];
	char* token;

	scanf("%d\n", &n);

	for(i=0;i<n;i++)
	{
		fgets(str, sizeof(str), stdin);
		token =strtok(str, " \n");

		while(token)
		{
			insert_data(token);
			token = strtok(NULL, " \n");
		}
		
	}

	qsort(DATA, total, sizeof(struct data), compare_cnt);

	for(i=0;i<total;i++)
		printf("%s %d\n", DATA[i].word, DATA[i].cnt);



}
