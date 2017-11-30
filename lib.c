#include <stdlib.h>
#include <stdio.h>
#include "lib.h"
#include <unistd.h>
#include <string.h>
#include <regex.h>        


action * actions;
int actionsLen=0;

int isVariable (char * string){
	 int    status;
    regex_t    re;

    if (regcomp(&re, "[a-zA-Z]([a-zA-Z0-9_])*", REG_EXTENDED|REG_NOSUB) != 0) {
        return(0);      /* Report error. */
    }
    status = regexec(&re, string, (size_t) 0, NULL, 0);
    regfree(&re);
    if (status != 0) {
        return(0);      /* Report error. */
    }
    return(1);
}

void printMovement (char movement [ACTION_LENGTH][FRAME_WIDTH][FRAME_HEIGHT], int position, int direction){
	int i,j,k,l;
	int offset = FRAME_WIDTH*(SCREEN_SPACES - position);
	if(position > SCREEN_SPACES){
		printf("Inavlid postion\n");
	}
	

	char spaces[FRAME_WIDTH*SCREEN_SPACES+1]={0};
	for(k=0; k<FRAME_WIDTH*SCREEN_SPACES ;k++){
		spaces[k]=' ';
	}
	spaces[k]=0;

	

	for (i=0; i<ACTION_LENGTH ;i++){
		for(j=0; j<FRAME_HEIGHT; j++){
			printf("%s", (spaces + (i*2)*direction  +offset));
			for(int k=0 ; k< FRAME_WIDTH; k++){
				printf("%c",movement[i][j][k]);
			}
			printf("\n");
		}
		sleep(1);
		system("clear");
	
	}
}





void fillFrames(FILE * fp ,action a){
	char c;
	int i,j,k;
	for (i = 0; i< ACTION_LENGTH ;i++){
		for(j = 0; j< FRAME_HEIGHT; j++){			
			for(k=0 ;(c =fgetc(fp)) != '\n' && k< FRAME_WIDTH; k++){
				a.frames[i][j][k] = c;
			}
				/*Complete rest of the line with ' ' */
			if(k<FRAME_WIDTH){
				while(k!=FRAME_WIDTH ){
					a.frames[i][j][k++]=' ';
				}
			}
			
		}
	}
}


void openActions(char * fileRoute){
	FILE *fp;
	fp = fopen(fileRoute, "r");	
	while ( 1 ){
		int i,j,k,c;
		char buffer [200]={0};
		i=0;
		action a;
		if((c =fgetc(fp)) == EOF){
				break;
		}else{
			ungetc(c,fp);
		}
		while ( (c =fgetc(fp)) != EOF ){
			if( c == '[' ){
				buffer[i]=0;
				
				if( isVariable(buffer) ){
					a.name =malloc(strlen(buffer));
					strcpy(a.name,buffer);
					printf("variable in buffer: %s in action: %s\n",buffer,a.name );
					
				} else {
					printf("Not a valid name for library input: %s \n",buffer );
					return ; 
				}
				i=0;
				buffer[i++]=c;
				while((c =fgetc(fp)) != '\n' ){
					buffer[i++]=c;
				}
				buffer[i]=0;
				if ( strcmp(buffer,"[right]" ) == 0){
					a.direction = RIGHT;
					break;
				}else if ( strcmp(buffer,"[left]" ) == 0){
					a.direction = LEFT;
					break;
				}else if ( strcmp(buffer,"[front]" ) == 0){
					a.direction = FRONT;
					break;
				}else {
					printf( "Not a valid attrbute for library input: %s\n",buffer);
					return;
				}

			}else {
				buffer[i++]=c;
			}
		}
		if ( c == EOF ){
			printf("Not a valid stick library.\n");
			return;
		}
		fillFrames(fp, a);
		

		
		printf("Name: %s direction: %d \n",a.name,a.direction );
		

		
		actions= realloc( actions, sizeof(action)*(actionsLen+1) );
		memcpy( &(actions[actionsLen]) , &a , sizeof(action) );
		actionsLen++;
		
	}

	for (int i = 0; i < actionsLen; ++i)
	{
		//printMovement( actions[i].frames,4 ,actions[i].direction);
	}
		
	fclose(fp);

}
		

int main(int argc, char const *argv[]){
		openActions("lib.stickLib");
	return 0;
}


