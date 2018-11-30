#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "scannerCSVsorter.h"
static int *totalp;

void sortFile(char * fileName, char * col,char * out, char * end){
    end[strlen(end)-4] = 0;
    
    FILE * opener = fopen(fileName, "r");    
    if (opener ==NULL){
        
        write(2, "Error opening file", 19);
        return;
    }
    
    int i=0;
    char pres;
    char t[300];
    char names[500];
    memset(t, 0, sizeof(t));
    char headers[28][50];
    int colcount = 0;
    while (pres !='\n'){
        pres = getc(opener);
        names[strlen(names)] = pres;
        if (pres==' '){
            continue;
        }
        if (pres == '\n'){
            colcount++;
            if(colcount > 28){
                write(2, "too many columns\n", 18);
                return;
            }
            
            break;
            
        }
        if (pres == ','){
            colcount++;
            if(colcount > 28){
                write(2, "too many columns\n", 18);
                return;
            }
            strcpy(headers[i],t);
            i++;
            memset(t, 0, sizeof(t));
            
        }else{
            t[strlen(t)] = pres;
            
        }
    }
    
    strcpy( headers[i], t);
    i++;
    memset(t, 0, sizeof(t));
    
    
    int key = getKey(col, headers);
    int strOrInt=0;
    if (strcmp(col, "num_critic_for_reviews")==0||strcmp(col, "duration")==0||strcmp(col, "director_facebook_likes")==0||strcmp(col, "actor_3_facebook_likes")==0||
        strcmp(col, "actor_1_facebook_likes")==0||strcmp(col, "gross")==0||strcmp(col, "num_voted_users")==0||strcmp(col, "cast_total_facebook_likes")==0||
        strcmp(col, "facenumber_in_poster")==0||strcmp(col, "num_user_for_reviews")==0||strcmp(col, "budget")==0||strcmp(col, "title_year")==0||
        strcmp(col, "actor_2_facebook_likes")==0||strcmp(col, "imdb_score")==0||strcmp(col, "aspect_ratio")==0||strcmp(col, "movie_facebook_likes")==0){
        strOrInt = 1;
        }
        
    if(key==-1){
                /*char error[]="Error: Column Not in CSV\n";
                int errLen= strlen(err);
                write(STDERR, error, errlen);
            */
        write(2, "Invalid Column Name\n", 21);
        exit(0);
    }
    
    char save[500];
    
    int quoteFlag = 1;
    char str[500];
    char temp[500];    
    char * pass;
    int count =0;
    int linecount=0;
    
    
    memset(save, 0, sizeof(save));
    
    memset(temp, 0, sizeof(temp));
    
    
        
    
    struct Node * head = NULL;
    struct Node * ptr = head;
    while (pres!=EOF){
        pres = getc(opener);
        if (pres==EOF){
            linecount++;
            if (key == count){
                strcpy(save, str);
            }            
            
            pass = temp;
            struct Node * n = (struct Node *) malloc(sizeof(struct Node));
            n->soi = strOrInt;
                        
            if (save[0] == ','){
                memmove(save, save+1, strlen(save));
            }
            
            strcpy(n->line, pass);
            while (save[0]==' ' || save[0] == '"'){
                memmove(save, save+1, strlen(save));
            }
            while (save[strlen(save)-1] == ' ' || save[strlen(save)-1] == '"'){
                save[strlen(save)-1] = '\0';
            }
            strcpy(n->value, save);
            n->next = NULL;
            if (head==NULL){
                head = n;
                ptr = head;
            }else{
                ptr = head;
                while(ptr->next!=NULL){
                    ptr = ptr->next;
                }
                ptr->next = n;
                ptr = n;
            }
            
            
            
            break;
        }
        
        if (pres=='"'){
            quoteFlag = quoteFlag * -1;
        }
        if (pres==','){
            
            if (quoteFlag == 1){
                linecount++;
                if (count == key){
                    
                    strcpy(save, str);
                
                }
                count++;
                memset(str, 0, sizeof(str));
            }
            
        }
        if (pres=='\n'){
            linecount++;
            if (linecount!=colcount){
                write(2, "Column mismatch!\n", 18);
                return;
            }
            linecount=0;
            pass = temp;
            if (count == key){
                
                strcpy(save, str);
                                    
            }
        
            struct Node * n = (struct Node *) malloc(sizeof(struct Node));
            n->soi = strOrInt;
            if (save[0] == ','){
                memmove(save, save+1, strlen(save));
            }
            while (save[0]==' ' || save[0] == '"'){
                memmove(save, save+1, strlen(save));
            }
            while (save[strlen(save)-1] == ' ' || save[strlen(save)-1] == '"'){
                save[strlen(save)-1] = '\0';
            }
            strcpy(n->line, pass);
            
            strcpy(n->value, save);
        
            
            
            n->next = NULL;
            if (head==NULL){
                head = n;
                ptr = head;
            }else{
                ptr = head;
                while(ptr->next!=NULL){
                    ptr = ptr->next;
                }
                ptr->next = n;
                ptr = n;
            }
            if(head!=NULL){
                //printf("%s\n", head->line);
            }
            
            
            
            
            
            count=0;
            memset(temp, 0, sizeof(temp));
            memset(str, 0, sizeof(str));
        }else {
            
            
            temp[strlen(temp)] = pres;
            
            str[strlen(str)] = pres;
            
        }
    }    
    
    
    Msort(&head);
    
    
    
    //char *base;
    char *extension;
    extension=strrchr(fileName, '.');
    const char* substring = strstr(fileName, extension);
    
    //size_t bsize=0;
    size_t baseLen= substring - fileName;
    //base=malloc(bsize+1);
    //memcpy(base,fileName,baseLen);
    //base[baseLen]='\0';
    
    char* outputFileName=(char*)malloc(sizeof(char)*(strlen(end)+strlen(out)+30));
    
    strcat(outputFileName,out);
    strcat(outputFileName,"/");
    strcat(outputFileName,end);
    strcat(outputFileName,"-sorted-");
    strcat(outputFileName,col);
    strcat(outputFileName,".csv");
    FILE * outputFile = fopen(outputFileName, "r");
    if (outputFile!=NULL){
        write(2, "output file already exists\n", 28);
        return;
    }else{
        outputFile= fopen(outputFileName, "ab");
    }
    
    
    fprintf(outputFile,"%s", names);
    
    
    struct Node* lastptr=head;
            while (lastptr!=NULL){
                    if (strlen(lastptr->line) > 2){
                            fprintf(outputFile,"%s\n",lastptr->line);
                    }

                    lastptr = lastptr->next;
            }
            
    free(lastptr);
    
    
    
    
    return;
    
}
int checkHeaders(char * sortBy, char headers[28][50]){
    
    int i;
    for(i=0;i<28;i++){
        
        if (strcmp(sortBy, headers[i])==0){
        
            return i;
        }
    }
    return -1;
    
}
int getKey(char * sortBy, char headers[28][50]){
    int key;
   if (strcmp(sortBy, "color")==0){
        key = checkHeaders("color", headers);
        
    }
    else if (strcmp(sortBy, "director_name")==0){
        key = checkHeaders("director_name", headers);
        
    }
    else if (strcmp(sortBy, "num_critic_for_reviews")==0){
        key = checkHeaders("num_critic_for_reviews", headers);
        
    }
    else if (strcmp(sortBy, "duration")==0){
        key = checkHeaders("duration", headers);
        
    }
    else if (strcmp(sortBy, "director_facebook_likes")==0){
        key = checkHeaders("director_facebook_likes", headers);
    
    }
    else if (strcmp(sortBy, "actor_3_facebook_likes")==0){
        key = checkHeaders("actor_3_facebook_likes", headers);
        
    }
    else if (strcmp(sortBy, "actor_2_name")==0){
        key = checkHeaders("actor_2_name", headers);
        
    }
    else if (strcmp(sortBy, "actor_1_facebook_likes")==0){
        key = checkHeaders("actor_1_facebook_likes", headers);
        
    }
    else if (strcmp(sortBy, "gross")==0){
        key = checkHeaders("gross", headers);
        
    }
    else if (strcmp(sortBy, "genres")==0){
        key = checkHeaders("genres", headers);
        
    }
    else if (strcmp(sortBy, "actor_1_name")==0){
        key = checkHeaders("actor_1_name", headers);
        
    }
    else if (strcmp(sortBy, "movie_title")==0){
        key = checkHeaders("movie_title", headers);
        
    }
    else if (strcmp(sortBy, "num_voted_users")==0){
        key = checkHeaders("num_voted_users", headers);
        
    }
    else if (strcmp(sortBy, "cast_total_facebook_likes")==0){
        key = checkHeaders("cast_total_facebook_likes", headers);
        
    }
    else if (strcmp(sortBy, "actor_3_name")==0){
        key = checkHeaders("actor_3_name", headers);
        
    }
    else if (strcmp(sortBy, "facenumber_in_poster")==0){
        key = checkHeaders("facenumber_in_poster", headers);
        
    }
    else if (strcmp(sortBy, "plot_keywords")==0){
        key = checkHeaders("plot_keywords", headers);
        
    }
    else if (strcmp(sortBy, "movie_imdb_link")==0){
        key = checkHeaders("movie_imdb_link", headers);
        
    }
    else if (strcmp(sortBy, "num_user_for_reviews")==0){
        key = checkHeaders("num_user_for_reviews", headers);
        
    }
    else if (strcmp(sortBy, "language")==0){
        key = checkHeaders("language", headers);
        
    }
    else if (strcmp(sortBy, "country")==0){
        key = checkHeaders("country", headers);
        
    }
    else if (strcmp(sortBy, "content_rating")==0){
        key = checkHeaders("content_rating", headers);
        
    }
    else if (strcmp(sortBy, "budget")==0){
        key = checkHeaders("budget", headers);
        
    }
    else if (strcmp(sortBy, "title_year")==0){
        key = checkHeaders("title_year", headers);
        
    }
    else if (strcmp(sortBy, "actor_2_facebook_likes")==0){
        key = checkHeaders("actor_2_facebook_likes", headers);
        
    }
    else if (strcmp(sortBy, "imdb_score")==0){
        key = checkHeaders("imdb_score", headers);
        
    }
    else if (strcmp(sortBy, "aspect_ratio")==0){
        key = checkHeaders("aspect_ratio", headers);
        
    }
    else if (strcmp(sortBy, "movie_facebook_likes")==0){
        key = checkHeaders("movie_facebook_likes", headers);
        
    }
    else {
        
        return -1;
    }
    return key;
}
struct Node* freeptr(struct Node* ptr){
        if (ptr == NULL){

                return NULL;
        }
    free(freeptr(ptr->next));

