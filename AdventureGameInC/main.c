//
//  main.c
//  AdventureGameInC
//
//  Created by JIAN WANG on 5/9/15.
//  Copyright (c) 2015 JWANG. All rights reserved.
//

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum {
    empty,
    cube,
    treasure,
}Property;

typedef struct Room Room;

struct Room {
    Property property;
    char exits[4];
    Room *nextRoom;
    Room *previousRoom;
};

typedef struct {
    int numberOfLives;
    Room *room;
}Player;

// Funtion prototype
void setAllExits(Room *room);
void showExits(Room *room);
int randomNumber();
void displayPlayerInfo(Player *player);
char getUserInput(Player *player);
void movePlayer(Player *player, char direction);
Player *setupRoomAndPlayer();
void gameIntro();
void showGameResult(Player *player);
void freeMemory(Player *player);



int main(int argc, const char * argv[]) {
    srand((unsigned int)time(NULL));
    
    gameIntro();
    
    Player *player = setupRoomAndPlayer();
    
    while (player->numberOfLives > 0 && player->room->property != 2) {
        displayPlayerInfo(player);
        movePlayer(player, getUserInput(player));
    }
    
    showGameResult(player);
    
    freeMemory(player);
    
    return 0;
}

void gameIntro() {
    printf("Welcome to the Adventure Game! Your goal is to find the treasure in one of the sixteen rooms that are on a 4 * 4 grid. Every room has exits and the direction depends on the room you are in. You have two lives. If the room you enter has a gelatinous cube, you will lose a life. You lost if all your lives were gone.\n");
}

Player *setupRoomAndPlayer() {
    Room *room1 = (Room *)malloc(sizeof(Room));
    setAllExits(room1);
    room1->exits[0] = '-';
    room1->exits[2] = '-';
    
    Player *player = (Player *)malloc(sizeof(Player));
    player->numberOfLives = 2;
    
    int playerStartingRoomNumber = randomNumber();
    
    int cubeRoom = randomNumber();
    while (cubeRoom == playerStartingRoomNumber) {
        cubeRoom = randomNumber();
    }
    
    int treasureRoom = randomNumber();
    while (treasureRoom == playerStartingRoomNumber | treasureRoom == cubeRoom) {
        treasureRoom = randomNumber();
    }
    
    printf("PlayerRoom - %d,\n cubeRoom - %d, treasuerRoom - %d\n", playerStartingRoomNumber, cubeRoom, treasureRoom);
    
    for (int i = 16; i > 1; i--) {
        Room *newRoom = (Room *)malloc(sizeof(Room));
        setAllExits(newRoom);
        
        if (i == cubeRoom) {
            newRoom->property = 1;
        } else if (i == treasureRoom) {
            newRoom->property = 2;
        } else if (i == playerStartingRoomNumber) {
            player->room = newRoom;
        }
        
        if (i < 5) {
            newRoom->exits[0] = '-';
        }
        if (i > 12) {
            newRoom->exits[1] = '-';
        }
        if (i % 4 == 1) {
            newRoom->exits[2] = '-';
        }
        if (i % 4 == 0) {
            newRoom->exits[3] = '-';
        }
        
        if (room1->nextRoom == NULL) {
            room1->nextRoom = newRoom;
            newRoom->previousRoom = room1;
        } else {
            newRoom->nextRoom = room1->nextRoom;
            room1->nextRoom->previousRoom = newRoom;
            room1->nextRoom = newRoom;
            newRoom->previousRoom = room1;
        }
    }
    return player;
}


void setAllExits(Room *room) {
    room->exits[0] = 'N';
    room->exits[1] = 'S';
    room->exits[2] = 'W';
    room->exits[3] = 'E';
}

void displayPlayerInfo(Player *player) {
    printf("You have %d lives ", player->numberOfLives);
    showExits(player->room);
}

void showExits(Room *room) {
    printf("and the room you are in has following exits: \n");
    for (int i = 0 ; i < 4; i++) {
        if (room->exits[i] != '-')  printf("%c\n", room->exits[i]);
    }
}

int randomNumber() {
    return rand() % 16 + 1;
}

char getUserInput(Player *player) {
    char str;
    int loopStop = 0;
    do {
        printf("Please choose your exit >");
        fpurge(stdin);
        scanf("%c", &str);
        str = toupper(str);
        for (int i = 0; i < 4; i++) {
            if (player->room->exits[i] == str) {
                loopStop = 1;
            }
        }
    } while (!loopStop);
    
    printf("You have chosen exit - %c\n", str);
    return str;
}

void movePlayer(Player *player, char direction) {
    if (direction == 'N') {
        player->room = player->room->previousRoom->previousRoom->previousRoom->previousRoom;
    } else if (direction == 'S') {
        player->room = player->room->nextRoom->nextRoom->nextRoom->nextRoom;
    } else if (direction == 'W') {
        player->room = player->room->previousRoom;
    } else if (direction == 'E') {
        player->room = player->room->nextRoom;
    }
    
    if (player->room->property == 1) {
        player->numberOfLives -= 1;
    }
}

void showGameResult(Player *player) {
    if (player->numberOfLives == 0) {
        printf("You lost all your lives! You lost!");
    } else if (player->room->property == 2) {
        printf("Yeah! You found the treasure! You won!");
    }
}

void freeMemory(Player *player) {
    
    Room *previousRoom = player->room->previousRoom;
    
    while (previousRoom->previousRoom != NULL) {
        Room *currentRoom = previousRoom;
        previousRoom = previousRoom->previousRoom;
        free(currentRoom);
    }
    free(previousRoom);
    
    while (player->room->nextRoom != NULL) {
        Room *currentRoom = player->room;
        player->room = player->room->nextRoom;
        free(currentRoom);
    }
    free(player->room);

    free(player);
}


