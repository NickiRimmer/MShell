#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

void hdl(int sig)
{
printf("Config reload");
}


int main()
{
//self-handler for SIGHUP signal
struct sigaction act;
memset(&act, 0, sizeof(act));
act.sa_handler = hdl;
sigset_t set;
sigemptyset(&set);
sigaddset(&set, SIGHUP);
act.sa_mask = set;
sigaction(SIGHUP, &act, 0);

//start of programm
int pi = getpid();

printf("Hello world!\nMy PID is: %d\n", pi);
char str[100];
char ssssss[100];
FILE *f = fopen("history.txt", "a");

printf("$ ");
while((scanf("%s", str)) != -1)
{
fprintf(f, "%s", str);

//exit func
if (strcmp(str, "exit") == 0 || strcmp(str, "\\q") == 0){
fprintf(f, "\n");
break;
}

//echo func
else if (strcmp(str, "echo") == 0){
char str2[100];
scanf("%s", str2);
fprintf(f, " %s", str2);
printf("%s\n", str2);
}

//env var \e func
else if (strcmp(str, "\\e") == 0){
char str2[100];
scanf("%s", str2);
fprintf(f, " %s", str2);
char* eeee = getenv(str2);
printf("%s\n", (eeee==NULL ? "NULL" : eeee));
}

//bin exp func
else if (strstr(str, "/bin")!=NULL && strstr(str, "/bin")-str==0){

int fork_pi = fork();
if (fork_pi < 0){
printf("Something goes wrong...");
}
else if (fork_pi == 0) {
execl(str, str, NULL);
}
else {
waitpid(fork_pi, NULL, 0);
}
}

//chech if disk is load
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

}

//VFS mount and cron adder func
else if(strcmp(str, "\\cron") == 0){
char str2[100];
scanf("%s", str2);
fprintf(f, "%s", str2);

struct stat st = {0};
int drcr = stat("/tmp/vfs", &st);
//printf("%d\n", drcr);
if (drcr == -1){
mkdir("/tmp/vfs", 0777);
}

drcr = stat("/tmp/vfs", &st);
//printf("%d\n", drcr);
if (drcr==0){

int ismounted = mount(str2, "/tmp/vfs", "ext2", 0, NULL);
if (ismounted == 0){
printf("VFS successfully created!\n");

drcr = stat("/tmp/vfs/etc", &st);
if (drcr == -1){
mkdir("/tmp/vfs/etc", 0777);
}

FILE *crn = fopen("/tmp/vfs/etc/crontab", "a");
fprintf(crn, "0 0 * * 3 root echo Hello!");
fclose(crn);
}
else {
printf("Error occured: %s, %d\n", strerror(errno), errno);
}

}
else {
printf("Unable to create/search folder \"/tmp/vfs/\"\n");
}

//printf("This is #11!\n");
}

else if(strcmp(str, "\\mem") == 0){
printf("This is #12! (would be made sometimes)\n");
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
