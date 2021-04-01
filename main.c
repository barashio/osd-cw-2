#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PY_SSIZE_T_CLEAN
#include </usr/include/python3.9/Python.h>

// variables 
time_t parentStart, parentStop, childStart, childStop;
int pid, parentpid, childpid, file, filecp, childInterrupted = 0, childLoop = 0, fd[2], n, randomnumber;
char numbers[4097], pythonbuffer[4097]; 
struct stat fileStat;

void sighand(int signo) { //signal handler function
    switch(signo){
        case SIGTSTP: // handle Crtl + Z to generate numbers between 10 to 50
            randomnumber = rand()%((50+1)-10) + 10; printf("%d,", randomnumber); break;
        case SIGINT: // handle Ctrl + C
            exit(0); break; 
        case SIGCHLD: // look for child to die
            childInterrupted = 1; break;
        default: // unexpected condition
            printf("Unable to handle unknown signal\n"); break;
    }
}

void statusfile(time_t parentStart, time_t childStart, time_t childStop, time_t parentStop, int parentpid, int childpid) {
    file = open("log.txt", O_WRONLY | O_CREAT, 0777); // open file with write only permission or create with read, write & execute permission
    if (file == -1) { printf("Error in file management!\n"); } // error handler for when opening file
    filecp = dup2(file, 1); // remapping of pipe to file so outputs are typed in file
    printf("Parent process PID (%d)started at (%u)\n", parentpid, parentStart); // start stop information in unix format
    printf("Child process PID (%d) started at (%u)\n", childpid, childStart);
    printf("Child process PID (%d) stopped at (%u)\n", childpid, childStop);
    printf("Parent process PID (%d) stopped at (%u)\n", parentpid, parentStop); 
    fstat(file, &fileStat);
    printf("File inode (%d) with size (%d) is created at (%d)\n", fileStat.st_ino, fileStat.st_size, fileStat.st_ctim); // information about inode, size and creation time
    close(file);
}

void py(char numbers[]) {
    Py_Initialize();
    snprintf(pythonbuffer, sizeof(pythonbuffer), "\n \
                                                \nimport mysql.connector \
                                                \nnumbersString = '%s'  \
                                                \nnumbersArray = numbersString.split(',') # seperate string into array \
                                                \ndel numbersArray[-1] # delete last empty value in array from comma seperation \
                                                \ntry: \
                                                \n\tconnection = mysql.connector.connect(user='root', password='w1646608', host='192.168.0.2', port='49162', database='w1646608') # connect to my docker MySQL DB \
                                                \n\tcursor = connection.cursor() # initialise cursor \
                                                \n\tcursor.execute('DELETE FROM numbers;') # empty DB \
                                                \n\tfor number in (numbersArray): \
                                                \n\t\tsqlcommand = sqlcommand = 'INSERT INTO numbers(number) VALUES ({})'.format(number) #pass variable into command in loop\
                                                \n\t\tcursor.execute(sqlcommand) # insert into DB \
                                                \n\tconnection.commit() # commit changes to DB \
                                                \n\tcursor.execute('SHOW TABLES') # get table name \
                                                \n\tfor (table_name,) in cursor: print(table_name) # print table name \
                                                \n\tcursor.execute('SELECT number FROM numbers') # get table data \
                                                \n\tfor (columnt_value,) in cursor: print(columnt_value) # show table data \
                                                \n\tconnection.close() # end connection \
                                                \nexcept mysql.connector.Error as err: print('Something went wrong: {}'.format(err)) # catch any error \
                                                \n", numbers); // pass number variable to string. once inside make it a whole string for the command below
    PyRun_SimpleString(pythonbuffer); // this command executes the whole command as a string. we needed previous one because we have variables in a string
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
}

int main (int argc, char* argv[]) {
    parentStart = time(NULL); // reg parent start time
    signal(SIGINT, SIG_IGN); // ignore signals
    signal(SIGTSTP, SIG_IGN); 
    signal(SIGCHLD, &sighand); 
    pipe(fd); // init pipe file descriptor
    pid = fork(); // create child
    childStart = time(NULL); // reg child start time
    if (pid == -1) { // child validation
        printf("\nError creating process!\n"); 
        return 1;
    } else if (pid == 0) { // child process
        signal(SIGTSTP, &sighand); // signal handlers
        signal(SIGINT, &sighand); 
        close(fd[0]); // close read enf of pipe
        dup2(fd[1], 1); // remap STDOUT to writing end of pipe
        while(childLoop == 0) { } // loop until SIGINT
        exit(0); // exit with success
    } else if (pid != 0) { // parent process
        while (childInterrupted == 0) {  } // wait for child
        childStop = time(NULL); // reg child stop time
        close(fd[1]); // close write end of pipe
        dup2(fd[0], 0); // remap STDIN
        scanf("%s", numbers); // scan the numbers from child
        py(numbers); // python function
        parentStop = time(NULL); // reg parent stop time
        parentpid = getpid(); // get parent PID
        childpid = pid; // get child PID
        statusfile(parentStart, childStart, childStop, parentStop, parentpid, childpid); // status file function
        close(fd[1]); // close write end of pipe
    } else { }
    return 0;
}
