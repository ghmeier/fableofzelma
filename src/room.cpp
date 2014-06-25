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
#include "object.hpp"


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

                linebuf_temp = strtok(linebuf, " ,.-");
                for (tile_j = 0; tile_j < width; tile_j++) {
                    tiles_ntok = sscanf(linebuf_temp, " %hu", &tile_tok);
                    if (tiles_ntok != 1) {
                        printf("Error compiling %s, line %d\n", fname1, line_count);
                        printf("  Invalid room specification in command \'%s\'", linebuf);
                        raise_error(ERR_BADFILE4, fname1);
                    }
                    if (tile_tok<=12 || tile_tok==50) {//12 is the number of background sprites and 50 is the doors
                        myTiles[tile_i].push_back(tile_tok);
                    }else {
                        myTiles[tile_i].push_back(FLOOR_TILE);
                       if (tile_tok<100) {
                            Object toPush(tile_tok,tile_j*59.0-380.0,tile_i*(-59.0)+321.0);
                            myObjects.push_back(toPush);
                        }else {
                            Enemy toAdd(tile_tok,tile_j*59.0-380.0,tile_i*(-59.0)+321.0);
                            myEnemies.push_back(toAdd);
                        }
                    }
                    linebuf_temp = strtok(NULL, " ,.-");
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
                }else {
                    printf("Error compiling %s, line %d\n", fname1, line_count);
                    printf("  Unknown or unexpected command \'%s\'", linebuf);
                    raise_error(ERR_BADFILE4, fname1);
                }
            }

        }


        // Flip and reverse as necessary
        if (rev == true) {
            for (tile_i = 0; tile_i < height; tile_i++) {
                std::reverse(myTiles[tile_i].begin(), myTiles[tile_i].end());
            }
        }

        if (flip == true) {
            std::reverse(myTiles.begin(), myTiles.end());
        }

        return;

    }

    /*****************************************************************************
    * Function: Room::draw()
    * Description: Draws the room, tile by tile
    *****************************************************************************/
    void Room::draw(){

        float texCoords[6];

        #define ROOM_CORNER_SIZE 249
        #define ROOM_WALL_SIZE 219
        #define ROOM_MIDDLE_SIZE 59
        #define ROOM_START_X -540 // 539.5
        #define ROOM_START_Y 540  // 539.5
        #define FLOOR_BORDER 30
        #define DOOR_HEIGHT 75
        #define DOOR_CHECK 50


        glBindTexture(GL_TEXTURE_2D, myGame->myTextures[TEX_BASIC_ROOM].texHandle);
        glBegin(GL_QUADS);

        // Draw the north wall first
        //NW corner
        getTexCoords(TEX_BASIC_ROOM, WALL_TOP_LEFT, texCoords);
        glTexCoord2d(texCoords[0], texCoords[1]);
        glVertex3f(ROOM_START_X, ROOM_START_Y-ROOM_CORNER_SIZE, CORNER_DEPTH);
        glTexCoord2d(texCoords[2], texCoords[1]);
        glVertex3f(ROOM_START_X + ROOM_CORNER_SIZE, ROOM_START_Y-ROOM_CORNER_SIZE, CORNER_DEPTH);
        glTexCoord2d(texCoords[2], texCoords[3]);
        glVertex3f(ROOM_START_X + ROOM_CORNER_SIZE, ROOM_START_Y, CORNER_DEPTH);
        glTexCoord2d(texCoords[0], texCoords[3]);
        glVertex3f(ROOM_START_X, ROOM_START_Y, CORNER_DEPTH);

        // North Wall
        getTexCoords(TEX_BASIC_ROOM, WALL_TOP_CENTER, texCoords);
        for (uint16_t j = 1; j < (width-1); j++) {
            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(ROOM_START_X+ROOM_WALL_SIZE+(j-1)*ROOM_MIDDLE_SIZE, ROOM_START_Y-ROOM_WALL_SIZE, WALL_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(ROOM_START_X+ROOM_WALL_SIZE+j*ROOM_MIDDLE_SIZE, ROOM_START_Y-ROOM_WALL_SIZE, WALL_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(ROOM_START_X+ROOM_WALL_SIZE+j*ROOM_MIDDLE_SIZE, ROOM_START_Y, WALL_DEPTH);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(ROOM_START_X+ROOM_WALL_SIZE+(j-1)*ROOM_MIDDLE_SIZE, ROOM_START_Y, WALL_DEPTH);
        }

        // NE corner
        getTexCoords(TEX_BASIC_ROOM, WALL_TOP_RIGHT, texCoords);
        glTexCoord2d(texCoords[0], texCoords[1]);
        glVertex3f(ROOM_START_X + ROOM_WALL_SIZE + (width - 2)*ROOM_MIDDLE_SIZE - FLOOR_BORDER, ROOM_START_Y-ROOM_CORNER_SIZE, CORNER_DEPTH);
        glTexCoord2d(texCoords[2], texCoords[1]);
        glVertex3f(ROOM_START_X + 2*ROOM_WALL_SIZE + (width - 2)*ROOM_MIDDLE_SIZE, ROOM_START_Y-ROOM_CORNER_SIZE, CORNER_DEPTH);
        glTexCoord2d(texCoords[2], texCoords[3]);
        glVertex3f(ROOM_START_X + 2*ROOM_WALL_SIZE + (width - 2)*ROOM_MIDDLE_SIZE, ROOM_START_Y, CORNER_DEPTH);
        glTexCoord2d(texCoords[0], texCoords[3]);
        glVertex3f(ROOM_START_X + ROOM_WALL_SIZE + (width - 2)*ROOM_MIDDLE_SIZE - FLOOR_BORDER, ROOM_START_Y, CORNER_DEPTH);


        // West Wall
        getTexCoords(TEX_BASIC_ROOM, WALL_LEFT, texCoords);
        for (uint16_t j = 1; j < (height-1); j++) {
            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(ROOM_START_X, ROOM_START_Y-ROOM_WALL_SIZE-j*ROOM_MIDDLE_SIZE, WALL_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(ROOM_START_X+ROOM_WALL_SIZE, ROOM_START_Y-ROOM_WALL_SIZE-j*ROOM_MIDDLE_SIZE, WALL_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(ROOM_START_X+ROOM_WALL_SIZE, ROOM_START_Y-ROOM_WALL_SIZE-(j-1)*ROOM_MIDDLE_SIZE, WALL_DEPTH);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(ROOM_START_X, ROOM_START_Y-ROOM_WALL_SIZE-(j-1)*ROOM_MIDDLE_SIZE, WALL_DEPTH);
        }

        // East Wall
        getTexCoords(TEX_BASIC_ROOM, WALL_RIGHT, texCoords);
        for (uint16_t j = 1; j < (height-1); j++) {
            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(ROOM_START_X + ROOM_WALL_SIZE +(width-2)*ROOM_MIDDLE_SIZE, ROOM_START_Y-ROOM_WALL_SIZE-j*ROOM_MIDDLE_SIZE, WALL_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(ROOM_START_X+2*ROOM_WALL_SIZE + (width-2)*ROOM_MIDDLE_SIZE, ROOM_START_Y-ROOM_WALL_SIZE-j*ROOM_MIDDLE_SIZE, WALL_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(ROOM_START_X+2*ROOM_WALL_SIZE + (width-2)*ROOM_MIDDLE_SIZE, ROOM_START_Y-ROOM_WALL_SIZE-(j-1)*ROOM_MIDDLE_SIZE, WALL_DEPTH);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(ROOM_START_X + ROOM_WALL_SIZE + (width-2)* ROOM_MIDDLE_SIZE, ROOM_START_Y-ROOM_WALL_SIZE-(j-1)*ROOM_MIDDLE_SIZE, WALL_DEPTH);
        }


        // Draw the south wall last //
        //SW corner
        getTexCoords(TEX_BASIC_ROOM, WALL_BOTTOM_LEFT, texCoords);
        glTexCoord2d(texCoords[0], texCoords[1]);
        glVertex3f(ROOM_START_X, ROOM_START_Y-2*ROOM_WALL_SIZE-(height-2)*ROOM_MIDDLE_SIZE, CORNER_DEPTH);
        glTexCoord2d(texCoords[2], texCoords[1]);
        glVertex3f(ROOM_START_X + ROOM_CORNER_SIZE, ROOM_START_Y-2*ROOM_WALL_SIZE-(height-2)*ROOM_MIDDLE_SIZE, CORNER_DEPTH);
        glTexCoord2d(texCoords[2], texCoords[3]);
        glVertex3f(ROOM_START_X + ROOM_CORNER_SIZE, ROOM_START_Y-ROOM_WALL_SIZE-(height-2)*ROOM_MIDDLE_SIZE + FLOOR_BORDER, CORNER_DEPTH);
        glTexCoord2d(texCoords[0], texCoords[3]);
        glVertex3f(ROOM_START_X, ROOM_START_Y-ROOM_WALL_SIZE-(height-2)*ROOM_MIDDLE_SIZE + FLOOR_BORDER, CORNER_DEPTH);

        // South Wall
        getTexCoords(TEX_BASIC_ROOM, WALL_BOTTOM, texCoords);
        for (uint16_t j = 1; j < (width-1); j++) {
            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(ROOM_START_X+ROOM_WALL_SIZE+(j-1)*ROOM_MIDDLE_SIZE, ROOM_START_Y-2*ROOM_WALL_SIZE-(height - 2)*ROOM_MIDDLE_SIZE, WALL_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(ROOM_START_X+ROOM_WALL_SIZE+j*ROOM_MIDDLE_SIZE, ROOM_START_Y-2*ROOM_WALL_SIZE-(height - 2)*ROOM_MIDDLE_SIZE, WALL_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(ROOM_START_X+ROOM_WALL_SIZE+j*ROOM_MIDDLE_SIZE, ROOM_START_Y-ROOM_WALL_SIZE-(height - 2)*ROOM_MIDDLE_SIZE, WALL_DEPTH);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(ROOM_START_X+ROOM_WALL_SIZE+(j-1)*ROOM_MIDDLE_SIZE, ROOM_START_Y-ROOM_WALL_SIZE-(height - 2)*ROOM_MIDDLE_SIZE, WALL_DEPTH);
        }

        // SE corner
        getTexCoords(TEX_BASIC_ROOM, WALL_BOTTOM_RIGHT, texCoords);
        glTexCoord2d(texCoords[0], texCoords[1]);
        glVertex3f(ROOM_START_X+ROOM_WALL_SIZE+(width-2)*ROOM_MIDDLE_SIZE - FLOOR_BORDER, ROOM_START_Y-2*ROOM_WALL_SIZE-(height-2)*ROOM_MIDDLE_SIZE, CORNER_DEPTH);
        glTexCoord2d(texCoords[2], texCoords[1]);
        glVertex3f(ROOM_START_X+2*ROOM_WALL_SIZE+(width-2)*ROOM_MIDDLE_SIZE, ROOM_START_Y-2*ROOM_WALL_SIZE-(height-2)*ROOM_MIDDLE_SIZE, CORNER_DEPTH);
        glTexCoord2d(texCoords[2], texCoords[3]);
        glVertex3f(ROOM_START_X+2*ROOM_WALL_SIZE+(width-2)*ROOM_MIDDLE_SIZE, ROOM_START_Y-ROOM_WALL_SIZE-(height-2)*ROOM_MIDDLE_SIZE + FLOOR_BORDER, CORNER_DEPTH);
        glTexCoord2d(texCoords[0], texCoords[3]);
        glVertex3f(ROOM_START_X+ROOM_WALL_SIZE+(width-2)*ROOM_MIDDLE_SIZE - FLOOR_BORDER, ROOM_START_Y-ROOM_WALL_SIZE-(height-2)*ROOM_MIDDLE_SIZE + FLOOR_BORDER, CORNER_DEPTH);


        // Cycles through possible places for a door
        // North Door?
        if(myTiles[0][6] == DOOR_CHECK){
            getTexCoords(TEX_BASIC_ROOM, DOOR_TOP, texCoords);
            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(-58, ROOM_START_Y - ROOM_WALL_SIZE + FLOOR_BORDER, CORNER_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(58, ROOM_START_Y - ROOM_WALL_SIZE + FLOOR_BORDER, CORNER_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(58, ROOM_START_Y - ROOM_WALL_SIZE + DOOR_HEIGHT + FLOOR_BORDER, CORNER_DEPTH);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(-58, ROOM_START_Y - ROOM_WALL_SIZE + DOOR_HEIGHT + FLOOR_BORDER, CORNER_DEPTH);
        }

        // South Door?
        if(myTiles[12][6] == DOOR_CHECK){
            getTexCoords(TEX_BASIC_ROOM, DOOR_BOTTOM, texCoords);
            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(-58, ROOM_START_Y - ROOM_WALL_SIZE - (width - 2)*ROOM_MIDDLE_SIZE - FLOOR_BORDER - DOOR_HEIGHT, CORNER_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(58, ROOM_START_Y - ROOM_WALL_SIZE - (width - 2)*ROOM_MIDDLE_SIZE - FLOOR_BORDER - DOOR_HEIGHT, CORNER_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(58, ROOM_START_Y - ROOM_WALL_SIZE - (width - 2)*ROOM_MIDDLE_SIZE - FLOOR_BORDER, CORNER_DEPTH);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(-58, ROOM_START_Y - ROOM_WALL_SIZE - (width - 2)*ROOM_MIDDLE_SIZE - FLOOR_BORDER, CORNER_DEPTH);
        }

        // West Door?
        if(myTiles[6][0] == DOOR_CHECK){
            getTexCoords(TEX_BASIC_ROOM, DOOR_LEFT, texCoords);
            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(ROOM_START_X + ROOM_WALL_SIZE - FLOOR_BORDER - DOOR_HEIGHT, -58, CORNER_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(ROOM_START_X + ROOM_WALL_SIZE - FLOOR_BORDER, -58, CORNER_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(ROOM_START_X + ROOM_WALL_SIZE - FLOOR_BORDER, 58, CORNER_DEPTH);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(ROOM_START_X + ROOM_WALL_SIZE - FLOOR_BORDER - DOOR_HEIGHT, 58, CORNER_DEPTH);
        }

        // East Door?
        if(myTiles[6][12] == DOOR_CHECK){
            getTexCoords(TEX_BASIC_ROOM, DOOR_RIGHT, texCoords);
            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(ROOM_START_X + ROOM_WALL_SIZE + (width - 2)*ROOM_MIDDLE_SIZE + FLOOR_BORDER, -58, CORNER_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(ROOM_START_X + ROOM_WALL_SIZE + (width - 2)*ROOM_MIDDLE_SIZE + FLOOR_BORDER + DOOR_HEIGHT, -58, CORNER_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(ROOM_START_X + ROOM_WALL_SIZE + (width - 2)*ROOM_MIDDLE_SIZE + FLOOR_BORDER + DOOR_HEIGHT, 58, CORNER_DEPTH);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(ROOM_START_X + ROOM_WALL_SIZE + (width - 2)*ROOM_MIDDLE_SIZE + FLOOR_BORDER, 58, CORNER_DEPTH);
        }

        // Floor Tiles
        for (uint16_t i = 1; i < (width - 1); i++) {
            for (uint16_t j = 1; j < (height - 1); j++) {
                getTexCoords(TEX_BASIC_ROOM, myTiles[j][i], texCoords);
                glTexCoord2d(texCoords[0], texCoords[1]);
                glVertex3f(ROOM_START_X + ROOM_WALL_SIZE + (i-1)*ROOM_MIDDLE_SIZE, ROOM_START_Y-ROOM_WALL_SIZE-(j)*ROOM_MIDDLE_SIZE, FLOOR_DEPTH);
                glTexCoord2d(texCoords[2], texCoords[1]);
                glVertex3f(ROOM_START_X + ROOM_WALL_SIZE + (i)*ROOM_MIDDLE_SIZE, ROOM_START_Y-ROOM_WALL_SIZE-(j)*ROOM_MIDDLE_SIZE, WALL_DEPTH);
                glTexCoord2d(texCoords[2], texCoords[3]);
                glVertex3f(ROOM_START_X + ROOM_WALL_SIZE + (i)*ROOM_MIDDLE_SIZE, ROOM_START_Y-ROOM_WALL_SIZE-(j-1)*ROOM_MIDDLE_SIZE, WALL_DEPTH);
                glTexCoord2d(texCoords[0], texCoords[3]);
                glVertex3f(ROOM_START_X + ROOM_WALL_SIZE + (i-1)*ROOM_MIDDLE_SIZE, ROOM_START_Y-ROOM_WALL_SIZE-(j-1)*ROOM_MIDDLE_SIZE, WALL_DEPTH);
            }
        }

        /*Draw Objects*/
        for (uint16_t i = 0; i< myObjects.size(); i++) {
                myObjects[i].draw();
        }

        //handle everything to do with enemies
        updateEnemies();

        glEnd();

        return;
    }

    /***********************************************************************
    * Function: Room::updateEnemies()
    * Description: Draw, change commands, and check collisions for enemies
    ************************************************************************/
    void Room::updateEnemies(){
        int CMDFRAMEMAX  =20;
        for (uint16_t i=0;i<myEnemies.size(); i++) {
            // If we are done with the commands, move on to the next team
            if (myEnemies[i].cmds_done == true) {
                continue;
            }

            Enemy *e = &myEnemies[i];

            if (!e->active) {
                continue;
            }

            cmd_type *curCmd = &myGame->enemyCommands[e->type][e->cmdIter];
            Link *toHit = NULL;

            //check for collisions :)
            e->can_move = true; //Link can move unless we find something in his way
                    for (uint16_t obj = 0; obj < this->myObjects.size(); obj++) {
                        foz::Object *myObject = &this->myObjects[obj];
                        if (myObject->active && myGame->linkColObj(e,myObject)) {
                            if ( myObject->type == ARROW_EAST || myObject->type == ARROW_NORTH || myObject->type == ARROW_SOUTH || myObject->type == ARROW_WEST){
                                myObject->active = false;
                                e->doDamage(10);
                            }else if (myObject->status == SOLID) {
                                e->can_move = false;
                            }else {
                                if (myObject->type == VOID_BLOCK) {
                                    e->health = -1;
                                }
                            }
                        }
                    }

                    for (uint16_t teams = 0; teams<4; teams++) {
                        for (uint16_t link = 0; link < myGame->myLinks[teams].size();link++) {
                               Link* current =  &myGame->myLinks[teams][link];
                            if (current->active && myGame->linkColLink(e,current)) {
                                e->can_move = false;
                                toHit = current;
                            }

                        }
                    }

                    for (uint16_t link = 0; link < myEnemies.size(); link++) {
                        if (link!=i) {
                            Link* current = &myEnemies[link];
                            if (current->active && myGame->linkColLink(e,current)) {
                                e->can_move = false;
                            }
                        }
                    }

                    //**IMPORTANT NOTE: the numbers here correspond to the pixel value of the edge of a given room in relation to Link.**
                    //**If you're planning on changing this be sure you have a good reason to or write these values down.**
                    if (e->y <= -328.0) {
                        if (e->direction == DIRECTION_SOUTH) {
                                e->can_move = false;
                                e->y = -328.0;
                        }else {
                            e->y = -328.0;
                        }
                    }
                    if (e->y >= 266.0) {
                        if (e->direction == DIRECTION_NORTH) {
                                e->can_move = false;
                                e->y = 266.0;
                        }else {
                            e-> y = 266.0;
                        }
                    }
                    if (e->x >= 270.0) {
                        if (e->direction == DIRECTION_EAST) {
                                e->can_move = false;
                                e->x = 270.0;
                        }else {
                            e->x = 270.0;
                        }
                    }
                    if (e->x <= -321.0) {
                        if (e->direction == DIRECTION_WEST) {
                                e->can_move = false;
                                e->x = -321.0;
                        }else {
                            //e->x = -321;
                        }
                    }

            switch (curCmd->cmd) {
                case MOVE_CMD:
                    if (e->can_move) {
                        e->update(curCmd->cmd);
                    }else {
                        e->update(WAIT_CMD);
                    }
                    e->cur_cmdframe++;
                    if (e->cur_cmdframe >= CMDFRAMEMAX) {
                        e->cur_cmdframe = 0;
                        e->cur_cmdopt++;
                        if (e->cur_cmdopt >= curCmd->opt[0]) {
                            e->cmdIter++;
                            e->cur_cmdopt = 0;
                        }
                    }
                    break;
                case LEFT_CMD:
                case RIGHT_CMD:
                case WAIT_CMD:
                case ACTIVATE_CMD:
                    if (e->cur_cmdframe==0) {
                        e->update(curCmd->cmd);
                    }

                    e->cur_cmdframe++;
                    if (e->cur_cmdframe >= CMDFRAMEMAX) {
                        e->cur_cmdframe = 0;
                        e->cmdIter++;
                    }
                    break;

                case ATTACK_CMD:
                    e->update(curCmd->cmd);
                    e->cur_cmdframe++;
                    if (e->cur_cmdframe==10) {
                        if (toHit != NULL) {
                            toHit->doDamage(e->damage);
                        }
                    }
                    if (e->cur_cmdframe >= CMDFRAMEMAX) {
                        e->cur_cmdframe = 0;
                        e->cmdIter++;
                    }
                    break;

                default:
                    if (curCmd->cmd == GOTO_CMD){
                        for (uint16_t j = 0; j < myGame->enemyCommands[e->type].size(); j++) {
                            if (strcmp(myGame->enemyCommands[e->type][j].label_str,curCmd->target_str)==0){
                                e->cmdIter = j;
                                curCmd = &myGame->enemyCommands[e->type][e->cmdIter];
                                e->cur_cmdframe = 0;
                                break;
                            }
                        }
                    }else {
                        e->cur_cmdframe++;
                        // Have we reached the end of a CMDFRAME?
                        // If so, see how many squares we have left to go.
                        if (e->cur_cmdframe >= CMDFRAMEMAX) {
                            e->cur_cmdframe = 0;
                            e->cmdIter++;
                        }
                    }
                    break;
            }
            e->draw();
            if (!e->active) {
                //delete e;
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
        for (int i=0;i<myEnemies.size();i++) {
            myEnemies[i].~Enemy();
        }
        myEnemies.clear();
        for (int i=0;i<myObjects.size();i++) {
            myObjects[i].~Object();
        }
        myObjects.clear();
    }





} // namespace foz
