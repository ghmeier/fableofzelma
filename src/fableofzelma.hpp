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
#include "links.hpp"
#include "object.hpp"

#define EXEC_NAME "fableofzelma"
#define WINDOW_TITLE "FableOfZelma, v-1.0a"

#define ERR_USAGE 1
#define ERR_SFML 2
#define ERR_NOFILE1 20
#define ERR_NOFILE2 21
#define ERR_NOFILE3 22
#define ERR_NOFILE4 23
#define ERR_NOFILE5 24
#define ERR_BADFILE1 30
#define ERR_BADFILE2 31
#define ERR_BADFILE3 32
#define ERR_BADFILE4 33
#define ERR_BADFILE5 34
#define ERR_AUDIO 10
#define ERR_OVERBUDGET 50
#define ERR_NOMEM 60
#define ERR_UNDEFINED 100

#define FRAME_RATE 60
#define NUM_SOUNDS 64
#define SCREEN_WIDTH_DEFAULT 1920
#define SCREEN_HEIGHT_DEFAULT 1080
#define SCREEN_DEPTH_DEFAULT 32
#define DEBUG_DEFAULT 0

#define CAMERA_PAN_DELTA 60
#define CAMERA_PAN_EPSILON 0.001
#define CAMERA_ZOOM_DELTA 60
#define CAMERA_ZOOM_EPSILON 0.001

#define MAP_DIR_DEFAULT "scripts/maps/"
#define ROOM_DIR_DEFAULT "scripts/rooms/"
#define TEAM_DIR_DEFAULT "scripts/users/"
#define MAP_FNAME_DEFAULT "default.zmf"
#define TEAM_FNAME_DEFAULT "default.zuf"
#define TIME_MS_DEFAULT 300000

typedef enum {DEMO_START=0, DEMO_MID, DEMO_END, GAME_START, GAME_MID, GAME_END} MODE_TYPE;

/* Function prototypes (utils.cpp) */
void strlower(char *in);
void print_help();
void raise_error(uint32_t, const char *msg);
float *getTexCoords(uint8_t texID, uint16_t spriteID, float *texCoords);


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
        std::vector<Object> myObjects;

        Room *north;
        Room *south;
        Room *west;
        Room *east;

        /* Main functions (room.cpp) */
        void compile(uint16_t id, bool rev, bool flip);
        void draw();
        ~Room();
    };

    /* Status structure that contains score and other details */
    class Status {
        public:
            MODE_TYPE mode;
            int16_t scores[4];
            float timer_ms;
            uint16_t budget;
            uint8_t main_song;
            bool end_music;
            uint8_t vol_counter;
            uint16_t music_buffer;
    };

    /* World information */
    class World {
      public:
        uint16_t width, height;
        std::vector< std::vector<Room> > myRooms;

        /* Main functions (world.cpp) */
        void compile(Config *myConfig, Status *myStatus);
        void draw();
        ~World();
    };


    /*Player types of commands*/
    class cmd_type {
        public:
            char label_str[16];
            char target_str[16];
            bool has_label;
            bool inv_pred;
            bool has_pred;
            bool has_link_pred;
            uint8_t pred;
            uint16_t link_pred;
            uint16_t link;
            uint8_t cmd;
            uint32_t line;
            uint16_t opt[2];
    };

    /* Per-team state information */
    class Team {
        public:
            uint8_t id;
            uint32_t status;
            char *name;
            uint16_t budget;
            uint16_t cur_cmd;
            uint16_t cur_link;
            uint16_t cur_cmdframe;
            uint16_t cur_cmdopt;
            bool cmds_done;
            std::vector<foz::cmd_type> cmds;

    };

    /* Camera information */
    class Camera {
      public:
        float x_left, x_right;
        float y_top, y_bottom;
        int16_t x_pan_count, y_pan_count, zoom_count;
        float x_pos, y_pos;
        float zoom_level;
        uint16_t width, height;
        CAMERA_ENUM state;

        /* Main functions (camera.cpp) */
        void init(foz::World *myWorld);
        void update(bool reposition);
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
            void compileTeams();
            void endGame();


            /* SFML functions (sfml_utils.cpp) */
            void initSFML();
            void loadResources();
            void drawScoreboard();
            void processEvents();

            /* Utility functions (utils.cpp) */
            void playSound(uint16_t sfxID, uint8_t vol, bool force);
            void printConfig();
            void printStatus();

             /* Globally declare Textures */
            foz::Texture myTextures[NUM_TEXTURES];
            foz::Status myStatus;
            std::vector<Link> myLinks[4];
            foz::Team myTeams[4];

        private:
            foz::Config myConfig;
            foz::World myWorld;
            foz::Camera myCamera;
            sf::Time myTime;
            uint32_t framecount;
            sf::ContextSettings mySettings;
            sf::RenderWindow myWindow;
            sf::Music myMusic[2];
            sf::SoundBuffer mySoundBuffers[5];
            sf::Sound mySounds[6];
            sf::Clock myClock;
    };


     class Drawable {
        public:
            virtual void  drawSelf();
    };

} // namespace foz


extern foz::Game *myGame;

