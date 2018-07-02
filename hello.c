#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct info
{
	char str[100];
	int count;
	int length;
}data[100000];

int mycmp(struct info *a, struct info *b)
{
	if (strcmp(a->str, b->str) < 0)
		return -1;
	else
		return 1;
}

int compare(const void *a, const void *b)
{
	struct info* q = (struct info*)a;
	struct info* p = (struct info*)b;

	if (q->count == p->count)
		return mycmp(q, p);
	else if (q->count > p->count)
		return -1;
	else
		return 1;
}

char str[100] = { 0, };

int main()
{
	int N = 0;
	int n = 0, i = 0, j = 0, index = 0;
	int chk = 0;
	char* pch;
	char temp1[1], temp2[1], check[1024];

	scanf("%d", &N);
	getchar();

	for (n = 0; n < N; n++)
	{
		fgets(str, 100, stdin);
		str[strlen(str) - 1] = '\0';

		pch = strtok(str, " ");

		while (pch != NULL)
		{
			int used = 0;

			for (i = 0; i < index; i++)
			{
				if (data[i].length == strlen(pch) && !strcmp(data[i].str, pch))
				{
					used = 1;
					data[i].count++;
					break;
				}
			}

			if (!used)
			{
				strcpy(data[index].str, pch);
				data[index].count++;
				data[index].length = strlen(pch);
				index++;
			}

			pch = strtok(NULL, " ");
		}
	}

	qsort(data, index, sizeof(struct info), compare);


	for (i = 0; i < index; i++)
	{
		int used = 0;

		for (j = 0; j < chk; j++)
		{
			if (check[j] == data[i].str[0])
			{
				used++;
			}
		}
		
		if (!used)
		{
			printf("%s: %d\n", data[i].str, data[i].count);
			check[chk] = data[i].str[0];
			chk++;
		}
	}


	/*
	for (i = 0; i < index; i++)
	{
		printf("%s: %d\n", data[i].str, data[i].count);
	}
	*/

	return 0;
}
