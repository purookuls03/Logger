#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include "logger_input.h"



void init(char *file_name_path)
{
    if(SUCCESS==readConfiguration(file_name_path))
    {
        buff= (char *) malloc(buffer_size*sizeof(char));        /* Buff use as dynamic memory */
        memset(buff,0,buffer_size);                         /* memset() is used to fill a block of memory with a particular value */
        logger_init=1;
        move_file_rename();
        printf("\ntask is completed\n");   
    }
    else
        printf("logger is not intlized\n");
}

void deinit()
{
    myflush();
    fclose(fp);
    free(buff);
}

int readConfiguration(char *ini_file)
{
    // parse the logger.ini, extract the 3 parameter and cocatenate them and store it into file
    FILE *fp_ini;

    fp_ini = fopen(ini_file,"r+");
    if(fp_ini==NULL)
    {
        printf("File is empty\n");
        return FAILURE;
    }
 
    char line[500];
    char *str;
    
    while (fgets(line, sizeof(line), fp_ini)) 
    {
        if(line[0]==';' || line[0]=='#')  /* It reads ";" and "#" line as comment line */
        {
            continue;
        }
        
    /*  Takes all input from "logger.ini"   */ 
                                            
        if(strstr(line,"LOG_Path=")!=NULL)
        {
            str=line+strlen("LOG_Path=");
            strcpy(log_path,str);
            // printf("\nLog_path = %s", log_path);
            log_path[strlen(str)-1]='\0';   
        }
        else if(strstr(line,"FileName=")!=NULL)
        {
           str=line+strlen("FileName=");
            strcpy(file_name,str);
            // printf("\nFileName = %s", file_name);
            file_name[strlen(str)-1]='\0';
        }
        else if(strstr(line,"extension=")!=NULL)
        {
            str=line+strlen("extension=");
            strcpy(log_extension,str);
            // printf("\nextension = %s\n", log_extension);
            log_extension[strlen(str)-1]='\0';
        }
        else if(strstr(line,"rotationsize=")!=NULL)
        {
            str=line+strlen("rotationsize=");
            // strcpy(size,str);
            str[strlen(str)-1]='\0';
            rotation_size=atol(str);
            printf("SIZE = %d\n", rotation_size);
        }
        else if(strstr(line,"buffersize=")!=NULL)
        {
            str=line+strlen("buffersize=");
            // strcpy(size,str);
            str[strlen(str)-1]='\0';
            buffer_size=atol(str);
            if(min_bufsize>buffer_size)
                buffer_size=min_bufsize;
            printf("SIZE = %d\n", buffer_size);
        }
        else if(strstr(line,"loglevel=")!=NULL)
        {
            str=line+strlen("loglevel=");
            str[strlen(str)-1]='\0';
            level=atoi(str);
            printf("level = %d\n", level);
        }
    }
        
        printf("\nLog_path = %s", log_path);
        printf("\nFileName = %s", file_name);
        printf("\nextension = %s\n", log_extension);
        // strcat(file_name,log_extension);
        // strcat(log_path,file_name);
        // printf("After appending the string = %s", log_path);
        sprintf(logger_file_name,"%s\\%s.%s",log_path,file_name,log_extension);
        printf("\n%s\n",logger_file_name);
        
        fclose(fp_ini);
        
        fp = fopen(logger_file_name,"a+");
        if(fp==NULL)
        {
            printf("Error in opening the file");
            return FAILURE;
        }
        return SUCCESS;
        
}

int logger(loglevel lvl ,char* filename,int linenum,char *format, ...)                     // Logger Function
{
    printf("level in logger function = %d\n", level);
    printf("loglevel in logger function = %d\n", lvl);
    
    if(lvl>level)           /* It checks the condition of total enum with "lgger.ini"  */
    {
        return SUCCESS;
    }
    
    if(logger_init==0)
    {
        printf("logger is not intlized properly\n");
        return 0;
    }
    
    int buflen=strlen(buff);
    char str[1024]={0};
    char tempstr[1024]={0};
    va_list args;
    va_start(args, format); 
    
    vsprintf(tempstr,format,args);
    printf("str before adding levl=%s\n",tempstr);
    va_end(args);
    sprintf(str,"%s : %s:file[%s]:%d\n",level_str[lvl], tempstr,filename,linenum);

    printf("str=%s,current size of buff=%d,current size of log=%d,buffer_size[%d]\n",str,buflen,strlen(str),buffer_size);
    if(strlen(buff)+strlen(str) >= buffer_size)
    {
        myflush();
    }
    
    sprintf(buff,"%s%s",buff,str);
    printf("total buff=%s\n",buff);
    
    return 1;
}

void  myflush()     /* This function is basically built for moving the file after a specific size of a file and save the filename with Local Date_Time */
{
    //printf("In myFlush, total buff=%s\n",buff);
    fprintf(fp,buff);
    fflush(fp);
    curr_file_size=curr_file_size+strlen(buff);
    if(curr_file_size>rotation_size)
    {
       move_file_rename();
       curr_file_size=0;
    }
    memset(buff,0,buffer_size);                  /* memset() is used to fill a block of memory with a particular value */

}

int move_file_rename()              /* Function for to change the file name with local Date Time */
{
    // printf("Total file size: %lu bytes\n", sb.st_size);
    char new_file[1024]={0};
    new_file[0]='\0';
    char text[64]={0};
    
    /* Defined in time.h built in library */
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    struct tm* tm_info;
    struct timeval tv;

    mingw_gettimeofday(&tv, NULL);
        
    strftime(text, sizeof(text)-1, "_%d%m%Y_%H%M%S", t);       /* Include Date_Time in file name */
        
    sprintf(text,"%s-%ul",text,tv.tv_usec);
    text[26] = '\0';
    printf("\ntxt==%s\n", text);

    /* Append log_path,file_name,text,log_extension and some requriments for new file */
    strcat(new_file,log_path);                     
    strcat(new_file,"\\");
    strcat(new_file,file_name);                 
    strcat(new_file,text);
    strcat(new_file,".");
    strcat(new_file,log_extension);

    printf("\nNewfilename = %s\n",new_file);
    printf("\noldfilename = %s\n",logger_file_name);
    
    fclose(fp);
        
    int ret = rename(logger_file_name, new_file);           // Move the File  
        
	if(ret == 0) 
    {
        printf("File renamed successfully\n");
    }     
    else 
    {
        printf("Error: unable to rename the file\n");
    }
    
    fp = fopen(logger_file_name,"a+");
    if(fp==NULL)
    {
        printf("Error in opening the file");
        return FAILURE;
    }
}

int getFileSize()
{
    /* Here we find the size of the current file */
    
    struct stat sb;         //Struct stat is defined in built in "sys/stat.h" library
    
    fclose(fp);
    
    if (stat(logger_file_name, &sb) == -1) 
    {
        perror("stat");                     
        return 0;                                       
    }
    
    printf("Total file [%s] size: %lu bytes\n",logger_file_name, sb.st_size);
    
    fp = fopen(logger_file_name,"a+");
    if(fp==NULL)
    {
        printf("Error in opening the file");        // File is open in appending mode If the code goes crash while finding the file it shows a Error 
        return FAILURE;
    }

    // return sb.st_size;                      // Return the size of a file
}



