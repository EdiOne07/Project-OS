#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
int count;
void access_rights(struct stat file){
    printf("User:\n");
    if(file.st_mode & S_IRUSR){
        printf("Read:Yes\n");
    }
    else{
        printf("Read:No\n");
    }
    if(file.st_mode & S_IWUSR){
        printf("Write:Yes\n");
    }
    else{
        printf("Write:No\n");
    }
    if(file.st_mode & S_IXUSR){
        printf("Execution:Yes\n");
    }
    else{
        printf("Execution:No\n");
    }
    printf("Group:\n");
    if(file.st_mode & S_IRGRP){
        printf("Read:Yes\n");
    }
    else{
        printf("Read:No\n");
    }
    if(file.st_mode & S_IWGRP){
        printf("Write:Yes\n");
    }
    else{
        printf("Write:No\n");
    }
    if(file.st_mode & S_IXGRP){
        printf("Execution:Yes\n");
    }
    else{
        printf("Execution:No\n");
    }
    printf("OTHERS:\n");
        if(file.st_mode & S_IROTH){
            printf("Read:Yes\n");
        }
        else{
            printf("Read:No\n");
        }
        if(file.st_mode &S_IWOTH){
            printf("Write:Yes\n");
        }
        else{
            printf("Write:No\n");
        }
        if(file.st_mode & S_IXOTH){
            printf("Execution:Yes\n");
        }
        else{
            printf("Execution:No\n");
        }
}
void link_file_name(char* file){
    char arr[1024];
    readlink(file,arr,sizeof(arr)-1);
    printf("The symbolic link name of file %s is %s\n", file,arr);
}
void link_file_size(char* file){
    char arr[1024];
    readlink(file,arr,sizeof(arr)-1);
    struct stat x;
    lstat(arr,&x);
    printf("The symbolic link size of file %s is %ld\n",file,x.st_size);
}
void count_c_files(char* name){
    DIR* dir;
    struct dirent* readv;
    int k=0;
    int length=0;
    dir=opendir(name);
    if(dir!=NULL){
        while((readv=readdir(dir))!=NULL){
            length=strlen(readv->d_name);
            if(strcmp(readv->d_name+length-2,".c")==0){
                k++;
            }
        }
        closedir(dir);
    }else{
        printf("Couldn't open directory");
    }
    printf("The total number of C file is %d:\n",k);
}
void score(char* argv,int errors, int warnings){
    double score=0;
    if(errors & warnings){
        score=10;
    }
    else if(errors>0){
        score=1;
    }
    else if(errors==0 && warnings>10){
        score=2;
    }
    else if(errors==0 && warnings<=10){
        score=2+8*(10-warnings)/10;
    }
    FILE* fo = fopen("grades.txt", "a");
    if(!fo) {
        perror("Could not open file");
        exit(1);
    }
    fprintf(fo,"%s:%.2f\n",argv, score);
    fclose(fo);

}
int main(int argc, char *argv[]){
    struct stat a;
    pid_t process;
    pid_t process2;
    int warnings,errors;
    if(argc<1){
        printf("Invalid input");
        exit(0);
    }
    for(int i=1;i<argc;i++){
        if(lstat(argv[i],&a)==-1){
            perror(argv[i]);
            continue;
        }
            count++;
            process=fork();
            if(process<0){
                perror("Process didn't start");
            }else if(process==0){
        if(S_ISREG(a.st_mode)){
            char c;
            struct stat file;
            char link_name[32];
            lstat(argv[i],&file);
            struct tm* timer=localtime(&file.st_mtime);
            printf("%s is a regular file\n", argv[i]);
            printf("Regular file:\n name(-n)\n size(-d)\n hard link count(-h)\n time of last modification(-m)\n access rights(-a)\n create symbolic link(-l)\n");
            scanf("%c", &c);
            if(c=='-'){
                scanf("%c", &c);
            }
            switch(c){
                case 'n': printf("The name of the file is %s\n",argv[i]); break;
                case 'd': printf("The size of the file is %ld\n",file.st_size); break;
                case 'h': printf("The hard link count is %ld\n",file.st_nlink);break;
                case 'm': printf("The time of the last modification is %ld\n",file.st_mtime);break;
                case 'a': printf("The access rights are:\n");
                          access_rights(file);
                          break;
                case 'l': printf("Input the file name for the symbolic link\n");
                          scanf("%s",link_name);
                          symlink(argv[i],link_name);
                          printf("The symbolic link %s is created",link_name);
                          break;
                default:printf("Not a regular file\n");break;
            }
          }
        else if(S_ISLNK(a.st_mode)){
            char c;
            struct stat file;
            lstat(argv[i],&file);
            printf("%s is a symbolic link file\n",argv[i]);
            printf("Symbolic link:\n name(-n)\n delete symbolic link(-l)\n size of symbolic link(-d)\n size of target file(-t)\n access rights(-a)\n");
            scanf("%c",&c);
            if(c=='-'){
                scanf("%c",&c);
            }
            switch(c){
                case 'n': link_file_name(argv[i]); break;
                case 'l': unlink(argv[i]);
                          printf("The link was deleted\n");break;
                case 'd': printf("The size of the link file is %ld\n",file.st_size);break;
                case 't': link_file_size(argv[i]);break;
                case 'a': access_rights(file);break;
                default:printf("Not a symbolic link file\n");break;
            }
        }
        else if(S_ISDIR(a.st_mode)){
            char c;
            struct stat file;
            lstat(argv[i],&file);
            printf("%s is a directory\n",argv[i]);
            printf("Directory:\n name(-n)\n size(-d)\n access rights(-a)\n total number of file with the .c extension(-c)\n");
            scanf("%c",&c);
            if(c=='-'){
                scanf("%c",&c);
                    
            }
            switch(c){
                case 'n': printf("The directory name is %s:\n",argv[i]);break;
                case 'd': printf("Size of directory is %ld:\n",file.st_size);break;
                case 'a': access_rights(file);break;
                case 'c': count_c_files(argv[i]);break;
                default:printf("Not a directory\n");break;
            }
        }
        else{
            printf("File given as argument is not a good input\n");
        }
        exit(0);
    }
    if(S_ISREG(a.st_mode)){
        int pfd[2];//0=write, 1=read
        if(pipe(pfd)==-1){
            perror("Couldn't make pipe");
            exit(2);
        }
        count++;
        process2=fork();
        if(process2<0){
        perror("Couldn't open process");
        exit(1);
    }
    else if(process2==0){
        close(pfd[0]);
        int length_for_c=strlen(argv[i]);
        if(strcmp(argv[i]+length_for_c-2,".c")==0){
            dup2(pfd[1],1);
            execlp("./script.sh","./script.sh",argv[i],NULL);
            close(pfd[1]);
            exit(0);
        }
        else{
            execlp("wc","wc","-l",argv[i],NULL);
            exit(0);
        }
        exit(0);
    }
    else{
        int length_for_c=strlen(argv[i]);
        close(pfd[1]);
        FILE *f=fdopen(pfd[0],"r");
        if(!f){
            perror("Error opening the file");
            exit(3);
        }
        fscanf(f,"%d",&errors);
        fscanf(f,"%d",&warnings);
        close(pfd[0]);
        if(strcmp(argv[i]+length_for_c-2,".c")==0){
            score(argv[i],warnings,errors);
        }
       
     }
    }
    if(S_ISDIR(a.st_mode)){
        count++;
        process2=fork();
        if(process2<0){
            perror("Couldn't start process");
            exit(1);
        }
        else if(process2==0){
            char s[257];
            strncat(s,argv[i],247);
            strcat(s,"_file.txt");
            if(execlp("./script_dir.sh","./script_dir.sh",s,NULL)==-1){
                printf("Couldn't run script\n");
            }
            exit(0);
        }
    }
    if(S_ISLNK(a.st_mode)){
        count++;
        process2=fork();
        if(process2<0){
            perror("Couldn't start process");
            exit(1);
        }else if(process2==0){
            execlp("chmod","chmod",760,argv[i],NULL);
        }
    }
     for(int i=0;i<count;i++){
        int status;
        wait(&status);
        if(WIFEXITED(status)){
            int exitcode=WEXITSTATUS(status);
            printf("The process with PID %d has ended with exit code %d\033[0m\n", process2, exitcode);
        }else{
            printf("The process with PID %d has terminated abnormally\n", process2);
        }
     }
  }
   return 0;
}