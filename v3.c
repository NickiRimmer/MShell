#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <filesystem>

void hdl(int sig)
{
printf("Config reload");
}

int main()
{

struct sigaction act;
memset(&act, 0, sizeof(act));
act.sa_handler = hdl;
sigset_t set;
sigemptyset(&set);
sigaddset(&set, SIGHUP);
act.sa_mask = set;
sigaction(SIGHUP, &act, 0);

int pi = getpid();

printf("Hello world!\nMy PID is: %d\n", pi);
char str[100];
char ssssss[100];
FILE *f = fopen("history.txt", "a");

printf("$ ");
while((scanf("%s", str)) != -1)
{
fprintf(f, "%s", str);
//printf("%s\n", strncpy(ssssss, str, 4));

if (strcmp(str, "exit") == 0 || strcmp(str, "\\q") == 0){
fprintf(f, "\n");
break;
}

else if (strcmp(str, "echo") == 0){
char str2[100];
scanf("%s", str2);
fprintf(f, " %s", str2);
printf("%s\n", str2);
}

else if (strcmp(str, "\\e") == 0){
char str2[100];
scanf("%s", str2);
fprintf(f, " %s", str2);
char* eeee = getenv(str2);
printf("%s\n", (eeee==NULL ? "NULL" : eeee));
}

else if (strstr(str, "/bin")!=NULL && strstr(str, "/bin")-str==0){
//else if (strlen(str)>4 && strcmp({str[0], str[1], str[2], str[3]}, "/bin")){
system(str);
printf("\n");
}

else if(strcmp(str, "\\l") == 0){

char disk[100];
scanf("%s", disk);
fprintf(f, " %s", disk);
int fd = open(disk, O_RDONLY);
if(fd<0){
printf("Error while open disk\n");
continue;
}
unsigned char buffer[513];
ssize_t readed = read(fd, buffer, 512);
if(readed<0){
printf("Error while read disk sector 0\n");
close(fd);
continue;
}
printf("Last 4 bytes of 0 sector is: %02x %02x\n", buffer[510], buffer[511]);
if (buffer[510]==85 && buffer[511]==170){
printf("It's a load disk!\n");
}
else {
printf("It is NOT a load disk!\n");
}

//printf("%d %d\n", buffer[510], buffer[511]);
//printf("This is #10!\n");
}

else if(strcmp(str, "\\cron") == 0){
char str2[100];
scanf("%s", str2);
fprintf(f, "%s", str2);

bool crdir = create_directory("/tmp/vfs");
if (crdir){
mount("/tmp/vfs", str2, "ext2", 0, NULL);
printf("VFS successfully created!\n");
}
else {
printf("Unable to create filder vfs\n");
}
//mount(

printf("This is #11!\n");
}

else if(strcmp(str, "\\mem") == 0){
printf("This is #12!\n");
}

else {
printf("\"%s\" is not a standart MShell command. Try better.\n", str);
}
printf("$ ");
fprintf(f, "\n");
}

printf("\n");
fclose (f);
return 0;
}
