/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/

/*****************************************************************************
 * utils.cpp - utility functions for the fableofzelma application.
 *
 *
 * NOTES:
 * 1/24/14 by JAZ::Design created.
 *****************************************************************************/

#include "fableofzelma.hpp"


/*****************************************************************************
 * Function: strlower
 * Description: Overwrites an input string so that each character is lower-case
 *****************************************************************************/
void strlower(char *in) {
    for (int i = 0; in[i]; i++) {
        if (in[i] >= 'A' && in[i] <= 'Z') {
            in[i] = in[i] - 'A' + 'a';
        }
    }
    return;
}


/*****************************************************************************
* Function: print_help
* Description: Prints out the program help message.
*****************************************************************************/
void print_help() {
    printf("Usage: %s [options] <file1> [<files> ...]\n\n", EXEC_NAME);
    printf("Main options:\n");

    printf("-W <value>, --width <value>        Screen pixel width ");
    printf("(default is %d)\n", SCREEN_WIDTH_DEFAULT);

    printf("-H <value>, --height <value>       Screen pixel height ");
    printf("(default is %d)\n", SCREEN_HEIGHT_DEFAULT);

    printf("-D <value>, --debug <value>        Debug level ");
    printf("(default is %d)\n", DEBUG_DEFAULT);

    printf("-M <file>, --map <file>            Map file ");
    printf("(default is %s)\n", MAP_FNAME_DEFAULT);

    printf("-h, --help                         Print this message\n");

    printf("<file1> <file2> <file3> <file4>    Team files ");
    printf("(default is %s)\n", TEAM_FNAME_DEFAULT);

    exit(0);
}

/*****************************************************************************
* Function: raise_error
* Description: Prints out an error message determined by
* the condition and exits the program.
*****************************************************************************/
void raise_error(uint32_t error_num, const char *msg) {

    fprintf(stderr, "\n");
    switch(error_num) {
        case ERR_SFML:
            fprintf(stderr, "Error: initSDL failed: %s\n", msg);
            break;
        case ERR_USAGE:
            fprintf(stderr, "Usage: %s [-W <n>] [-H <n>] [-D <n>] ", EXEC_NAME);
            fprintf(stderr, "[-Z <file>] <file1> [<file2> <file3> <file4>]\n");
            break;
        case ERR_NOFILE1:
            fprintf(stderr, "Error: %s texture file not found\n", msg);
            break;
        case ERR_NOFILE2:
            fprintf(stderr, "Error: %s player file not found\n", msg);
            break;
        case ERR_NOFILE3:
            fprintf(stderr, "Error: %s map file not found\n", msg);
            break;
        case ERR_NOFILE4:
            fprintf(stderr, "Error: %s room file not found\n", msg);
            break;
        case ERR_NOFILE5:
            fprintf(stderr, "Error: %s audio file not found\n", msg);
            break;
        case ERR_BADFILE1:
            fprintf(stderr, "Error: %s texture file is invalid\n", msg);
            break;
        case ERR_BADFILE2:
            fprintf(stderr, "Error: %s player file is invalid\n", msg);
            break;
        case ERR_BADFILE3:
            fprintf(stderr, "Error: %s map file is invalid\n", msg);
            break;
        case ERR_BADFILE4:
            fprintf(stderr, "Error: %s room file is invalid\n", msg);
            break;
        case ERR_BADFILE5:
            fprintf(stderr, "Error: %s audio file is invalid\n", msg);
            break;
        case ERR_AUDIO:
            fprintf(stderr, "Error: audio problem in %s\n", msg);
            break;
        case ERR_NOMEM:
            fprintf(stderr, "Error: memory allocation problem in %s\n", msg);
            break;
        case ERR_OVERBUDGET:
            fprintf(stderr, "Error: team %s is over budget\n", msg);
            break;
        case ERR_UNDEFINED:
            default:
            fprintf(stderr, "Error: undefined error\n");
            break;
    }
    fprintf(stderr, "%s exited with error code %d\n", EXEC_NAME, error_num);
    exit(error_num);
}


