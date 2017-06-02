#include"Helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
//
// http://stackoverflow.com/questions/2029103/correct-way-to-read-a-text-file-into-a-buffer-in-c
//
char* textFileToCString(const char* aFileName)
{
    char *source = NULL;
    FILE *fp = fopen(aFileName, "r");
    long bufsize;
    size_t newLen;

    if (fp != NULL) 
    {
        /* Go to the end of the file. */
        if (fseek(fp, 0L, SEEK_END) == 0) 
        {
            /* Get the size of the file. */
            bufsize = ftell(fp);
            if (bufsize == -1){ /* Error */ }

            /* Allocate our buffer to that size. */
            source = malloc(sizeof(char) * (bufsize + 1)); //original is bufsize+1

            /* Go back to the start of the file. */
            if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

            /* Read the entire file into memory. */
            newLen = fread(source, sizeof(char), bufsize, fp);

            if (newLen == 0) 
            {
                fputs("Error reading file", stderr);

            } 
            else 
            {
                source[newLen++] = '\0'; /* Please god always null terminate it. */ //original is newLen++

            }

        }

        fclose(fp);

        return source;

    }

}

char* retrieveXMLElementById(const char* source, const char* id)
{
    char* output = 0;
    {
        uint len = strlen(source);
        uint i = 0;
        uint withinElement = 0;

        for(i = 0; i < len; i++)
        {
            //Determine we're inside an element
            if ((source)[i] == '<')
                withinElement = 1;
            else if ((source)[i] == '>')
                withinElement = 0;

            //Check for id attribute inside element
            if (withinElement == 1)
            {
                if ((source)[i] == 'i')
                    if ((source)[1+i] == 'd')
                    {
                        //id attribute detected
                        uint idValueStartIndex = 0;
                        uint idValueEndIndex   = 0;
                        uint count = 0; //TODO: only used for debug text

                        i +=1;

                        //printf("\nId found: %d\n",++count);

                        //Calculate id value start & end indicies
                        {
                            int withinValue = 0;
                            int j = 0;

                            for(j=i; j < len; j++)
                            {
                                //Determine we're inside the id's value
                                if ((source)[j] == '"' && withinValue == 0)
                                {
                                    withinValue = 1;
                                    idValueStartIndex = 1+j;

                                }
                                else if ((source)[j] == '"' && withinValue == 1)
                                {
                                    withinValue = 0;
                                    idValueEndIndex = j;
                                    break;

                                }
                                
                            }

                        }

                        //Retrieve id value & compare to id arg
                        {
                            char* idValueBuff = (char*)malloc(sizeof(char) * ((idValueEndIndex-idValueStartIndex) + 1));
                            {
                                uint j = 0, k = 0;
                                
                                for (j = idValueStartIndex; j < idValueEndIndex; j++, k++)
                                {
                                    *(idValueBuff+k) = *(source+j);
                                    
                                }

                                *(idValueBuff+0+k) = '\0';

                            }

                            if (strcmp(idValueBuff,id) == 0)
                            {
                                //Specified id was found, calculate content buffer size & copy data
                                char* contentBuff       = 0;
                                uint  contentStartIndex = 0;
                                uint  contentEndIndex   = 0;
                                
                                //Calc size
                                {
                                    for(; i < len; i++)
                                    {
                                        if ((source)[i] == '>')
                                            contentStartIndex = 1+i;
                                        else if ((source)[i] == '<')
                                            if ((source)[1+i] == '/')
                                            {
                                                contentEndIndex = i-1;
                                                break;

                                            }
                                    
                                    }

                                    contentBuff = (char*)malloc(sizeof(char) * ((contentEndIndex-contentStartIndex) + 0));

                                }

                                //Copy data to buffer
                                {
                                    uint j = 0, k = 0;
                                
                                    for (j = contentStartIndex; j < contentEndIndex; j++, k++)
                                    {
                                        *(contentBuff+k) = *(source+j);
                                        
                                    }

                                    *(contentBuff+0+k) = '\0'; //TODO: whats up???

                                }

                                //printf("\n\nFOUND BEGIN CONTENT DUMP************\n\n");
                                //printf(contentBuff);

                                output = contentBuff;
                                return output;

                            }

                        }
                
                    }

            }

        }



    }

    return output;

}

char* cStringAdd2(const char* firstString, const char* secondString)
{
    char* output = 0;
    {
        uint fslen = strlen(firstString);
        uint sslen = strlen(secondString);

        output = (char*)malloc(sizeof(char) * (fslen + sslen + 1));
        {
            uint i   = 0;
            uint len = fslen + sslen;

            for(; i < len; i++)
            {
                if (i < fslen)
                    *(output+i) = *(firstString+i);
                else
                    *(output+i) = *(secondString+(i-fslen));
        
            }

            *(output+i) = '\0'; //TODO: could be a problem

        }

    }

    return output;

}

char* cStringAdd3(const char* firstString, const char* secondString, const char* thirdString)
{
    char* output = 0;
    {
        uint fslen = strlen(firstString);
        uint sslen = strlen(secondString);
        uint tslen = strlen(thirdString);

        output = (char*)malloc(sizeof(char) * (fslen + sslen + tslen + 1));
        {
            uint i   = 0;
            uint len = fslen + sslen + tslen;

            for(; i < len; i++)
            {
                if (i < fslen)
                    *(output+i) = *(firstString+i);
                else if (i < fslen + sslen)
                    *(output+i) = *(secondString+(i-fslen));
                else
                    *(output+i) = *(thirdString+(i-fslen-sslen));
        
            }

            *(output+i) = '\0';

        }



    }

    return output;

}