Project_name : Logger

1. This logger library is impliemented in c can be used for logging in c projects. Log file is a file that records either events that occur in an operating system or other software runs

2. As of Now, This project is only compitable with Windows OS, mingw and visual studio code enviourment. 

3. This logger library wil expose three functions to the user 
	a. init("C:\\Users\\Dell\\Documents\\GitHub\\DBInserter\\Config\\Logger.ini");  
   		This api should be called starting of the project, so that logger can be initilized properly before using logger api given below. This init api will require logger configuration file path. This configuration file should have
		below parameters. Sample configuration file(Logger.ini) having parameters description is given under Conf folder.
		LOG_Path=D:\\log
		FileName=DBInserter
		extension=log
		rotationsize=50000
		buffersize=500
		loglevel=2
       
        b. logger(DEBUG,__FILE__, __LINE__,"The value is %d %c %f %f %d", 3,'a', 4.22,3.7891,55);
     		This api is used to print the log as per the configuration given under point a.
    
    	c. deinit();
		This api should be called when programm is going to terminate.

4. 	It is a simple example to demonstrate a possible  logger library. 
	The Test file demonstrates how to call the library file after it's compiled
	gcc -c logger.c -o logger.o
	gcc Test.c -L. -llogger
	

5. Test apllication to use this logger library is given in test.c .
	gcc Test.c -L. -llogger -o test.exe

	


.
