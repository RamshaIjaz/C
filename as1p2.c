/*
----------------- COMP 310/ECSE 427 Winter 2018 -----------------
I declare that the awesomeness below is a genuine piece of work
and falls under the McGill code of conduct, to the best of my knowledge.
-----------------------------------------------------------------
*/

//Please enter your name and McGill ID below
//Name: RAMSHA IJAZ
//McGill ID: 260665762

//all the header files you would require
#include <stdio.h>    //for standard IO
#include <unistd.h>   //for execvp/dup/dup2
#include <string.h>   //for string manipulation
#include <stdlib.h>   //for fork
#include <ctype.h>    //for character type check (isnum,isalpha,isupper etc)
#include <sys/wait.h> //for waitpid
#include <fcntl.h>    //open function to open a file. type "man 2 open" in terminal
#include <time.h>     //to handle time

//pointer to Linked list head
struct node *head_job = NULL;

//global variable used to store process id of process
//that has been just created
//can be used while adding a job to linked list
pid_t process_id;

//flag variable to check if redirection of output is required
int isred = 0;

//global job counter
int job_counter = 1;

//structure of a single node
//donot modify this structure
struct node
{
    int number;        //the job number
    int pid;           //the process id of the process
    char *cmd;         //string to store the command name
    time_t spawn;      //time to store the time it was spawned
    struct node *next; //when another process is called you add to the end of the linked list
};

// Add a job to the linked list
void addToJobList(char *args[])
{

    //allocate memory for the new job
    struct node *job = malloc(sizeof(struct node));
    struct node *current_job = NULL;

    //If the job list is empty, create a new head
    if (head_job == NULL)
    {
        //init the job number with global job counter
        job->number = job_counter;

        //set its pid from the global variable process_id
        job->pid = process_id;

        //cmd can be set to arg[0]
        job->cmd = args[0];

        //set the job->next to point to NULL.
        job->next = NULL;

        //set the job->spawn using time function
        job->spawn = (unsigned int)time(NULL);

        //set head_job to be the job
        head_job = job;

        //set current_job to be head_job
        current_job = head_job;
    }

    //Otherwise create a new job node and link the current node to it
    else
    {
        //point current_job to head_job
        current_job = head_job;
        job_counter++;

        //traverse the linked list to reach the last job
        while (current_job->next != NULL)
        {
            current_job = current_job->next;
        }

        //init all values of the job like above num,pid,cmd.spawn
        job->number = job_counter;
        job->pid = process_id;
        job->cmd = args[0];
        job->spawn = (unsigned int)time(NULL);
        job->next = NULL;

        //make next of current_job point to job
        current_job->next = job;
    }
}

//Function to refresh job list
//Run through jobs in linked list and check
//if they are done executing then remove it
void refreshJobList()
{
    //pointer require to perform operation
    //on linked list
    struct node *current_job;
    struct node *prev_job;

    //variable to store returned pid
    pid_t ret_pid;

    //perform init for pointers
    current_job = head_job;
    prev_job = head_job;

    //traverse through the linked list
    while (current_job != NULL)
    {

        //use waitpid to init ret_pid variable
        ret_pid = waitpid(current_job->pid, NULL, WNOHANG);
        //one of the below needs node removal from linked list

        if (ret_pid == -1)
        {

            if (prev_job != current_job)
            {
                prev_job = current_job;
            }
            current_job = current_job->next;
        }

        else if (ret_pid == 0) //process status not changed
        {

            if (prev_job != current_job)
            {
                prev_job = current_job;
            }
            current_job = current_job->next;
        }

        else //requires removal
        {

            //if first / last job needs removal
            if (current_job == head_job)
            {
                head_job = head_job->next;
                current_job = head_job;
                prev_job = head_job;
            }

            else
            {
                current_job = current_job->next;
                prev_job->next = current_job;
            }
        }
    }
    return;
}

