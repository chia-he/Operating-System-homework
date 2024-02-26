#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int count = 0;
void search(char *dir, char *targetfile, char *uptargetfile){

	/*search from given directory*/
	DIR* pDir = opendir(dir);
	struct dirent* pEntry = NULL;
	/*read one-by-one*/
	while((pEntry = readdir(pDir)) != NULL){
		/*a file*/ 
		if(pEntry->d_type == DT_REG){
			/*find one target.*/	
			if(!strcmp(pEntry->d_name, targetfile)
				|| !strcmp(pEntry->d_name, uptargetfile)){
				++count;
				printf("found in %s/%s\n", dir, pEntry->d_name);
			}
		}/*a directory*/
		else if(pEntry->d_type == DT_DIR){
			/*skip "." and ".."*/
			if(!strcmp(pEntry->d_name, ".") || !strcmp(pEntry->d_name, "..")){
				continue;
			}
			/*skip /run/user/1000/gvfs*/
			if(!strcmp(pEntry->d_name, "gvfs")){
				continue;
			}			
			char* result = malloc(strlen(dir) + 
					strlen(pEntry->d_name) + strlen("/") + 1);
			/*just for matching find result*/
			if(!strcmp(dir, "/")){
				strcpy(result,"");
			}else{			
				strcpy(result, dir);
			}
			strcat(result, "/");
			strcat(result, pEntry->d_name);
			search(result, targetfile, uptargetfile);
			free(result);
			
		}
		
	}
	closedir(pDir);
}
int main(int argc, char *argv[]){
	
	/*Case changed.*/
	char* upargv = malloc(strlen(argv[argc-1]) + 1);	
	int i = 0;	
	while(argv[argc-1][i]){
		upargv[i] = toupper(argv[argc-1][i]);
		i++;
	}
	/*argc = 1 means no given parameter.*/
	if(argc == 3){
		/*find argv[2] ,start from argv[1].*/
		search(argv[1], argv[2], upargv);
	}
	else if(argc == 2){
		/*default : find argv[1] ,start from current directory.*/		
		search(".", argv[1], upargv);
		
	}
	if(count==0){
		printf("cannot find the corresponding file.\n");
	}	
	return 0;
}
