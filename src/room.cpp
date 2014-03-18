/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * room.cpp - FableofZelma application. This class contains the
 * main room class for building and managing rooms.
 *
 *
 * NOTES:
 * 3/11/14 by JAZ::Design created.
 *****************************************************************************/


#include "fableofzelma.hpp"
#include "resources.hpp"


namespace foz {



    /*****************************************************************************
    * Function: Room::compile
    * Description: Compiles the specified room file (checks the current directory
    * as well as the scripts/rooms/ directory for the file) and sets up the
    * individual tiles.
    *****************************************************************************/
    void Room::compile(uint16_t id, bool rev, bool flip) {

        std::ifstream infile;
        char *fname1, *fname2;
        char linebuf[256], tiles_str[16];
        char *linebuf_temp;
        uint32_t line_count;
        uint8_t size_ntok, tiles_ntok;
        uint16_t width_tok, height_tok, tile_tok;
        uint16_t tile_i=0, tile_j=0;
        bool size_flag=false, tile_flag=false;

        /* Does the file exist in the current directory? */
        fname1 = (char *)malloc(12);
        sprintf(fname1, "room%03d.zrf", id);
        infile.open(fname1, std::ifstream::in);
        if (!infile) {

            // If not, just check in the ROOM_DIR_DEFAULT directory
            fname2 = (char *)malloc(strlen(ROOM_DIR_DEFAULT)+12);
            sprintf(fname2, "%sroom%03d.zrf", ROOM_DIR_DEFAULT, id);
            infile.open(fname2, std::ifstream::in);
            if (!infile) {
                raise_error(ERR_NOFILE4, fname1);
            }
        }

        // We have opened the file successfully. Parse through it.
        size_flag = false;
        for (line_count=1; !infile.eof(); line_count++) {
            infile.getline(linebuf, 256);
            strlower(linebuf);

            // If we have a '#', the line is a comment and we can skip it
            // We can also skip blank lines
            if ((linebuf[0] == 0) || (linebuf[0] == '#') || (linebuf[0] == 10) || (linebuf[0] == 13)) {
                continue;
            }

            // Check to see if we're ready to allocate rooms
            if (size_flag && tile_flag) {

                if (tile_i >= height) {
                    printf("Error compiling %s, line %d\n", fname1, line_count);
                    printf("  Expected no more than %u lines of tiles\n", height);
                    printf("  Invalid line is \'%s\'", linebuf);
                    raise_error(ERR_BADFILE4, fname1);
                }

                linebuf_temp = linebuf;
                for (tile_j = 0; tile_j < width; tile_j++) {
                    linebuf_temp = strtok(linebuf_temp, " ,.-");
                    tiles_ntok = sscanf(linebuf_temp, " %hu", &tile_tok);
                    if (tiles_ntok != 1) {
                        printf("Error compiling %s, line %d\n", fname1, line_count);
                        printf("  Invalid room specification in command \'%s\'", linebuf);
                        raise_error(ERR_BADFILE4, fname1);
                    }
                    myTiles[tile_i].push_back(tile_tok);
                }
                tile_i++;
                continue;
            }

            // Check for .size information, which should be followed by a width, height pair
            // Be generous with capitalization, whitespace, and delimiters
            size_ntok = sscanf(linebuf, " .size %hu %hu", &width_tok, &height_tok);
            if (size_ntok != 2) {
                size_ntok = sscanf(linebuf, " .size %hu, %hu", &width_tok, &height_tok);
            }
            if (size_ntok != 2) {
                size_ntok = sscanf(linebuf, " .size %hu,%hu", &width_tok, &height_tok);
            }

            // We've found our .size information
            if (size_ntok == 2) {
                size_flag = true;
                width = width_tok;
                height = height_tok;
            }
            else {
                // It's not .size information, so look for the .tiles flag
                sscanf(linebuf, " %s", tiles_str);
                if (!strcmp(tiles_str, ".tiles")) {
                    tile_flag = true;
                    myTiles.resize(height);
                }
                else {
                    printf("Error compiling %s, line %d\n", fname1, line_count);
                    printf("  Unknown or unexpected command \'%s\'", linebuf);
                    raise_error(ERR_BADFILE4, fname1);
                }
            }

        }

        // Initialize the room pointers
        north = NULL;
        south = NULL;
        west = NULL;
        east = NULL;

        return;

    }

    /*****************************************************************************
    * Function: Room::draw()
    * Description: Draws the room, tile by tile
    *****************************************************************************/
    void Room::draw(){

                float texCoords[6];

            double BlockWidth = 0.10227272; // Definetly will be rounding error here if
            double BlockHeight = 0.18181818; // we stay with the identitiy insead of pixelspace

    for (int y = 0; y < height; y++){
    for (int x = 0; x < width; x++){
        glBindTexture(GL_TEXTURE_2D, myGame->myTextures[TEX_BASIC_ROOM].texHandle);
        getTexCoords(TEX_BASIC_ROOM, myTiles[x][y], texCoords);
        glBegin(GL_QUADS);
            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(-.5625 + x*BlockWidth, 1 - (y+1)*BlockHeight, 0);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(-.5625 + (x+1)*BlockWidth, 1 - (y+1)*BlockHeight, 0);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(-.5625 + (x+1)*BlockWidth, 1 - y*BlockHeight, 0);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(-.5625 + x*BlockWidth, 1 - y*BlockHeight, 0);
        glEnd();
    }
    }

    }
    /*****************************************************************************
    * Function: ~Room::Room()
    * Description: Simple destructor. Cleans up the room tiles.
    *****************************************************************************/
    Room::~Room() {

        uint16_t i;

        /* Perform a little bit of memory cleanup */
        for (i = 0; i < myTiles.size(); i++) {
            myTiles[i].clear();
        }
        myTiles.clear();

    }





} // namespace foz
