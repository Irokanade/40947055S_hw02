//
//  footballQuery.c
//  hw0302
//
//  Created by michaelleong on 17/04/2021.
//

#include "footballQuery.h"
#include <string.h>

//returns -1 when there is a problem
int footballQuery() {
    char fileName[129] = {0};
    FILE *pFile = NULL;
    int choice = 0;
    
    printf("Please open a season record: ");
    fgets(fileName, 129, stdin);
    
    if(fileName[strlen(fileName) - 1] == '\n') {
        fileName[strlen(fileName) - 1] = 0;
    } else {
        //clear the buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    
    //check the extension if .csv
    char *extension = &fileName[strlen(fileName) - 4];
    if(strcmp(extension, ".csv") != 0) {
        printf("file does not have .csv extension\n");
        return -1;
    }
    
    //printf("ext: %s\n", extension);
    
    //open the file
    if((pFile = fopen(fileName, "r")) == NULL) {
        printf("file does not exist\n");
        return -1;
    }
    
    //380 games in one season
    gameInfo gameInfoList[380];
    //initialize the list
    initializeGameListInfo(gameInfoList, sizeof(gameInfoList)/sizeof(gameInfoList[0]));
    
    //20 teams in premier league
    teamStats teamStatsList[20];
    //initialize the list
    initializeTeamStatsList(teamStatsList, sizeof(teamStatsList)/sizeof(teamStatsList[0]));
    
    int index = 0; //index to find team
    int teamListIndex = 0; //index of new team stat
    int gameIndex = 0; //index for all the games
    
    char heading[129] = {0};
    fgets(heading, 129, pFile);
    //printf("heading: %s\n", heading);
    
    while(!feof(pFile)) {
        readUntilChar(pFile, gameInfoList[gameIndex].date, ',');
        readUntilChar(pFile, gameInfoList[gameIndex].homeTeam, ',');
        readUntilChar(pFile, gameInfoList[gameIndex].awayTeam, ',');
        fscanf(pFile, "%d,%d,%c,%d,%d,%c,", &gameInfoList[gameIndex].homeScoreFT, &gameInfoList[gameIndex].awayScoreFT, &gameInfoList[gameIndex].fullTimeResult, &gameInfoList[gameIndex].homeScoreHT, &gameInfoList[gameIndex].awayScoreHT, &gameInfoList[gameIndex].halfTimeResult);
        readUntilChar(pFile, gameInfoList[gameIndex].referee, ',');
        
        //need to deal with red cards
        char useless[129] = {0};
        for(size_t i = 0; i < 10; i++) {
            //skip 10 ',' data
            //data is not needed for this homework
            readUntilChar(pFile, useless, ',');
        }
        //finally read the red card data
        fscanf(pFile, "%d,%d\n", &gameInfoList[gameIndex].homeRedCard, &gameInfoList[gameIndex].awayRedCard);
        
        //set the other data in game info
        setGameInfo(&gameInfoList[gameIndex]);
        
        
        //search for home team in list else create a new record
        index = searchNameTeamList(gameInfoList[gameIndex].homeTeam, teamStatsList, sizeof(teamStatsList)/sizeof(teamStatsList[0]));
        if(index == -1) {
            index = teamListIndex++;
        }
        //update the data for home team
        updateTeamStat(&teamStatsList[index], &gameInfoList[gameIndex], gameInfoList[gameIndex].homeTeam);
        
        //search for away team in list else create a new record
        index = searchNameTeamList(gameInfoList[gameIndex].awayTeam, teamStatsList, sizeof(teamStatsList)/sizeof(teamStatsList[0]));
        if(index == -1) {
            index = teamListIndex++;
        }
        //update the data for away team
        updateTeamStat(&teamStatsList[index], &gameInfoList[gameIndex], gameInfoList[gameIndex].awayTeam);
        
        
        gameIndex++;
    }
    
    printf("1) Who is the winner in this season?\n");
    printf("2) Which team gets the most scores?\n");
    printf("3) Which team gets the most red cards?\n");
    printf("4) Which team wins the most games at home?\n");
    printf("5) Which team wins the most games away from home?\n");
    printf("6) Which game has the most scoring gap?\n");
    printf("7) Team information.\n");
    printf("8) Exit\n");
    
    while(choice != 8) {
        printf("Choice (1-7, 8:exit): ");
        scanf("%d", &choice);
        
        int mostPoints = 0;
        int mostScores = 0;
        int mostRedCards = 0;
        int mostHomeWins = 0;
        int mostAwayWins = 0;
        int largestScoreGap = 0;
        char searchTeam[129] = {0};
        switch(choice) {
            case 1:
                //winner of the season
                for(size_t i = 0; i < sizeof(teamStatsList)/sizeof(teamStatsList[0]); i++) {
                    if(teamStatsList[i].points > mostPoints) {
                        mostPoints = teamStatsList[i].points;
                    }
                }
                
                //printf("Most points: %d\n", mostPoints);
                printf("The winner is");
                //search all team names with the same most points
                for(size_t i = 0; i < sizeof(teamStatsList)/sizeof(teamStatsList[0]); i++) {
                    if(teamStatsList[i].points == mostPoints) {
                        printf(" %s,", teamStatsList[i].name);
                    }
                }
                
                printf("\n");
                break;
                
            case 2:
                //find the most scores
                for(size_t i = 0; i < sizeof(teamStatsList)/sizeof(teamStatsList[0]); i++) {
                    if(teamStatsList[i].scores > mostScores) {
                        mostScores = teamStatsList[i].scores;
                    }
                }
                
                //search all team names with the same most scores
                for(size_t i = 0; i < sizeof(teamStatsList)/sizeof(teamStatsList[0]); i++) {
                    if(teamStatsList[i].scores == mostScores) {
                        printf(" %s,", teamStatsList[i].name);
                    }
                }
                
                printf(" %d\n", mostScores);
                break;
                
            case 3:
                //find the most red cards
                for(size_t i = 0; i < sizeof(teamStatsList)/sizeof(teamStatsList[0]); i++) {
                    if(teamStatsList[i].redCards > mostRedCards) {
                        mostRedCards = teamStatsList[i].redCards;
                    }
                }
                
                //search all team names with the same most red cards
                for(size_t i = 0; i < sizeof(teamStatsList)/sizeof(teamStatsList[0]); i++) {
                    if(teamStatsList[i].redCards == mostRedCards) {
                        printf(" %s,", teamStatsList[i].name);
                    }
                }
                
                printf(" %d\n", mostRedCards);
                break;
                
            case 4:
                //find the most home wins
                for(size_t i = 0; i < sizeof(teamStatsList)/sizeof(teamStatsList[0]); i++) {
                    if(teamStatsList[i].homeWins > mostHomeWins) {
                        mostHomeWins = teamStatsList[i].homeWins;
                    }
                }
                
                //search all team names with the same home wins
                for(size_t i = 0; i < sizeof(teamStatsList)/sizeof(teamStatsList[0]); i++) {
                    if(teamStatsList[i].homeWins == mostHomeWins) {
                        printf(" %s,", teamStatsList[i].name);
                    }
                }
                
                printf(" %d\n", mostHomeWins);
                break;
                
            case 5:
                //find the most away wins
                for(size_t i = 0; i < sizeof(teamStatsList)/sizeof(teamStatsList[0]); i++) {
                    if(teamStatsList[i].awayWins > mostAwayWins) {
                        mostAwayWins = teamStatsList[i].awayWins;
                    }
                }
                
                //search all team names with the same home wins
                for(size_t i = 0; i < sizeof(teamStatsList)/sizeof(teamStatsList[0]); i++) {
                    if(teamStatsList[i].awayWins == mostAwayWins) {
                        printf(" %s,", teamStatsList[i].name);
                    }
                }
                
                printf(" %d\n", mostAwayWins);
                break;
                
            case 6:
                //find the largest score gap
                for(size_t i = 0; i < sizeof(gameInfoList)/sizeof(gameInfoList[0]); i++) {
                    int diff = gameInfoList[i].homeScoreFT - gameInfoList[i].awayScoreFT;
                    if(diff < 0) {
                        //only want the diff so modulus diff
                        diff *= -1;
                    }
                    if(diff > largestScoreGap) {
                        largestScoreGap = diff;
                    }
                }
                
                //search all games for the same largest score gap and print it out
                for(size_t i = 0; i < sizeof(gameInfoList)/sizeof(gameInfoList[0]); i++) {
                    int diff = gameInfoList[i].homeScoreFT - gameInfoList[i].awayScoreFT;
                    if(diff < 0) {
                        //only want the diff so modulus diff
                        diff *= -1;
                    }
                    if(diff == largestScoreGap) {
                        printf("%s,%s(%d) vs %s(%d)\n", gameInfoList[i].date, gameInfoList[i].homeTeam, gameInfoList[i].homeScoreFT, gameInfoList[i].awayTeam, gameInfoList[i].awayScoreFT);
                    }
                }
                
                //printf("Largest score gap: %d\n", largestScoreGap);
                break;
                
            case 7:
                fflush(stdin);
                //team information
                printf("Team: ");
                fgets(searchTeam, 129, stdin);
                //remove the '\n' character
                if(searchTeam[strlen(searchTeam) - 1] == '\n') {
                    searchTeam[strlen(searchTeam) - 1] = 0;
                } else {
                    //clear the buffer
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF) { }
                }
                
                printf("search team: %s\n", searchTeam);
                //search for team information via name
                for(size_t i = 0; i < sizeof(teamStatsList)/sizeof(teamStatsList[0]); i++) {
                    if(strcmp(searchTeam, teamStatsList[i].name) == 0) {
                        printf("Points: %d\n", teamStatsList[i].points);
                        printf("Win/Draw/Lose: %d/%d/%d\n", teamStatsList[i].homeWins+teamStatsList[i].awayWins, teamStatsList[i].homeDraws+teamStatsList[i].awayDraws, teamStatsList[i].homeLoses+teamStatsList[i].awayLoses);
                        printf("Goals Scored/Goals Against: %d/%d\n", teamStatsList[i].scores, teamStatsList[i].scoresAgainst);
                    }
                }
                
                break;
                
            case 8:
                break;
                
            default:
                printf("Invalid option >:[\n");
                break;
        }
    }
    
    
    fclose(pFile);
    return 1;
}