/*****************************************************************************
 * Function: getTexCoords
 * Description: Returns texture coordinates for a specified texture map and
 * object name.
 *****************************************************************************/
float *getTexCoords(uint8_t texID, uint16_t spriteID, float *texCoords) {

    foz::Texture *myTextures = myGame->myTextures;
    texCoords[0] = 1.0*myTextures[texID].spriteMap[spriteID][0] / myTextures[texID].width;
    texCoords[3] = 1.0*myTextures[texID].spriteMap[spriteID][1] / myTextures[texID].height;
    texCoords[2] = 1.0*myTextures[texID].spriteMap[spriteID][2] / myTextures[texID].width;
    texCoords[1] = 1.0*myTextures[texID].spriteMap[spriteID][3] / myTextures[texID].height;
    texCoords[4] = 1.0*(myTextures[texID].spriteMap[spriteID][2] - myTextures[texID].spriteMap[spriteID][0]);
    texCoords[5] = 1.0*(myTextures[texID].spriteMap[spriteID][3] - myTextures[texID].spriteMap[spriteID][1]);
    return texCoords;
}





namespace foz {


    /*****************************************************************************
    * Function: Game::playSound
    * Description: Plays a specific sound effect and updates the sound buffer
    *****************************************************************************/
    void Game::playSound(uint16_t sfxID, uint8_t vol, bool force) {

      if ((mySounds[myStatus.music_buffer].getStatus() == sf::Sound::Stopped) || (force == true)) {
            mySounds[myStatus.music_buffer].setBuffer(mySoundBuffers[sfxID]);
            mySounds[myStatus.music_buffer].setVolume(vol);
            mySounds[myStatus.music_buffer].play();
            myStatus.music_buffer++;
            myStatus.music_buffer %= NUM_SOUNDS;
        }

    }

    /*****************************************************************************
    * Function: playSound
    * Description: adds room parameters to the play sound function for localized sound handling
    *****************************************************************************/
    void Game::playSound(uint16_t sfxID, uint8_t vol, bool force, int xRoom, int yRoom){
      if ((mySounds[myStatus.music_buffer].getStatus() == sf::Sound::Stopped) || (force == true) ) {
            if ( (xRoom >= myCamera.x_pos && xRoom<= myCamera.x_pos + 2 - myCamera.zoom_level) &&
                 (yRoom >= myCamera.y_pos && yRoom<= myCamera.y_pos + 2 - myCamera.zoom_level) ){
                mySounds[myStatus.music_buffer].setBuffer(mySoundBuffers[sfxID]);
                mySounds[myStatus.music_buffer].setVolume(vol);
                mySounds[myStatus.music_buffer].play();
                myStatus.music_buffer++;
                myStatus.music_buffer %= NUM_SOUNDS;
            }
        }
    }



    /*****************************************************************************
    * Function: Game::printStatus
    * Description: Prints out the game's status, in terms of player and zombie
    * locations, score, etc.
    *****************************************************************************/

    void Game::printStatus() {
        printf("\nPrinting status information\n");
    }


    /*****************************************************************************
    * Function: printConfig
    * Description: Prints the current configuration data.
    *****************************************************************************/

    void Game::printConfig() {

        if (myConfig.debug_level > 5) {
            printf("\nCurrent configuration information:\n");
            printf("\tDebug level:           %d\n", myConfig.debug_level);
            printf("\tScreen pixel width:  %d\n", myConfig.screen_width);
            printf("\tScreen pixel height: %d\n", myConfig.screen_height);

            for (int8_t i = 0; i < 4; i++) {
                printf("\tTeam %d file name: %16s\n", i, myConfig.team_fname[i]);
            }

            printf("\tMap file name: %16s\n", myConfig.map_fname);
            printf("\n");


        }
    }


} //namespace foz
