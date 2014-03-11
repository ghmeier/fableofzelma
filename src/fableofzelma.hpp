/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * fableofzelma.cpp - The Fable of Zelda application. Takes as input move files
 * and an optional map file and runs a Legend of Zelda-style simulation
 * for up to four teams.
 *
 *
 * NOTES:
 * 1/24/14 by JAZ::Design created.
 *****************************************************************************/


#pragma once



#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include "resources.hpp"

#define EXEC_NAME "fableofzelma"
#define WINDOW_TITLE "FableOfZelma, v-1.0a"

#define ERR_USAGE 1
#define ERR_SFML 2
#define ERR_NOFILE1 3
#define ERR_NOFILE2 4
#define ERR_NOFILE3 5
#define ERR_BADFILE1 6
#define ERR_BADFILE2 7
#define ERR_BADFILE3 8
#define ERR_BADFILE4 9
#define ERR_AUDIO 10
#define ERR_OVERBUDGET 20
#define ERR_NOMEM 50
#define ERR_UNDEFINED 100

#define FRAME_RATE 60
#define NUM_SOUNDS 64
#define SCREEN_WIDTH_DEFAULT 1920
#define SCREEN_HEIGHT_DEFAULT 1080
#define SCREEN_DEPTH_DEFAULT 32
#define DEBUG_DEFAULT 0

#define MAP_DIR_DEFAULT "scripts/maps/"
#define ROOM_DIR_DEFAULT "scripts/rooms/"
#define TEAM_DIR_DEFAULT "scripts/users/"
#define MAP_FNAME_DEFAULT "default.zmf"
#define TEAM_FNAME_DEFAULT "default.zuf"


/* Function prototypes (utils.cpp) */
void strlower(char *in);
void print_help();
void raise_error(uint32_t, const char *msg);


namespace foz {


    /* Game configuration information */
    class Config {
        public:
            int32_t debug_level;
            uint16_t screen_width;
            uint16_t screen_height;
            uint8_t screen_depth;
            char *team_fname[4];
            char *map_fname;
    };


    /* Room information */
    class Room {
      public:
        uint16_t id;
        uint16_t width, height;
        std::vector< std::vector<uint16_t> > myTiles;

        Room *north;
        Room *south;
        Room *west;
        Room *east;

        /* Main functions (room.cpp) */
        void compile(uint16_t id, bool rev, bool flip);
        void draw();
        ~Room();

    };



    /* World information */
    class World {
      public:
        uint16_t width, height;
        std::vector< std::vector<Room> > myRooms;

        /* Main functions (world.cpp) */
        void compile(Config myConfig);
        void draw();
        ~World();
    };


    /* Per-team state information */
    class Team {
        public:
            uint32_t status;
            char *name;
            uint16_t budget;
            int32_t timer_ms;
            uint16_t cur_cmd;
            bool cmds_done;
//            std::vector<foz::cmd_type> cmds;
    };


    /* Texture structure, so that we can more easily swap textures out */
    class Texture {
        public:
            uint32_t width;
            uint32_t height;
            float scale;
            GLuint texHandle;
            uint16_t (*spriteMap)[4];
    };

    /* Status structure that contains score and other details */
    class Status {
        public:
            int16_t scores[4];
            float time_ms;
            uint16_t budget;
            uint8_t main_song;
            bool day;
            bool end_music;
            uint8_t vol_counter;
            uint16_t music_buffer;
    };


    /* Main Game class */
    class Game {
        public:
            /* Main functions (fableofzelma.cpp) */
            Game(int argc, char **argv);
            ~Game();
            void init();
            void reset();
            void mainLoop();
            void updateGame();
            void restartGame();
            void endGame();

            /* SFML functions (sfml_utils.cpp) */
            void initSFML();
            void loadResources();
            void drawScoreboard();
            void processEvents();
            void testDraw();

            /* Utility functions (utils.cpp) */
            float *getTexCoords(uint8_t texID, uint16_t spriteID, float *texCoords);
            void playSound(uint16_t sfxID, uint8_t vol, bool force);
            void printConfig();
            void printStatus();

             /* Globally declare Textures */
            foz::Texture myTextures[NUM_TEXTURES];

        private:
            foz::Config myConfig;
            foz::World myWorld;
            sf::Time myTime;
            uint32_t framecount;
            sf::ContextSettings mySettings;
            sf::RenderWindow myWindow;
            sf::Music myMusic[2];
            sf::SoundBuffer mySoundBuffers[5];
            sf::Sound mySounds[6];
            sf::Clock myClock;
    };


} // namespace foz


extern foz::Game *myGame;

