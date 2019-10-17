
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tCounter = 0;

int typeOf(char c){

    // space or simicolon
    if (c == ' ' || c == ';')
        return 0;

    // letters
    if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90))
        return 1;

    // numbers
    if (c>=48 && c<=57)
        return 2; 

    return 3;
}

char **fileToLines(FILE *file){
    // takes in a file pointer and returns poiner to a 2d array of commands
    
    // create an array of strings
    char **lines = calloc(1024, sizeof(char**));
    // add a string into the first slot
    lines[0] = calloc(1024, sizeof(char));    
    // character to hold input
    char c;
    // ints to keep track of character# and string#
    int characterCounter=0, stringCounter=0;

    
    // cycle through file til the end
    while((c = fgetc(file)) != EOF){

        //printf("%c", c);

        if (c==';'){
            // at the end of a line (denoted with ;)        
            stringCounter++;
            lines[stringCounter] = calloc(1024, sizeof(char));
            characterCounter=0;
        }
        else{
        // add to the array 
        lines[stringCounter][characterCounter++] = c;
        }

    }

    return lines;
}

char ** lineToKeys(char * line){
            //printf("atlineToKeys with %s\n", line);

    // takes in a line and return an arry of keys
    char ** keys;
    keys = calloc(255, sizeof(char));
    keys[0] = calloc(255, sizeof(char));
    char c;
    int i=0, len = strlen(line), cCounter=0, kCounter=0, lastType=-1;
    //printf("len = %d", len);
    // for the first character
    if (lastType == -1)
        lastType=typeOf(c);

    for(i=0; i<=len; i++){
        c = line[i];

        if (c != ' ' && c !='\n'){
            if (typeOf(c) != lastType){
                //printf("key: %s\n",keys[kCounter]);
                cCounter=0;
                kCounter++;
                keys[kCounter] = calloc(1024,sizeof(char*));
                //keys[kCounter][cCounter++] = c;
            }
            else{
                //keys[kCounter][cCounter++] = c;
            }
            keys[kCounter][cCounter++] = c;
        }

        lastType = typeOf(c);

    }


    return keys;

}

char *** linesToKeys(char **lines){
    char ***keys;
    // the first layer holds the lines
    keys = calloc(1024, sizeof(char**));
    // the second layer holds the array of keys
    // the third layer holds the array of characters

    int i=0;
    while(lines[i] != NULL) {
        keys[i] = calloc(1024, sizeof(char*));
        keys[i] = lineToKeys(lines[i]);
        i++;
    }
    
    return keys;
}

void printPost(){
    printf(
    "\n  ret i32 0\n"
    "}"
    );

}

void printPre(){
printf(
        "target triple = \"x86_64-pc-linux-gnu\"\n"
        "\n"        
        "declare i32 @printf(i8*, ...) #1\n"
        "@.str = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\", align 1\n"
        "\n"
        "define void @print_integer(i32) #0 {\n"
        "  %%2 = alloca i32, align 4\n"
        "  store i32 %%0, i32* %%2, align 4\n"
        "  %%3 = load i32, i32* %%2, align 4\n"
        "  %%4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %%3)\n"
        "  ret void\n"
        "}\n"
        "\n"
        "define i32 @main() #0 {\n"
    );

}
//===================================================================================
int processKeyLine(char ** keyLine){



    // cycle through and if there is a + print whats efore and after
    // if there is a print, print the current number &

    if (keyLine == NULL)
        return 0;
    int mathFlag = 0;

    int i=0;
    while (keyLine[i] != 0){
        //printf("%s\n", keyLine[i]);
        if (strcmp(keyLine[i],"+") == 0){
                printf("%%t%d = add nsw i32 %s, %s\n",++tCounter, keyLine[i-1], keyLine[i+1]);
                mathFlag = 1;
        }

        else if (strcmp(keyLine[i],"-") == 0){
            printf("%%t%d = sub nsw i32 %s, %s\n",++tCounter, keyLine[i-1], keyLine[i+1]);
                mathFlag = 1;
        }
        else if (strcmp(keyLine[i],"*") == 0){
            printf("%%t%d = mul nsw i32 %s, %s\n",++tCounter, keyLine[i-1], keyLine[i+1]);
                mathFlag = 1;
        }
        else if (strcmp(keyLine[i],"/") == 0){
            printf("%%t%d = sdiv nsw i32 %s, %s\n",++tCounter, keyLine[i-1], keyLine[i+1]);
                mathFlag = 1;
        }
        else if (strcmp(keyLine[i],"%") == 0){
            printf("%%t%d = srem nsw i32 %s, %s\n",++tCounter, keyLine[i-1], keyLine[i+1]);
                mathFlag = 1;
        }

        i++;
    }

//printf("noMathFlag = %d\n\n", mathFlag);

    i=0;
    while (keyLine[i] != 0){
        if (strcmp(keyLine[i],"print") == 0)
            if (mathFlag == 1)            
                printf("call void @print_integer(i32 %%t%d)\n", tCounter);
            else
                printf("call void @print_integer(i32 %s)\n", keyLine[i+1]);

        i++;
    }

    // if there is an operator put the current t value and the operator with the 2 numbers
    // also set a flag
    // if the flag is off and their is a print, print the print statement with the next key


    return 1;
}


int main(int argv, char **argc){

    //________________________________________
    // check for correct parameter count
    if (argv != 2){
        printf("Usage: ./a.out inputFile.txt\n");
        return 0;
    }

    //________________________________________
    // open the file and check if it was successful
    FILE *file;
    if ((file = fopen(argc[1], "r")) == NULL){
        printf("error: file not found");    
        return 0;
    }

    //________________________________________
    // call a function that puts each line in its own array
    char ** lines = fileToLines(file);

    //________________________________________
    // checking the lines 2d array
    //int i=0;
    //while(lines[i] != NULL)   {
    //    printf("%s",lines[i]);
    //    i++;    
    //}

    // split lines up into an array of arrays containing the keys in the lines
    char ***linesOfKeys = linesToKeys(lines);

    //________________________________________
    // print the llvm ir

    printPre();
    int  i=0;
    while(processKeyLine(linesOfKeys[i++]) != 0){};
    printPost();



    return 0;
}










