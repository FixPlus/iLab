#include <stdio.h>
#include <stdlib.h>
#include <sys\stat.h>

struct _Text{
	char** lanes;
	unsigned numoflanes;
};

typedef _Text Text;

Text initText(const char* filename);
void save_the_text(const char* filename, Text text);
FILE* open_file(const char* filename);
void freeText(Text* text);
int cmp(const void* s1,const void* s2, int dir);
int compareStrings(char *str1,char *str2, int dir);
Text sortByAlphabet(Text text, int dir);
int filterSybols(int* idx, char c);
int compareChars(char a, char b);
Text copyText(Text text);
int cmpFront(const void* s1,const void* s2);
int cmpBack(const void* s1,const void* s2);

int main(){
	
	Text poem = initText("sort_the_poem.c");
	
	Text sorted = sortByAlphabet(poem,1);
	Text backSorted = sortByAlphabet(poem,-1);
	
	saveText("sort_new.txt",poem);
	saveText("sort_new_sorted.txt",sorted);
	saveText("sort_new_backSorted.txt",backSorted);
	printf("done!");
	
	free(*poem.lanes);
	freeText(&poem);
	freeText(&sorted);
	freeText(&backSorted);
	return 0;
}

Text sortByAlphabet(Text text, int dir){
	Text ret = copyText(text);	
	int (*comparator)(const void*,const void*) = dir > 0 ? &cmpFront: &cmpBack;
	qsort(ret.lanes, ret.numoflanes-1,sizeof(char*), comparator);
	
	return ret;
}

int cmpFront(const void* s1,const void* s2){
	return cmp(s1,s2,1);
}

int cmpBack(const void* s1,const void* s2){
	return cmp(s1,s2,-1);
}

int cmp(const void* s1,const void* s2, int dir){
	char** str1 = (char**) s1;
	char** str2 = (char**) s2;	
	int inc = dir > 0 ? 0:1;
	return compareStrings(*(str1 + inc) - 2*inc, *(str2 + inc) - 2*inc, dir);	
}

int compareStrings(char *str1,char *str2, int dir){
	int comp = 0;
	int idx1 = 0;
	int idx2 = 0;
	
	for(;;){
		if(*(str1+dir*idx1) == 0)
			if(*(str2+dir*idx2) == 0) return 0;
			else
				return 1;		
		else
			if(*(str2+dir*idx2) == 0) return -1;
			else
				if(filterSybols(&idx1, *(str1+dir*idx1)) && filterSybols(&idx2, *(str2+dir*idx2))){
					int comp = compareChars(*(str1+dir*idx1), *(str2+dir*idx2));
					if(comp != 0) 
						return comp;
					
					idx1++;
					idx2++;
				}
		
	}
	
}

int filterSybols(int* idx, char c){

	if((c > 'Z' && c < 'a') || c > 'z' || c < 'A'){
		*idx = *idx + 1;
		return 0;
	}	
	return 1;
}

int compareChars(char a, char b){
	if(a < 97) a += 32;
	if(b < 97) b += 32;
	
	if(a == b)
		return 0;
	else
		if(a < b) return -1;
		else 
			return 1;
}

void freeText(Text* text){
	free(text->lanes);
	text->lanes = NULL;
	text->numoflanes = 0;
	free(text);
}

Text compileFromBuf(char* buf, unsigned length){
	
	unsigned numoflanes = 0;
	
	for(int i = 0; i < length; i++)
		if(buf[i] == '\n') numoflanes++;
	
	Text text = {
		(char**)calloc(numoflanes+1,sizeof(char*)),
		numoflanes
	};
	
	if(length == 0) 
		return text;
	
	numoflanes = 0;
	
	text.lanes[0] = buf;
	
	for(int i = 0; i < length; i++){
		if(buf[i] == '\n'){ 
			
			buf[i] = 0;
			numoflanes++;
			text.lanes[numoflanes] = buf + i + 1;
		}
	}
	
	text.lanes[text.numoflanes] = buf + length;
	
	return text;
	
}

Text initText(const char* filename){
	FILE* file = openFile(filename);
	
	if(file == NULL){
		printf("Cannot open file %s !", filename);
		return compileFromBuf(NULL, 0);
	}

	struct stat st = {};
	stat(filename,&st);

	char* buf = (char*)calloc(st.st_size+1, sizeof(char));
	fseek(file, 0, SEEK_SET);
	fread(buf,sizeof(char),st.st_size, file);
	
	fclose(file);
	
	return compileFromBuf(buf, st.st_size);
}

Text copyText(Text text){
	Text ret = {
		(char**)calloc(text.numoflanes,sizeof(char*)),
		text.numoflanes
	};
	
	for(int i = 0; i < text.numoflanes; i++)
		ret.lanes[i] = text.lanes[i];
	
	return ret;	
}

FILE* openFile(const char* filename){
	FILE* ret = fopen(filename,"r");
	return ret;
}

void saveText(const char* filename, Text text){
	FILE* file = fopen(filename,"w");
	
	for(int i = 0; i < text.numoflanes;i++)
		if(*text.lanes[i] != 0)
			fprintf(file,"%s\n",text.lanes[i]);
		
	fclose(file);
}
