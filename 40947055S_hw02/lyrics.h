//
//  lyrics.h
//  hw0301
//
//  Created by michaelleong on 15/04/2021.
//

#ifndef lyrics_h
#define lyrics_h

#include <stdio.h>

typedef struct _timeStamp {
    long int minutes;
    long int seconds;
    long int milliseconds;
    char lyricsTxt[129];
} timeStamp;

typedef struct _singer {
    char name[129];
    char *colour;
} singer;

int searchName(char *name, singer singerList[], size_t size);
int lyrics(void);

#endif /* lyrics_h */