//search for name in team list
int searchNameTeamList(char *name, teamStats teamStatsList[], size_t teamListSize) {
    for(size_t i = 0; i < teamListSize; i++) {
        if(strcmp(name, teamStatsList[i].name) == 0) {
            return (int)i; //returns the index of the list
        }
    }
    
    return -1; //return -1 if not found
}

int readUntilChar(FILE *pFile, char *result, char stopChar) {
    while(!feof(pFile)) {
        char temp = fgetc(pFile);
        //stop when reach the stop char or end of string or new line
        if(temp != stopChar && temp != 0 && temp != '\n') {
            strncat(result, &temp, 1);
        } else {
            return 1;
        }
    }
    
    return 1;
}

void initializeGameListInfo(gameInfo gameInfoList[], size_t size) {
    for(size_t i = 0; i < size; i++) {
        memset(gameInfoList[i].date, 0, sizeof(gameInfoList[i].date)/sizeof(gameInfoList[i].date[0]));
        memset(gameInfoList[i].awayTeam, 0, sizeof(gameInfoList[i].awayTeam)/sizeof(gameInfoList[i].awayTeam[0]));
        memset(gameInfoList[i].homeTeam, 0, sizeof(gameInfoList[i].homeTeam)/sizeof(gameInfoList[i].homeTeam[0]));
        gameInfoList[i].awayScoreFT = 0;
        gameInfoList[i].awayScoreHT = 0;
        gameInfoList[i].homeScoreFT = 0;
        gameInfoList[i].homeScoreHT = 0;
        gameInfoList[i].fullTimeResult = '0';
        gameInfoList[i].halfTimeResult = '0';
        memset(gameInfoList[i].winner, 0, sizeof(gameInfoList[i].winner)/sizeof(gameInfoList[i].winner[0]));
        memset(gameInfoList[i].loser, 0, sizeof(gameInfoList[i].loser)/sizeof(gameInfoList[i].loser[0]));
        memset(gameInfoList[i].referee, 0, sizeof(gameInfoList[i].referee)/sizeof(gameInfoList[i].referee[0]));
    }
}

