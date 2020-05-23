#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define SIZE 4000
#define MAX 200
#define R 35
#define ARR 15

typedef struct node {
	int adr;
	struct node* next;
}node;

typedef struct {
	char *name;
	node* head;
	int value;
}ht;

typedef struct {
    int *queue;
	int *parent;
    int rear;
    int front;
}que;

typedef unsigned long long int lint;

void enque(que *q, int x) {
    q->queue[q->rear++] = x;
}

int deque(que *q) {
    return q->queue[q->front++];
}

node* createNode(int adr) {
	node* root = (node*) calloc(1, sizeof(node));
	if (root == NULL) {
		printf("Yer Acilamadi \n");
	}
	root->adr = adr;
	root->next = NULL;
	return root;
}

void createHash(ht* hashTable, lint M) {
	int i;	
	for (i = 0; i < M; i++) {
		hashTable[i].name = (char*) calloc(MAX, sizeof(char));
		hashTable[i].name[0] = '\0';
		hashTable[i].value = 0;
	}
}

void createQue(que *q, lint M) {
	q->queue = (int*) calloc(M, sizeof(int));
	q->parent = (int*) calloc(M, sizeof(int));
	q->rear = 0;
    q->front = 0;
}

int double_hashing(lint key, int i, lint M) {
	int h1 = key % M;
	int h2 = 1 + (key % (M - 1));
	return (h1 + i * h2) % M;
}

lint find_key(char *str) {
	int i = 0;
	lint key = 0;
	int N = strlen(str);
	
	while (i < N) {
		key += str[i] * pow(R, N - i - 1);
		i++;
	}
	return key;
}

int insert_to_table(lint key, char* cursor, ht* hashTable, lint M) {
	int adr, i = 0;
	int same = 0;
	adr = double_hashing(key, i, M);
	
	while( (!same) && (hashTable[adr].name[0] != '\0')  && (i < M) ) {
		if (strcmp(cursor, hashTable[adr].name) == 0)
			same = 1;
		else {
			i++;
			adr = double_hashing(key, i, M);
		}
	}
	if (same) {
		return adr;
	}
	else if (hashTable[adr].name[0] == '\0') {
		strcpy(hashTable[adr].name, cursor);
		hashTable[adr].head = NULL;
		return adr;
	} 
	else if (hashTable[adr].name[0] != '\0') {
		printf("Tabloda Bos Yer Bulunamadi \n");
		return -1;
	}
	return -1;
}

void insert_node(ht* hashTable, int adr1, int adr2) {
	node* temp_node = createNode(adr2);
	node* root;
	if (hashTable[adr1].head == NULL) {
		hashTable[adr1].head = temp_node;
	}
	else {
		root = hashTable[adr1].head;
		while (root->next) {
			root = root->next;
		}
		root->next = temp_node;
	}
}

void get_elements(char* line, ht* hashTable, lint M) {
	char *cursor, temp1[200], temp2[200];
	int i, j, aktor_adr = 0, movie_adr = 0;
	lint key = 0;
	
	for (cursor = strtok(line, "/"); cursor != NULL; cursor = strtok(NULL, "/")) {
		if (strcmp(cursor, "S\n") == 0) {
			continue;
		}
		i = 0;
		while((i < strlen(cursor) && ((cursor[i] < '0') || (cursor[i] > '9')))) {
			i++;
		} 
		if (i < strlen(cursor)) {
			key = find_key(cursor);
			movie_adr = insert_to_table(key, cursor, hashTable, M);
		}
		else {
			i = 0;
			while (cursor[i] != ',') {
				temp1[i] = cursor[i];
				i++;
			} temp1[i] = '\0';
			
			i+= 2;
			j = 0;
			while ( (i < strlen(cursor)) && (cursor[i] != '\n')) {
				temp2[j] = cursor[i];
				i++;
				j++;
			} temp2[j] = '\0';
			strcat(temp2, " ");
			strcat(temp2, temp1);
			key = find_key(temp2);
			aktor_adr = insert_to_table(key, temp2, hashTable, M);
			if (aktor_adr != -1) {
				insert_node(hashTable, movie_adr, aktor_adr);
				insert_node(hashTable, aktor_adr, movie_adr);
			}
		}
	}
}

int search(char* aktor, ht* hashTable, lint M) {
	int i = 0;
	
	while ((i < M) && (strcmp(hashTable[i].name, aktor) != 0)) {
		i++;
	}
	if (i < M)
		return i;
	else {
		printf("%s isimli aktor listede bulunamadi!\n", aktor);
		return -1;
	}
}

int bfs(que *q, char* aktor1, char* aktor2, ht* hashTable, lint M, int select) {
    int i = 0, flag = 0, j = 0, count = 0;
	
	int adr1 = search(aktor1, hashTable, M);
	int adr2 = search(aktor2, hashTable, M);
	if ((adr1 == -1) || (adr2 == -1))
		return;
	
	if(adr1==adr2){
		if(select==1)
			printf("Kevin Bacon's Kevin Bacon number is 0\n");
		return 0;
	}
	
	int tmp = adr1;
	node* root;
    int *path = (int*) calloc(M, sizeof(int));
	int* visited = (int*) calloc(M, sizeof(int));
	
	q->front = 0; 
	q->rear = 0;
	
	q->parent[tmp] = -1;
    visited[tmp] = 1;
	enque(q, tmp);
    while ((!flag) && (q->front < q->rear)) {
        tmp = deque(q);
        if (hashTable[tmp].head) {
			root = hashTable[tmp].head;
			while (root) {
				if (visited[root->adr] != 1) {
					q->parent[root->adr] = tmp;
					visited[root->adr] = 1;
					enque(q, root->adr);
					if (root->adr == adr2) {
						flag = 1;
					}
				}
				root = root->next;
			}
        }
    }
    
	if (flag == 1) {
		i = adr2;
		j = 0;
		while (i != adr1) {
			path[j++] = i;
			i = q->parent[i];
		}
		
		path[j] = adr1;
		
		while (j > 0) {
			if(select==1)
				printf("%s - ", hashTable[path[j]].name);
			j = j - 2;
			if(select==1){
				printf("%s '", hashTable[path[j++]].name);
				printf("%s'\n", hashTable[path[j--]].name);
			}
			count++;
		}
		if (select==1)
				printf("\n%s 's Kevin Bacon Number is %d\n",hashTable[adr2].name, count);
			return count;		
	}
	else{
		if(select==1)
			printf("%s ile Kevin Bacon arasinda Baglantisi Yoktur.\n",aktor2);
		return ARR;
	} 
}

