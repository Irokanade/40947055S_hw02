//
//  bmpDistortion.c
//  hw0303
//
//  Created by michaelleong on 23/04/2021.
//

#include "bmpDistortion.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

int bmpDistortion() {
    char fileName1[129] = {0};
    char fileName2[129] = {0};
    FILE *pFile = NULL;
    FILE *pFile2 = NULL;
    int angle = 0;
    
    //input file name
    printf("Please input BMP file: ");
    fgets(fileName1, 129, stdin);
    
    //deal with the new line character
    if(fileName1[strlen(fileName1) - 1] == '\n') {
        fileName1[strlen(fileName1) - 1] = 0;
    } else {
        //clear the buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    
    //open the file
    if((pFile = fopen(fileName1, "r")) == NULL) {
        printf("Error opening file >:[\n");
        return -1;
    }
    
    //input the output file name
    printf("Please input the output BMP file name: ");
    fgets(fileName2, 129, stdin);
    
    //deal with the new line character
    if(fileName2[strlen(fileName2) - 1] == '\n') {
        fileName2[strlen(fileName2) - 1] = 0;
    } else {
        //clear the buffer again
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    
    //open the output file
    if((pFile2 = fopen(fileName2, "w")) == NULL) {
        printf("Error opening file >:[\n");
        return -1;
    }
    
    printf("Angle (0-90): ");
    scanf("%d", &angle);
    
    if(angle > 90 || angle < 0) {
        printf("angle must be within 0~90 >:[\n");
        return -1;
    }
    
    sBmpHeader header;
    sBmpHeader modifiedHeader;
    
    //deal with tan(0)
    if(angle == 0){
        uint8_t* rowBuffArr = NULL;
        int32_t newRowBuff = 0;
            
        fread(&header,sizeof(sBmpHeader),1,pFile);
        header.width *= 2;
        header.height = 1;
        header.bitmap_size = (int)modifiedHeader.height*modifiedHeader.width*(modifiedHeader.bpp/8);
        header.size = header.bitmap_size+54;
        fwrite(&header,sizeof(sBmpHeader),1,pFile2);

        newRowBuff = header.width * 3 + header.width % 4;
        rowBuffArr = (uint8_t*)malloc(sizeof(uint8_t) * newRowBuff);
        for(int32_t i = 0;i < newRowBuff;i++) rowBuffArr[i] = 0; //all black

        fwrite(rowBuffArr,1,newRowBuff,pFile2);
        return 1;
    }

    fread(&header, sizeof(header), 1, pFile);
    
    //update new header data
    modifiedHeader = header;
    
    //deal with tan(90)
    if(angle == 90) {
        //just copy lah :)
    } else {
        modifiedHeader.width += (int)(header.height/tan(angle*M_PI/180));
        modifiedHeader.bitmap_size = (int)modifiedHeader.height*modifiedHeader.width*(modifiedHeader.bpp/8);
        modifiedHeader.size = modifiedHeader.bitmap_size+54;
    }
    
    fwrite(&modifiedHeader, sizeof(modifiedHeader), 1, pFile2);
    
    int oriRowByteLen = header.width*3 + header.width%4; //add mod 4 for padding
    int newRowByteLen = modifiedHeader.width*3 + modifiedHeader.width%4;
    uint8_t colBuffOri[oriRowByteLen];
    uint8_t colBuffModified[newRowByteLen];
    
    int height = 0;
    while(!feof(pFile)) {
        fread(colBuffOri, oriRowByteLen, 1, pFile);
        int offset = 0;
        if(angle == 90) {
            offset = 0;
        } else {
            offset = (int)(height/(tan(angle*M_PI/180))*(modifiedHeader.bpp/8));
        }
    
        //set everything to 255(white)
        for(size_t j = 0; j < newRowByteLen; j++) {
            colBuffModified[j] = 255;
        }
        
        for(size_t j = 0; j < oriRowByteLen; j++) {
            colBuffModified[j+offset] = colBuffOri[j];
        }
        
        fwrite(colBuffModified, newRowByteLen, 1, pFile2);
        height++;
    }
    
    //close the files
    fclose(pFile);
    fclose(pFile2);
    
    return 1;
}
