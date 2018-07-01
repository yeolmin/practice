#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct data
{
	char* str;
	int cnt;
	struct data *left;
	struct data *right;
}DATA;


static struct data *root;

void insert_data(char* str)
{

	struct data *temp;
	struct data **p = &root;

	temp = (struct data *)malloc(sizeof(struct data));
	temp->str = str;
	temp->cnt = 1;
	temp->left = NULL;
	temp->right = NULL;

	while ( *p )
	{

		
		//printf("compare %s, %s = %d \n" , (*p)->str, str, strcmp((*p)->str, str));
		if (strcmp((*p)->str, str)  > 0)
		{
			p = &(*p)->left;
			printf("원래가 더큼 \n");
		}

		else if (strcmp((*p)->str, str) <0) 
		{		p = &(*p)->right;
			printf("원래가 더작음\n");
		}
		else
		{
			printf("같음\n ");
			((*p)->cnt)++;
			return;
		}
	}

	*p = temp;

}

void inorder(struct data* temp)
{
	if(temp == NULL)
		return;
	inorder(temp->left);
	printf("%s  ", temp->str);
	inorder(temp->right);
}

int main()
{
	int n, total = 0; 
	char str[100];
	char temp[1000][4];
	scanf("%d\n", &n);



	while(n--)
	{


		fgets(str, 100, stdin);
		char* token = strtok(str, " \n");
		while(token)
		{
			//	strcpy(arr[total].str, token);
			printf("token : %s\n", token);
			//	(arr[total].cnt)++;
			strcpy(temp[total],token);
			//insert_data(token);	
			total++;

			token = strtok(NULL, " \n");
		}

	}
	int i;
	for(i =0;i<total;i++)
		insert_data(temp[i]);
inorder(root);

}

