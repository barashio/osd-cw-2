#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>

#define PY_SSIZE_T_CLEAN
#include </usr/include/python3.9/Python.h>

// VARIABLES
time_t parentStart, parentStop, childStart, childStop;
int childInterrupted = 0, childLoop = 0, fd[2], n, randomnumber;
char buffer[4097]; 

void sighand(int signo) {
    switch(signo){
        case SIGTSTP:
            randomnumber = rand()%((50+1)-10) + 10; printf("%d,", randomnumber); break;
        case SIGINT:
            exit(0); break;
        case SIGCHLD:
            childInterrupted = 1; break;
        default:
            printf("Unable to handle unknown signal\n"); break;
    }
}

void log(time_t parentStart, time_t parentStop, time_t childStart, time_t childStop) {
    printf("");
}

void py() {
    Py_Initialize();
    PyRun_SimpleString("\n \
                        \nimport mysql.connector \
                        \nnumbersString = '13,13,13,' \
                        \nnumbersArray = numbersString.split(',') # seperate string into array \
                        \ndel numbersArray[-1] # delete last empty value in array from comma seperation \
                        \ntry: \
                        \n\tconnection = mysql.connector.connect(user='root', password='w1646608', host='192.168.0.2', port='49162', database='w1646608') # connect to my docker MySQL DB \
                        \n\tcursor = connection.cursor() # initialise cursor \
                        \n\tcursor.execute('DELETE FROM numbers;') # empty DB \
                        \n\tfor number in (numbersArray): cursor.execute('INSERT INTO numbers(number) VALUES (%s)' % number) # insert into DB \
                        \n\tconnection.commit() # commit changes to DB \
                        \n\tcursor.execute('SHOW TABLES') # get table name \
                        \n\tfor (table_name,) in cursor: print(table_name) # print table name \
                        \n\tcursor.execute('SELECT number FROM numbers') # get table data \
                        \n\tfor (columnt_value,) in cursor: print(columnt_value) # show table data \
                        \n\tconnection.close() # end connection \
                        \nexcept mysql.connector.Error as err: print('Something went wrong: {}'.format(err)) # catch any error \
                        \n");
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
}

int main (int argc, char* argv[]) {
    parentStart = time(NULL); // reg parent start time
    signal(SIGINT, SIG_IGN); signal(SIGTSTP, SIG_IGN); signal(SIGCHLD, &sighand); // ignore signals
    pipe(fd); // init pipe file descriptor
    int pid = fork(); // create child
    if (pid == -1) { // child validation
        printf("\nError creating process!\n"); return 1;
    } else if (pid == 0) {
        childStart = time(NULL); // reg child start time
        signal(SIGTSTP, &sighand); signal(SIGINT, &sighand); // signal handlers
        close(fd[0]); dup2(fd[1], 1); // close read enf of pipe
        while(childLoop == 0) { } // loop until SIGINT
        childStop = time(NULL); // reg child stop time
        exit(0); // exit with success
    } else if (pid != 0) {
        while (childInterrupted == 0) {  } // wait for child
        close(fd[1]); // close write end of pipe
        if ((n = read(fd[0], buffer, 4096) ) >= 0) { buffer[n] = 0; printf("\nRead %d bytes from the pipe: %s\n", n, buffer); } // read from pipe
        parentStop = time(NULL); // reg parent stop time

        // random  generated numbers by child from 10 to 50 is to be written to a database via a python driver/connector
        py(); // python function
    } else { }
    return 0;
}
