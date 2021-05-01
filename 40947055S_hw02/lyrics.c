//
//  lyrics.c
//  hw0301
//
//  Created by michaelleong on 15/04/2021.
//

#include "lyrics.h"
#include <string.h>
#include <time.h>

#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define REDWHT "\033[1;97;101m"
#define BLUWHT "\033[1;97;104m"
//Reset
#define reset "\e[0m"

//search singer name in array
//return index if exist
//return -1 if not found
int searchName(char *name, singer singerList[], size_t size) {
    for(size_t i = 0; i < size; i++) {
        if(strcmp(name, singerList[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

//returns -1 when there's a problem
int lyrics() {
    FILE *pFile = NULL;
    char filename[129];
    
    //initialize colours
    //max of 8
    singer singerList[9];
    singerList[0].colour = RED;
    singerList[1].colour = GRN;
    singerList[2].colour = YEL;
    singerList[3].colour = BLU;
    singerList[4].colour = MAG;
    singerList[5].colour = CYN;
    singerList[6].colour = REDWHT;
    singerList[7].colour = BLUWHT;
    
    
    printf("Open a LRC file: ");
    fgets(filename, 129, stdin);
    
    //clean up the '\n' character
    if(filename[strlen(filename) - 1] == '\n') {
        filename[strlen(filename) - 1] = 0;
    } else {
        //clear the buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    
    //printf("filename: %s\n", filename);
    
    //check file extension
    char *temp;
    temp = &filename[strlen(filename) - 4];
    //printf("temp: %s\n", temp);
    if(strcmp(temp, ".lrc")) {
        printf("file does not have the extension .lrc\n");
        return -1;
    }
    
    //open the file
    if((pFile = fopen(filename, "r")) == NULL) {
        printf("file does not exits\n");
        return -1;
    }
    
    //singer list index
    int singerListIndex = 0;
    
    time_t start, end;
    //start the time
    time(&start);
    
    while(!feof(pFile)) {
        timeStamp stamp;
        char strTemp[129] = {0};
        
        int valid = fscanf(pFile, "[%ld:%ld.%ld]", &stamp.minutes, &stamp.seconds, &stamp.milliseconds);
        if(valid == 3) {
            fgets(stamp.lyricsTxt, 129, pFile);
            //printf("valid: %d\n", valid);
                    do time(&end); while(difftime(end, start) <= stamp.minutes*60 + stamp.seconds + stamp.milliseconds*0.01);
            //printf("[%ld:%ld.%ld]%s", stamp.minutes, stamp.seconds, stamp.milliseconds, stamp.lyricsTxt);
            
            if(singerListIndex == 0) {
                //no singers then print cyn
                printf("%s", CYN);
            }
            printf("%s", stamp.lyricsTxt);
            
        } else {
            fgets(strTemp, 129, pFile);
            //printf("strTemp: %s\n", strTemp);
            
            int index = 0;
            if((strchr(strTemp, ']') == NULL) && (strchr(strTemp, ':') != NULL)) {
                
                index = searchName(strTemp, singerList, sizeof(singerList)/sizeof(singerList[0]));
                if(index == -1) {
                    strcpy(singerList[singerListIndex].name, strTemp);
                    index = singerListIndex;
                    //printf("index: %d\n", index);
                    singerListIndex++;
            }
                
                printf("%s", singerList[index].colour);
                
                
                //printf("name: %s", strTemp);
            } else {
                //label
                //printf("label: %s", strTemp);
            }
        }
    }
    
    printf("%s", reset);
    //printf("%s", "reset\n");
    fclose(pFile);
    
    return 0;
}