void initializeTeamStatsList(teamStats teamStatsList[], size_t size) {
    for(size_t i = 0; i < size; i++) {
        memset(teamStatsList[i].name, 0, sizeof(teamStatsList[i].name)/sizeof(teamStatsList[i].name[0]));
        teamStatsList[i].awayWins = 0;
        teamStatsList[i].awayLoses = 0;
        teamStatsList[i].awayDraws = 0;
        teamStatsList[i].homeWins = 0;
        teamStatsList[i].homeLoses = 0;
        teamStatsList[i].homeDraws = 0;
        teamStatsList[i].totalScores = 0;
        teamStatsList[i].points = 0;
        teamStatsList[i].scores = 0;
        teamStatsList[i].scoresAgainst = 0;
    }
}

void setGameInfo(gameInfo *gameInfoPage) {
    //set the winner
    if(gameInfoPage->fullTimeResult == 'H') {
        strncpy(gameInfoPage->winner, gameInfoPage->homeTeam, strlen(gameInfoPage->homeTeam));
        strncpy(gameInfoPage->loser, gameInfoPage->awayTeam, strlen(gameInfoPage->homeTeam));
    } else if(gameInfoPage->fullTimeResult == 'A') {
        strncpy(gameInfoPage->winner, gameInfoPage->awayTeam, strlen(gameInfoPage->homeTeam));
        strncpy(gameInfoPage->loser, gameInfoPage->homeTeam, strlen(gameInfoPage->homeTeam));
    }
    //else it is draw so don't set any winners
}

