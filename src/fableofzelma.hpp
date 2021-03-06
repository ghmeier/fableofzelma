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

#define GLOBALHEIGHT 64.0
#define GLOBALWIDTH 58.0

typedef enum {DEMO_START=0, DEMO_MID, DEMO_END, GAME_START, GAME_MID, GAME_END, GAME_RESET, GAME_PAUSE, GAME_MID_NODRAW} MODE_TYPE;

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
            //reference to each team's .zuf file
            char *team_fname[4];
            //filepath to world map
            char *map_fname;
            //enemy command files
            char *enemy_fname[NUM_ENEMIES];
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
            bool has_link;
            uint8_t pred;
            uint16_t link_pred;
            uint16_t link;
            uint8_t cmd;
            uint32_t line;
            uint16_t opt[2];
    };

    class Link {
        public:
            Link(uint8_t type, uint16_t myid, uint8_t myteam, uint8_t world_width, uint8_t world_height);
            Link(uint16_t type, float x, float y);
            ~Link(){};
            void update(uint8_t cmd);
            void draw();
            void doDamage(int16_t amount);

            bool active;            //draws the link when true, and ignores when false
            bool can_move;          //used to determine when link is colliding with anything
            uint8_t type;           //determines which type of link it is, CURRENTLY - only 0
            uint8_t direction;      //links directional facing, corresponds to the ENUM
            uint8_t team;           //reference to links team (0-3) corresponds with loaded teams
            uint8_t status;         //only saving this in the event that link gets status effects
            int16_t health;         //link's integer health value
            int16_t damage;         //the amount link takes down enemy's health when attacking
            uint16_t sprite;        //current sprite being drawn to the screen, see link_object ENUM
            uint32_t depth;         //used for layering
            uint8_t texfile;        //which texture link is using, red, green, blue, purple
            float width, height;    //in pixels. determines links size both visually and in collisions
            uint16_t id;            //link's identifier on the team (not sure if this works yet)
            uint8_t room_x, room_y; //these are used to place link in a room
            float x, y;             //link's coordinate position within the room
            float speed;            //amount link can move per frame, currently moves him ~59 pixels per move call
            uint8_t frameCount;     //modulates the way sprites change on link to slow them down to normal speed
            uint16_t numKeys;       //number of keys to open chests that link has
            bool wasHitLast;        //true if the link was hit by an enemy's last command
            std::vector<cmd_type> commands; //All of this link's commands as well as labels and goto's
            uint16_t cur_cmd;
            uint8_t cur_cmdframe;
    };

    /* Enemy extends Link and is used for any enemy objects */
    class Enemy : public Link{
        public:
            Enemy(uint16_t mytype, float myx, float myy);
            Enemy(uint16_t mytype, float myx, float myy,int16_t mySubject);
            void compileEnemies();
            void update(uint8_t cmd);
            void draw();
            void doDamage(int16_t amount);
            ~Enemy();

            uint16_t cmdIter;
            uint16_t cur_cmdframe;
            bool cmds_done;
            uint16_t cur_cmdopt;
            uint16_t subject;
    };

    /* Room information */
    class Room {
      public:
        uint16_t id;
        uint16_t width, height;
        std::vector< std::vector<uint16_t> > myTiles;
        std::vector<Object> myObjects;
        std::vector<Enemy> myEnemies;

        /* Main functions (room.cpp) */
        void compile(uint16_t id, bool rev, bool flip);
        void draw();
        void updateEnemies();
        void update();
        ~Room();
    };

    /* Status structure that contains score and other details */
    class Status {
        public:
            MODE_TYPE mode;
            int16_t scores[4];
            float time_ms;
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

        void compile(Config *myConfig, Status *myStatus);
        void draw();
        void update();
        ~World();
    };

    /* Per-team state information */
    class Team {
        public:
            Team(){}
            ~Team(){cmds.clear();myLinks.clear();}
            uint8_t id;
            uint32_t status;
            char *name;
            uint16_t score;
            uint16_t prevScore;
            uint16_t budget;
            uint16_t cur_cmd;
            uint16_t cur_link;
            uint16_t cur_cmdframe;
            uint16_t cur_cmdopt;
            uint16_t link_left;
            bool cmds_done;
            std::vector<foz::cmd_type> cmds;
            std::vector<foz::Link> myLinks;
    };

    /* Camera information */
    class Camera {
      public:
        float x_left, x_right;
        float y_top, y_bottom;
        int16_t x_pan_count, y_pan_count, zoom_count;
        float x_pos, y_pos;
        float zoom_level;
        uint8_t currentTeam;
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
            void compileEnemies();
            void endGame();

            /* SFML functions (sfml_utils.cpp) */
            void initSFML();
            void loadResources();
            void drawScoreboard();
            void drawWord(std::string,float,float);
            void drawMinimap(float zoomLevel,float x, float y);
            void processEvents();

            /* Utility functions (utils.cpp) */
            void playSound(uint16_t sfxID, uint8_t vol, bool force);
            void playSound(uint16_t,uint8_t,bool,int,int);
            void printConfig();
            void printStatus();

            /* Collision helper functions*/
            bool linkColObj(Link *myLink, Object *myObject);
            bool linkColLink(Link *myLink, Link *other);
            bool objColObj(Object *myLink, Object *myObject);


            float NUMBER_WIDTH = 0;
            float NUMBER_HEIGHT = 0;
            float LETTER_WIDTH = 0;
            float LETTER_HEIGHT = 0;

             /* Globally declare Textures */
            foz::Texture myTextures[NUM_TEXTURES];

            foz::Status myStatus;
            std::vector<Link> myLinks[4];
            foz::Team myTeams[4];
            std::vector< std::vector<foz::cmd_type> > enemyCommands;

        private:
            foz::Config myConfig;
            foz::World myWorld;
            foz::Camera myCamera;
            sf::Time myTime;
            uint32_t framecount;
            sf::ContextSettings mySettings;
            sf::RenderWindow myWindow;
            sf::Music myMusic[2];
            sf::SoundBuffer mySoundBuffers[NUM_SFX];
            sf::Sound mySounds[NUM_SOUNDS];
            sf::Clock myClock;
    };

} // namespace foz
extern foz::Game *myGame;


