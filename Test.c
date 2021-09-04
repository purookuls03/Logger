#include<stdio.h>
#include "logger.h"

int main()
{
    init("C:\\Users\\Dell\\Documents\\GitHub\\DBInserter\\Config\\Logger.ini");
    while(1)
    {
        logger(ALWAYS,__FILE__, __LINE__,"Staring the program");
        logger(DEBUG,__FILE__, __LINE__,"The value is %d %c %f %f %d", 3,'a', 4.22,3.7891,55);
        logger(ERROR,__FILE__, __LINE__,"End the program");
        logger(WARN,__FILE__, __LINE__,"End the program");
    }
    
    deinit();

    return 0;
}