void updateTeamStat(teamStats *teamStatsPage, gameInfo *gameInfoPage, char *teamName) {
    //check if home of away
    if(strcmp(gameInfoPage->homeTeam, teamName) == 0) {
        //team is home
        //update name as might be a new record
        strncpy(teamStatsPage->name, gameInfoPage->homeTeam, strlen(gameInfoPage->homeTeam));
        //update home wins of losses or draws and update points
        if(gameInfoPage->fullTimeResult == 'H') {
            teamStatsPage->homeWins += 1;
            //update points
            teamStatsPage->points += 3;
        } else if(gameInfoPage->fullTimeResult == 'A') {
            teamStatsPage->homeLoses += 1;
        } else {
            teamStatsPage->homeDraws += 1;
            //update points
            teamStatsPage->points += 1;
        }
        
        //set the scores
        teamStatsPage->scores += gameInfoPage->homeScoreFT;
        teamStatsPage->scoresAgainst += gameInfoPage->awayScoreFT;
        
        //set red cards
        teamStatsPage->redCards += gameInfoPage->homeRedCard;
        
    } else {
        //else it is away
        //update name as might be a new record
        strncpy(teamStatsPage->name, gameInfoPage->awayTeam, strlen(gameInfoPage->awayTeam));
        //update away wins or losses or draws and set points
        if(gameInfoPage->fullTimeResult == 'A') {
            teamStatsPage->awayWins += 1;
            //update points
            teamStatsPage->points += 3;
        } else if(gameInfoPage->fullTimeResult == 'H') {
            teamStatsPage->awayLoses += 1;
        } else {
            teamStatsPage->awayDraws += 1;
            //update points
            teamStatsPage->points += 1;
        }
        
        //set the scores
        teamStatsPage->scores += gameInfoPage->awayScoreFT;
        teamStatsPage->scoresAgainst += gameInfoPage->homeScoreFT;
        
        //set red cards
        teamStatsPage->redCards += gameInfoPage->awayRedCard;
    }
}
