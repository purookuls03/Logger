#ifndef __LOGGERH__

#define __LOGGERH__

enum logger
{
    ALWAYS = 0,
    ERROR = 1,
    WARN = 2,
    DEBUG = 3
};

typedef enum logger loglevel;

void init(char *file_name_path);

int logger(loglevel lvl,char* filename,int linenum,char *format, ...);

void deinit();

#endif

