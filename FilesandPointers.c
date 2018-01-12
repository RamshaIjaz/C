#include <stdio.h>
#include <stdlib.h>

void FindRecord(char *, char *, char[]);                            //declaring each function prototypes for each methods and Helper methods
void Replace(char *, char *, char[]);
void SaveRecord(char *, char *, char[]);
int lengthofString(char *);
int findLinenumber(char *, char *);

int main()
{
    char *o_name, *n_name, currrecord[1000];                    //intialize original name, replacement name, record array
    char *filename = "C:\\Users\\ramsha\\Documents\\COMP 206\\phonebook.csv";                               //and file name

    o_name = (char *)malloc(100);                               //allocate 100 bytes for name
    n_name = (char *)malloc(100);                               //allocate 100 bytes for newname

    printf("Enter a name: ");                                   //prompt for original name
    scanf("%[^\n]%*c",o_name);

    printf("Enter a newname: ");                              //prompt for new name
    scanf("%[^\n]%*c",n_name);

    FindRecord(filename, o_name, currrecord);                //calls FindRecord to search the original name in file
    if (currrecord[0] == 0)                                   //exits from main when match not found
    {
        printf("Name not found");
        return 0;
    }

    Replace(o_name, n_name, currrecord);                    //calls Replace to replace the original name with the new name

    SaveRecord(filename, o_name, currrecord);                 //calls SaveRecord to save the file with the new name

    free(o_name);                                                 //empty the name heap
    free(n_name);                                                  //empty the new name heap
    return 0;
}

void FindRecord(char *filename, char *name, char record[])
{
    FILE *fptr = fopen(filename, "rt");
    int i;

    while(!feof(fptr))                                          //loops until end of file
    {
        fscanf(fptr,"%[^\n]%*c", record);                       //reads the file line by line
        for(i=0; i<1000; i++)
        {
            if(record[i] == ',' && name [i] == 0)               //return to main when a match is found and the file is closed
            {
                fclose(fptr);
                return;
            }
            else if (record[i] != name[i])                      //while loop continues if a match is not found ==> goes to next line to check again
            {
                break;
            }
        }

    }
    fclose(fptr);
    record [0]=0;                                               //deletes the string from record when match was not found
}
void Replace(char *name, char *newname, char record[])
{
    int i, lengthNewname, lengthName, lengthofrecord, diff;
    lengthName = lengthofString(name);
    lengthNewname = lengthofString(newname);
    lengthofrecord = lengthofString(record);
    diff = lengthName - lengthNewname;                          //calculate the difference between the lenght of new name and original name
    if(diff >0 )                                                //if orignal name > new name then shift the characters to left
    {
        for(i=lengthName; *(record+i) != 0; i++)
        {
            *(record+i-diff)= *(record + i);
        }
        *(record + i-diff)=0;                                   //sets the last charcter to null
        for(i=0; i<lengthNewname; i++)                          //replaces the orignal name with the new name
        {
            *(record+i)=*(newname+i);

        }
    }
    else if(diff<0)                                             //if newname > orignal name then shift to right
    {
        for(i=lengthofrecord+1; i>(lengthName-1); i--)
        {
            *(record +i+abs(diff))= *(record +i);

        }

        for(i=0; i<lengthNewname; i++)                          //replaces the orignal name with the new name
        {
            *(record+i)=*(newname+i);
        }
    }
    else                                                        //overwrites the orignal name with the new name as both names are same
    {
        for(i=0; i<lengthNewname; i++)
        {
            *(record+i)=*(newname+i);
        }

    }

}


void SaveRecord(char *filename, char *name, char record[])
{
    int lineNumber = findLinenumber(filename, name);            //calls findLinenumber to find the line where match was found in file
    FILE *fptr = fopen(filename, "rt");
    int i, j, count =0;
    char records [100][100];
    while(!feof(fptr))
    {

        fscanf(fptr,"%[^\n]%*c", records[count]);               //reads the records into a 2D array

        count++;                                                //keeps track of number of records
    }
    for(i=0; *(record+i) != 0; i++)                             //replaces existing records with the new record
    {
        records[lineNumber][i]=*(record+i);

    }
    records[lineNumber][i] = 0;                                 //puts null character in the last slot of the 2D aray

    fclose(fptr);
    fptr = fopen(filename, "wt");                               //writes the content from the 2D array into the old file
    for(i=0; i<count; i++)                                      //loops for the next row in 2D array
    {
        for(j =0; records[i][j]!= 0; j++)                       //loops for the next character until the null character in the 2D array
        {
            fputc(records[i][j], fptr);
        }

        if(i<count-1)
        {
            fprintf(fptr, "\n");                                    //puts new line character at the end of each line
        }
    }


    fclose(fptr);
}










//Helper method calculates the length of any string until the null character
int lengthofString(char *str)
{
    int j=0;
    while(str[j]!= 0)
    {

        j++;
    }
    return j;
}


//Helper method finds line number where match is found in file
int findLinenumber(char *filename, char *name)
{
    FILE *fptr = fopen(filename, "rt");
    int i,lineNumber=0;
    char oldrecord [1000];
    while(!feof(fptr))
    {
        fscanf(fptr,"%[^\n]%*c", oldrecord);
        for(i=0; i<1000; i++)
        {
            if( oldrecord[i] == ',' && name [i] == 0)
            {
                fclose(fptr);
                return lineNumber;

            }
            else if (oldrecord[i] != name[i])
            {
                lineNumber++;
                break;
            }
        }

    }

    fclose(fptr);
    return lineNumber;
}