        return NULL;

}
void printList(FILE * fout, struct Node * p){
    

        while (p!=NULL){
                if (strlen(p->line) > 2){
                        fprintf(fout, "%s\n", p->line);
                }


                p = p->next;


        }
        fclose(fout);
    return;
}

void traverseDir(char *dirName, char * output, char *sortBy) {
    char * newOut= (char*)malloc(sizeof(char)*500);
    strcpy(newOut, output);
    
    DIR *dir;
    struct dirent *item;
    int counter = 0;
    dir = opendir(dirName);
    
    int pid1 = 0; 
    int pid2 = 0;
    
    if(dir == NULL) { 
        write(2, "Directory does not exist\n", 26);
        
        exit(0); 
    }
    
    while((item = readdir (dir)) != NULL) {
        
        if (strcmp(item->d_name, ".") == 0 || strcmp(item->d_name, "..") == 0) {
            continue;
        }
        
        if(item->d_type == 4) {
            //printf("Found Dir %s\n", item->d_name);
            
            fflush(0);
            pid1 = fork();
            
            if(pid1 == 0) {
                printf("%d,", getpid()); fflush(stdout);
                *totalp+=1;
                /*
                char * pathway= (char*)malloc(sizeof(char)*500);
                pathway=dirName;
                char * const hold = output;
                strcat(pathway,"/");
                printf("MIDDLE TRAVERSE: %s\n", hold);
                
                strcat(pathway,item->d_name);
                */
                strcat(dirName, "/");
                strcat(dirName, item->d_name);
                
                traverseDir(dirName, newOut, sortBy);                
                exit(0);
            }
        } else {
                if(!(strstr(item->d_name, "-sorted")) && (strlen(item->d_name)>4 && strcmp(item->d_name + strlen(item->d_name - 4), ".csv"))){
                    //printf("Found File %s\n", item->d_name);
                    counter++;
                    fflush(0);
                    pid2 = fork();
                    
                    if(pid2 == 0) {
                        printf("%d,", getpid()); fflush(stdout);
                        *totalp+=1;
                        /*
                        char * pathway= (char*)malloc(sizeof(char)*500);
                        pathway= dirName;
                        strcat(pathway,"/");
                        strcat(pathway,item->d_name); 
                        //FILE *file = fopen(pathway, "r");
                        */
                        strcat(dirName, "/");
                        strcat(dirName, item->d_name);
                        
                        sortFile(dirName, sortBy, newOut, item->d_name);
                        exit(0);
                    }                
                }
        }
    }
   
    if (pid1 != 0) {    
        wait(0); 
    }
    
    if (pid2 != 0) {
        while(counter > 0) { 
            wait(0); 
            counter--;
        }
    }

    return;
}