//Function that list all the jobs
void listAllJobs()
{
    struct node *current_job;

    //refresh the linked list
    refreshJobList();

    //init current_job with head_job
    current_job = head_job;

    //heading row print only once.
    printf("\nID\tPID\tCmd\tstatus\tspawn-time\n");

    //traverse the linked list and print using the following statement for each job
    while (current_job != NULL)
    {
        printf("%d\t%d\t%s\tRUNNING\t%s\n", current_job->number, current_job->pid, current_job->cmd, ctime(&(current_job->spawn)));
        current_job = current_job->next;
    }

    return;
}

// wait till the linked list is empty
// you would have to look for a place
// where you would call this function.
// donot modify this function
void waitForEmptyLL(int nice, int bg)
{
    if (nice == 1 && bg == 0)
    {
        while (head_job != NULL)
        {
            sleep(1);
            refreshJobList();
        }
    }
    return;
}

//helper method to check for character vs spaces/NL/EOF
int letterCheck(int character)
{
    int istrue = 1;
    if (character == ' ' || character == 10 || character == 9 || character == -1)
    {
        istrue = 0;
    }
    return istrue;
}

//function to perform word count
int wordCount(char *filename, char *flag)
{
    int cnt = 0, v, w;
    int ch, cur_st = 0, prev_st = 0;
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("The file does not exist\n");
        if (!strcmp("-l", flag) || !strcmp("-w", flag))
            ;
        else
        {
            printf("Unrecognized flag\n");
        }
    }

    //if flag is l, count the number of lines in the file, set it in cnt
    else if (!strcmp("-l", flag))
    {

        while (!feof(file))
        {
            ch = fgetc(file);
            if (ch == '\n')
            {
                cnt++;
            }
        }
        fclose(file);
    }

    //if flag is w, count the number of words in the file, set it in cnt
    else if (!strcmp("-w", flag))
    {

        while (!feof(file))
        {

            cur_st = letterCheck(fgetc(file));
            if (cur_st == 1 && prev_st == 0)
            { //if cur_st is char but prev_st was a space/NL ==>new word
                cnt++;
            }
            prev_st = cur_st;
        }
        fclose(file);
    }
    else
    {
        printf("Unrecognized flag\n");
    }
    printf("%d", cnt);
    return cnt;
}
//Helper function

// function to augment waiting times for a process
// donot modify this function
void performAugmentedWait()
{
    int w, rem;
    time_t now;
    srand((unsigned int)(time(&now)));
    w = rand() % 15;
    printf("sleeping for %d\n", w);
    rem = sleep(w);
    return;
}

//simulates running process to foreground
//by making the parent process wait for
//a particular process id.
int waitforjob(char *jobnc)
{
    struct node *trv;
    int jobn = atoi(jobnc);
    trv = head_job;

    //traverse through linked list and find the corresponding job
    while (trv != NULL && trv->number != jobn)
    {
        trv = trv->next;
    }
    //if correspoding job is found
    //use its pid to make the parent process wait.
    //waitpid with proper argument needed here
    if (trv != NULL)
    {
        waitpid(trv->pid, NULL, WUNTRACED);
    }

    return 0;
}

// splits whatever the user enters and sets the background/nice flag variable
// and returns the number of tokens processed
// donot modify this function
int getcmd(char *prompt, char *args[], int *background, int *nice)
{
    int length, i = 0;
    char *token, *loc;
    char *line = NULL;
    size_t linecap = 0;
    printf("%s", prompt);
    length = getline(&line, &linecap, stdin);
    if (length <= 0)
    {
        exit(-1);
    }
    // Check if background is specified..
    if ((loc = index(line, '&')) != NULL)
    {
        *background = 1;
        *loc = ' ';
    }
    else
        *background = 0;
    while ((token = strsep(&line, " \t\n")) != NULL)
    {
        for (int j = 0; j < strlen(token); j++)
            if (token[j] <= 32)
                token[j] = '\0';
        if (strlen(token) > 0)
        {
            if (!strcmp("nice", token))
            {
                *nice = 1;
            }
            else
            {
                args[i++] = token;
            }
        }
    }
    return i;
}