void file_reader(ht* hashTable, lint M, int input) 
{
	FILE *fp;
	char line[SIZE];
	
	switch(input){
		case 1:
			fp = fopen("input-1.txt", "r");
				if (fp == NULL) {
					printf("File could not opened !!\n");
					return;
				}
			break;
		case 2:
			fp = fopen("input-2.txt", "r");
				if (fp == NULL) {
					printf("File could not opened !!\n");
					return;
				}
			break;
		case 3:
			fp = fopen("input-3.txt", "r");
				if (fp == NULL) {
					printf("File could not opened !!\n");
					return;
				}
			break;
	}
	
	while ( fgets(line, SIZE - 1, fp) ) {
		get_elements(line, hashTable, M);
	}
}

void Menu_Yazdir(int input){
	system("cls");
	printf("...input-%d.txt Dosyasindan Graf Basariyla Olusturuldu...\n",input);
	printf("!!!'Firstname Lastname' Seklinde Giriniz -> Ornek input: Kevin Bacon\n\n");
	printf("1 - HER KEVIN BACON SAYISINDAKI AKTOR SAYISI  \n");
	printf("2 - AKTORUN KEVIN BACON BAGLANTISI \n");
	printf("3 - EXIT");
}

int main()
{
	char aktor1[100], aktor2[100];
	lint M=3001;
	
	int bacon_num[ARR]={0};
	que q;
	int i=0, j=0, flag=1, input, mode;
	
	printf("1 - input-1.txt\n");
	printf("2 - input-2.txt\n");
	printf("3 - input-3.txt\n");
	printf("Acmak istediginiz input dosyasinin numarasini giriniz: ");
	scanf("%d",&input);
	
	if(input==2)
		M = 8001;
	else if(input==3)
		M = 323541;
		
	ht* hashTable = (ht*) calloc(M, sizeof(ht));
	
	createHash(hashTable, M);
	createQue(&q, M);
	
	file_reader(hashTable, M, input);
	 
	Menu_Yazdir(input);
	printf("\n***Islem Yapmak Istediginiz Fonksiyonu Seciniz : ");
	scanf("%d", &mode);
	
	do {
		switch(mode) {
			case 1: 
					Menu_Yazdir(input);
					printf("\n\n--- HER KEVIN BACON SAYISINDAKI AKTOR SAYISI ---\n");
					strcpy(aktor1, "Kevin Bacon");
					
					for(i = 0; i < M; i++) {
						if (hashTable[i].name[0] != '\0') {
							j=0;
							
							while(hashTable[i].name[j] != '\0'){
								if(hashTable[i].name[j] == '('){
									break;
								}
								j++;
							}
							
							if(hashTable[i].name[j] == '\0'){
								bacon_num[bfs(&q, aktor1, hashTable[i].name, hashTable, M, 0)]++;
									}
								}
							}
					printf("\n");
					for(i=0;i<ARR;i++){
						if(bacon_num[i]!=0)
							printf("Kevin Bacon Sayisi %d olan aktor sayisi: %d\n",i,bacon_num[i]);
					}
					printf("Kevin Bacon Sayisi Sonsuz olan aktor sayisi: %d\n",bacon_num[ARR]);
					
					for(i=0;i<=ARR;i++)
						bacon_num[i]=0;
						
						printf("\n***Islem Yapmak Istediginiz Fonksiyonu Seciniz : ");
						scanf("%d", &mode);
				break;
			case 2: 
					Menu_Yazdir(input);
					printf("\n\n--- AKTORUN KEVIN BACON BAGLANTISI ---\n");
					strcpy(aktor1, "Kevin Bacon");
					printf("Aktorun Adini ve Soyadini Giriniz: ");
					scanf(" %[^\n]s", aktor2);
					printf("\n");
					
					int adr = search(aktor2, hashTable, M);
					if (adr != -1){
					
					flag=1;
					if(hashTable[adr].value!=0){
						printf("--- Onceden Aranan Eleman Bulundu ---\n\n");
							printf("%s 's Kevin Bacon Number is: %d\n",hashTable[adr].name,hashTable[adr].value);
							flag=0;
					}
					if(flag==1){
							hashTable[adr].value = bfs(&q, aktor1, aktor2, hashTable, M, 1);
							if(hashTable[adr].value==ARR) hashTable[adr].value=0;
					}
				}
				
				printf("\n***Islem Yapmak Istediginiz Fonksiyonu Seciniz : ");
				scanf("%d", &mode);
				break;
		}
	} while(mode != 3);
	printf("Program Sonlandi...");
	return 0;
}
