#ifndef __LOGGERINPUT_H__

#define __LOGGERINPUT_H__

#define SUCCESS 1
#define FAILURE 0



char log_path[128], file_name[128], log_extension[128]; 

int min_bufsize=64;
long int rotation_size;
long int file_size=0,curr_file_size=0;
long int curr_buffsize=0,buffer_size=0;
int level=0;
enum logger
{
    ALWAYS = 0,
    ERROR = 1,
    WARN = 2,
    DEBUG = 3
};

typedef enum logger loglevel;
char *level_str[]={"ALWAYS","ERROR","WARN","DEBUG"};

char *buff;
FILE *fp;
char logger_file_name[128]={0};
int logger_init=0;

void  myflush();
int readConfiguration(char *file_name_path);
int getFileSize();
int move_file_rename();

#endif