// this initialises the args to All null.
// donot modify
void initialize(char *args[])
{
    for (int i = 0; i < 20; i++)
    {
        args[i] = NULL;
    }
    return;
}

int main(void)
{
    //args is a array of charater pointers
    //where each pointer points to a string
    //which may be command , flag or filename
    char *args[20];

    //flag variables for background, status and nice
    //bg set to 1 if the command is to executed in background
    //nice set to 1 if the command is nice
    //status
    int bg, status, nice;

    //variable to store the process id.
    pid_t pid;

    //variable to store the file des
    //helpful in output redirection
    int fd1, fd2;

    //your terminal executes endlessly unless
    //exit command is received
    while (1)
    {
        //init background to zero
        bg = 0;
        //init nice to zero
        nice = 0;
        //init args to null
        initialize(args);
        //get the user input command
        int cnt = getcmd("\n>> ", args, &bg, &nice);
        //keep asking unless the user enters something
        while (!(cnt >= 1))
            cnt = getcmd("\n>> ", args, &bg, &nice);

        waitForEmptyLL(nice, bg);

        //use the if-else ladder to handle built-in commands
        //built in commands don't need redirection
        //also no need to add them to jobs linked list
        //as they always run in foreground

        if (!strcmp("jobs", args[0]))
        {
            //call the listalljobs function
            listAllJobs();
        }
        else if (!strcmp("exit", args[0]))
        {
            //exit the execution of endless while loop
            exit(0);
        }
        else if (!strcmp("fg", args[0]))
        {

            //bring a background process to foreground
            waitforjob(args[1]);
        }
        else if (!strcmp("cd", args[0]))
        {
            int result = 0;

            // if no destination directory given
            // change to home directory
            if (args[1] == NULL)
            {
                char *home = getenv("HOME");
                if (home != NULL)
                {
                    result = chdir(home);
                }
                else
                {
                    printf("No $Home variable declared in the environment\n");
                }
            }

            //if given directory does not exist
            //print directory does not exit
            else
            {
                result = chdir(args[1]); //ASK ABOUT SPACES e.g COMP 310
            }
            if (result == -1)
            {
                printf("cd: %s: No such file or directory\n", args[1]);
            }
            //if everthing is fine
            //change to destination directory
        }
        else if (!strcmp("pwd", args[0]))
        {
            //use getcwd and print the current working directory
            char buff[1024];
            getcwd(buff, 1024);
            printf("%s", buff);
        }
        else if (!strcmp("wc", args[0]))
        {

            //call the word count function
            wordCount(args[2], args[1]);
        }
        else
        {
            waitForEmptyLL(nice, bg);
            //create a child
            pid = fork();

            //to check if it is parent
            if (pid > 0)
            {

                //we are inside parent
                printf("Inside the parent\n");
                if (bg == 0)
                {

                    //FOREGROUND
                    // waitpid with proper argument required
                    waitpid(pid, NULL, WUNTRACED);
                }

                else
                {
                    //BACKGROUND
                    process_id = pid;
                    addToJobList(args);
                }
            }
            else
            {
                // we are inside the child

                //introducing augmented delay
                performAugmentedWait();

                //check if args has ">"
                //if yes set isred to 1

                if (args[1] != NULL && !strcmp(">", args[1]))
                {
                    isred = 1;
                }
                //else set isred to 0
                else
                {
                    isred = 0;
                }

                //if redirection is enabled
                if (isred == 1)
                {

                    fd1 = dup(1);
                    close(1); //close stdout

                    //open file and change output from stdout to that
                    fd2 = open(args[2], O_WRONLY | O_APPEND | O_CREAT, 00700);

                    //set ">" and redirected filename to NULL

                    args[1] = NULL;
                    args[2] = NULL;

                    //run your command
                    execvp(args[0], args);

                    //restore to stdout

                    close(fd2),
                        dup2(fd1, 1);
                    close(fd1);
                }
                else
                {
                    //simply execute the command.

                    execvp(args[0], args);
                }
            }
        }
    }

    return 0;
}