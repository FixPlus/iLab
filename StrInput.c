#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* profanity[] ={"MEOW","Pascal","Kek"};
const int prof_size = 3;
void getStrPointer(const char* prompt, char* str, int length);
char* getStrCalloc(const char* prompt);
char* getStrCallocCensor(const char* prompt);
int readAndShowStr(int mode);
void printStr(const char* str);
int askMode();

int main(){
	
	
	
	while(!readAndShowStr(askMode()));
	
	
	
	return 0;
}

int askMode(){
	int mode = 0;
	printf("Choose mode:");
	if(scanf("%d",&mode) != 1) mode = 0;
	scanf( "%*[^\n]" ); scanf( "%*c" );	
	return mode;
}

int readAndShowStr(int mode){
	const int max_length = 50;
	char* str  = (char*)calloc(max_length,sizeof(char));
	switch(mode){
		case 0:{
			getStrPointer("Type in string:\n", str,max_length );
			printStr(str);
			free(str);
			break;
		}
		case 1:{
			str = getStrCalloc("Type in string:\n");
			printStr(str);
			free(str);
			break;
		}
		case 2:{
			str = getStrCallocCensor("Type in string (do not abuse!):\n");
			printStr(str);
			free(str);	
			break;
		}
		default:{
			printf("Incorrect!\n");
			return 0;
		}
	}
	
	char flag = 'N';
	printf("Type more? Y or N ");
	scanf("%c",&flag);
	printf("\n");
	if(flag == 'Y') 
		return 0;
	
	return 1;
}

void printStr(const char* str){
	printf("Typed: %s\n",str);
}

void getStrPointer(const char* prompt, char* str, int length){
	
	printf(prompt);
	fgets(str, length,stdin);
	
}

char* getStrCalloc(const char* prompt){
	int length = 50;
	char* str  = (char*)calloc(length,sizeof(char));
	printf(prompt);
	fgets(str, length,stdin);	
	return	str;
	
}

void censorStr(char* statement){
	char* num = (char*)1;
	int flag = 1;
	for(int i = 0; i < prof_size;i++){
		while(num != 0){
			num = strstr(statement, profanity[i]);
			if( num != 0)
			{
				for(char* j = num; j < num + strlen(profanity[i]); j++)
					*j = '*';
			}
		}
		num++;
	}
	
}

char* getStrCallocCensor(const char* prompt){
	char* str = "";
	str = getStrCalloc(prompt);
		
	censorStr(str);
	return str;
	
}