/*
void fileHandler(char * nameToSort, char * column, char * out){
    //check nameToSort to see if it is a CSV
    if(strstr(nameToSort, "-sorted")){
        return;
    }
        
    else if((strstr(nameToSort, ".csv")) == NULL){
        return;
    }else{
        struct Node* h = sortFile(nameToSort, column, out);
        
        freeptr(h);
    }
    

}*/

int main(int argc, char **argv){
    int status;
    totalp = mmap(NULL, sizeof *totalp, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *totalp=0;
    
    fflush(0);
    printf("Initial PID: %d\n", getpid());
    
    printf("PIDS of all child processes: ");
    
   if (argc==4||argc==6||argc < 3||argc > 7){
                write(2, "Argument Error", 15);
            return 0;
   }
        
    if(strcmp(argv[1],"-c")!=0){
                return 0;
   }
    
        
            char * dirName= (char*)malloc(sizeof(char)*100);
        char * outputDir= (char*)malloc(sizeof(char)*100);
        strcpy(dirName,".");
        strcpy(outputDir,".");
         
        
        
        int i;
        
        for(i=3;i<argc;i++){
                if(i==3 || i==5){
                    
                    
                        if(strcmp(argv[i],"-d")==0){
                            
                            if (strcmp(argv[i+1], "-o")==0){
                                write(2, "Argument Error", 15);
                                return 0;
                            }
                                dirName= argv[i+1];
                        }
                        else if (strcmp(argv[i],"-o")==0){
                            
                            if (strcmp(argv[i+1], "-d")==0){
                                write(2, "Argument Error", 15);
                                return 0;
                            }
                                outputDir=argv[i+1];
                               
                                
                        }
                        
                }
                
        }
        
        
        
    traverseDir(dirName, outputDir, argv[2]);
    //free(dirName);
    //free(outputDir);
    printf("\nTotal number of processes: %d\n", *totalp);
    return 1;
}
