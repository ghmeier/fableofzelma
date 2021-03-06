/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * world.cpp - FableofZelma application. This class contains the
 * main world class for building and managing worlds (maps).
 *
 *
 * NOTES:
 * 3/11/14 by JAZ::Design created.
 *****************************************************************************/


#include "fableofzelma.hpp"


namespace foz {

    /*****************************************************************************
    * Function: World::draw
    * Description: Draws the structure of the rooms in the world.
    *****************************************************************************/
    void World::draw() {
        uint16_t i, j, k, i_team;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(-1080.0/2.0*(width-1), 1080.0/2.0*(height-1), 0.0);

        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                myRooms[i][j].draw();
                glTranslatef(1080.0, 0.0, 0.0);
            }
            glTranslatef(-1080.0*(width), -1080.0, 0.0);
        }


        // Draw any active objects in each room
        glLoadIdentity();
        glTranslatef(-1080.0/2.0*(width-1), 1080.0/2.0*(height-1), 0.0);
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                for (i_team = 0; i_team < 4; i_team++) {
                    for (k = 0; k < myGame->myTeams[i_team].myLinks.size(); k++) {
                        if ((myGame->myTeams[i_team].myLinks[k].active == true) &&
                            (myGame->myTeams[i_team].myLinks[k].room_x == j) &&
                            (myGame->myTeams[i_team].myLinks[k].room_y == i)) {
                            myGame->myTeams[i_team].myLinks[k].draw();
                        }
                    }
                }
                glTranslatef(1080.0, 0.0, 0.0);
            }
            glTranslatef(-1080.0*(width), -1080.0, 0.0);
        }

        // Resets Matrix so it draws objects starting in top left room after
        // drawing the rooms
        glLoadIdentity();
        glTranslatef(-1080.0/2.0*(width), 1080.0/2.0*(height), 0.0);

        return;
    }

    /*****************************************************************************
    * Function: World::update()
    * Description: Handles all of the back end updates to the rooms and links.
    * Avoids having to draw everything in order to update it.
    *****************************************************************************/
    void World::update(){
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                myRooms[i][j].update();
            }
        }
    }
    /*****************************************************************************
    * Function: World::compile
    * Description: Compiles the specified world file (checks the current directory
    * as well as the scripts/maps/ directory for the file) and builds the
    * individual rooms.
    *****************************************************************************/
    void World::compile(Config *myConfig, Status *myStatus) {

        std::ifstream infile;
        char *fname;
        char linebuf[256], room_str[16];
        char *linebuf_temp;
        std::vector<char *> linebuf_tok;
        uint32_t line_count;
        uint8_t size_ntok, room_ntok, budget_ntok, time_ntok;
        uint8_t rev_tok, flip_tok;
        uint16_t width_tok, height_tok, room_tok, budget_tok;
        uint32_t time_tok;
        uint16_t room_i=0, room_j=0;
        bool size_flag=false, budget_flag=false, room_flag=false;
        foz::Room *temp_room;

        /* Open and compile the map file */
        if (myConfig->debug_level > 1) {
            printf("Compiling map file %s\n", myConfig->map_fname);
        }

        /* Does the file exist in the current directory? */
        infile.open(myConfig->map_fname, std::ifstream::in);
        if (!infile) {
            if (myConfig->debug_level > 1) {
                printf("Map file %s not found in root directory, checking %s\n", myConfig->map_fname, MAP_DIR_DEFAULT);
            }

            // If not, just check in the MAP_DIR_DEFAULT directory
            fname = (char *)malloc(strlen(myConfig->map_fname)+strlen(MAP_DIR_DEFAULT)+1);
            strcpy(fname, MAP_DIR_DEFAULT);
            strcat(fname, myConfig->map_fname);
            infile.open(fname, std::ifstream::in);
            if (!infile) {
                raise_error(ERR_NOFILE3, myConfig->map_fname);
            }
        }

        // We have opened the file successfully. Set defaults, and parse through it.
        size_flag = false;
        budget_flag = false;
        myStatus->time_ms = TIME_MS_DEFAULT;
        myStatus->main_song = 1;


        for (line_count=1; !infile.eof(); line_count++) {
            infile.getline(linebuf, 256);
            strlower(linebuf);

            // If we have a '#', the line is a comment and we can skip it
            // We can also skip blank lines
            if ((linebuf[0] == 0) || (linebuf[0] == '#') || (linebuf[0] == 10) || (linebuf[0] == 13)) {
                continue;
            }

            // Check to see if we're ready to allocate rooms
            if (size_flag && room_flag) {

                if (room_i >= height) {
                    printf("Error compiling %s, line %d\n", myConfig->map_fname, line_count);
                    printf("  Expected no more than %u lines of rooms\n", height);
                    printf("  Invalid line is \'%s\'", linebuf);
                    raise_error(ERR_BADFILE3, myConfig->map_fname);
                }

                linebuf_temp = strdup(linebuf);
                linebuf_tok.push_back(strtok(linebuf_temp, " "));
                for (room_j = 1; room_j < width; room_j++) {
                    linebuf_tok.push_back(strtok(NULL, " "));
                }

                for (room_j = 0; room_j < width; room_j++) {

                    printf("linebuf_tok is %s\n", linebuf_tok[room_j]);
                    room_ntok = sscanf(linebuf_tok[room_j], " %03hu-%c%c", &room_tok, &rev_tok, &flip_tok);

                    if (room_ntok != 3) {
                        printf("Error compiling %s, line %d\n", myConfig->map_fname, line_count);
                        printf("  Invalid room specification in command \'%s\'", linebuf);
                        raise_error(ERR_BADFILE3, myConfig->map_fname);
                    }

                    /* Open and compile the room file */
                    if (myConfig->debug_level > 1) {
                        printf("Compiling room file room%03d.zrf\n", room_tok);
                    }

                    temp_room = new foz::Room;
                    temp_room->compile(room_tok, rev_tok=='r', flip_tok=='f');
                    myRooms[room_i].push_back(*temp_room);
                    delete temp_room;

                    // We are done compiling, so set up the pointers
                    if (myConfig->debug_level > 1) {
                        printf("Room file compilation complete\n");
                    }

                }

                linebuf_tok.clear();
                room_i++;
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
                continue;
            }

            // Check for .budget information
            budget_ntok = sscanf(linebuf, " .budget %hu", &budget_tok);
            if (budget_ntok == 1) {
                budget_flag = true;
                myStatus->budget = budget_tok;
                continue;
            }

            // Check for .time information
            time_ntok = sscanf(linebuf, " .time %u", &time_tok);
            if (time_ntok == 1) {
                myStatus->time_ms = 1.0*time_tok;
                continue;
            }

            // Check for the .room flag
            sscanf(linebuf, " %s", room_str);
            if (!strcmp(room_str, ".rooms")) {
                room_flag = true;
                myRooms.resize(height);
                continue;
            }

            // We've matched nothing, must be an invalid command
            printf("Error compiling %s, line %d\n", myConfig->map_fname, line_count);
            printf("  Unknown or unexpected command \'%s\'", linebuf);
            raise_error(ERR_BADFILE3, myConfig->map_fname);

        }

        // Check for required values
        if (budget_flag != true) {
            printf("Error compiling %s\n", myConfig->map_fname);
            printf("  No budget specification\n");
            raise_error(ERR_BADFILE3, myConfig->map_fname);
        }

        // We are done compiling, so set up the pointers
        if (myConfig->debug_level > 1) {
            printf("Map file compilation complete\n");
        }

        return;
    }



    /*****************************************************************************
    * Function: ~World::World()
    * Description: Simple destructor. Cleans up the room data structures.
    *****************************************************************************/
    World::~World() {

        uint16_t i;

        /* Perform a little bit of memory cleanup */
        for (i = 0; i < myRooms.size(); i++) {
            myRooms[i].clear();
        }
        myRooms.clear();

    }

} // namespace foz
