//
//  bmp_alpha.c
//  hw0304
//
//  Created by michaelleong on 21/04/2021.
//

#include "bmp_alpha.h"
#include <string.h>
#include <inttypes.h>

int bmpAlpha() {
    char fileName1[129] = {0};
    char fileName2[129] = {0};
    FILE *pFile = NULL;
    FILE *pFile2 = NULL;
    uint8_t alpha;
    
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
    
    //open the file
    if((pFile2 = fopen(fileName2, "w")) == NULL) {
        printf("Error opening file >:[\n");
        return -1;
    }
    
    printf("Alpha: (0-31): ");
    scanf("%" SCNu8, &alpha);
    
    if(alpha > 31 || alpha < 0) {
        printf("Alpha must be within 0~31 >:[\n");
        return -1;
    }
    
    //copy the header
    sBmpHeader header;
    fread(&header, sizeof(header), 1, pFile);
    
    
    //check bpp is 24
    if(header.bpp != 24) {
        printf("bpp is not 24 >:[\n");
        return -1;
    }
    
    //change alpha
    //add the mask rgb mask
    header.size = 70 + header.width * header.height * 4;
    header.offset = 70;
    header.header_size = 56;
    header.bpp = 32;
    header.compression = 3; //change compression type to accomodate mask
    header.bitmap_size = header.width * header.height * 4;
    uint32_t mask[4] = {0x00FF0000,0x0000FF00,0x000000FF,0xFF000000};  //RGBA
    fwrite(&header,sizeof(sBmpHeader), 1, pFile2);
    fwrite(&mask,sizeof(uint32_t), 4, pFile2);
    
    int32_t originalBytes = 0;
    int32_t modifiedBytes = 0;
    int32_t alphaIndex = 0;

    originalBytes = header.width * 3 + header.width % 4;
    modifiedBytes = header.width * 4;
    uint8_t originalRow[originalBytes];
    uint8_t modifiedRow[modifiedBytes];

    while(!feof(pFile)){
        fread(originalRow,1,originalBytes,pFile);
    
        alphaIndex = 0;
        for(int32_t i = 0;i < header.width * 3;i = i + 3){
            //BGRA
            modifiedRow[alphaIndex] = originalRow[i];
            modifiedRow[alphaIndex + 1] = originalRow[i+1];
            modifiedRow[alphaIndex + 2] = originalRow[i+2];
            modifiedRow[alphaIndex + 3] = alpha;
            alphaIndex += 4;
        }

        fwrite(modifiedRow,1,modifiedBytes,pFile2);
    }
    
    
    fclose(pFile);
    fclose(pFile2);
    
    return 1;
}

void print_bmp_header( sBmpHeader *pHeader )
{
    printf( "ID: %c%c\n", pHeader -> bm[0], pHeader -> bm[1] );
    printf( "Size: %u\n", pHeader -> size );
    printf( "Reserve: %u\n", pHeader -> reserve );
    printf( "Offset: %u\n", pHeader -> offset );
    printf( "Header Size: %u\n", pHeader -> header_size );
    printf( "Width: %u\n", pHeader -> width );
    printf( "Height: %u\n", pHeader -> height );
    printf( "Planes: %u\n", pHeader -> planes );
    printf( "Bits Per Pixel: %u\n", pHeader -> bpp );
    printf( "Compression: %u\n", pHeader -> compression );
    printf( "Bitmap Data Size: %u\n", pHeader -> bitmap_size );
    printf( "H-Resolution: %u\n", pHeader -> hres );
    printf( "V-Resolution: %u\n", pHeader -> vres );
    printf( "Used Colors: %u\n", pHeader -> used );
    printf( "Important Colors: %u\n", pHeader -> important );
    
    return;
}
