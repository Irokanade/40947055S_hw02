//
//  bmpDistortion.h
//  hw0303
//
//  Created by michaelleong on 23/04/2021.
//

#ifndef bmpDistortion_h
#define bmpDistortion_h

#include <stdio.h>
#include <stdint.h>

struct _sBmpHeader
{
    char        bm[2];
    uint32_t    size;
    uint32_t    reserve;
    uint32_t    offset;
    uint32_t    header_size;
    int32_t    width;
    int32_t    height;
    uint16_t    planes;
    uint16_t    bpp;
    uint32_t    compression;
    uint32_t    bitmap_size;
    uint32_t    hres;
    uint32_t    vres;
    uint32_t    used;
    uint32_t    important;
}__attribute__ ((__packed__));

typedef struct _sBmpHeader sBmpHeader;

int bmpDistortion(void);

#endif /* bmpDistortion_h */
