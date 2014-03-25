
#ifndef LINKS_H
#define LINKS_H

/*Link Types Enum*/
#define NUM_LINK 1
#define NUM_LINK_SPELLINGS 1
typedef enum {REGULAR=0} LINK_TYPE;
std::string linkNames[NUM_LINK][NUM_LINK_SPELLINGS];

/*Command Enum*/
#define NUM_CMD_SPELLINGS 1
typedef enum {SELECT_CMD=0, PLACE_CMD, MOVE_CMD, LEFT_CMD, RIGHT_CMD, GOTO_CMD, NUM_CMD_TYPES} CMD_TYPE;
std::string cmdNames[NUM_CMD_TYPES][NUM_CMD_SPELLINGS];

namespace foz {

    class Link
    {
         public:
                Link(uint8_t type, uint16_t myid){};
                ~Link(){};
                void update();
                void place(uint8_t team, uint16_t row, uint16_t col);
                void updateTransition(uint16_t val);
                void draw(uint16_t index);
                uint8_t getStatus() {return status;}
                uint16_t getID() {return id;}
                uint8_t getType() {return type;}
                uint16_t getRow() {return row;}
                int16_t getHealth() {return health;}
                void setStatus(uint8_t val) {status = val;}
                void setHealth(int16_t myhealth) {health = myhealth;}
                void setRow(uint16_t myrow) {row = myrow;}
                void setCol(uint16_t mycol) {col = mycol;}
                uint16_t getCol() {return col;}
                uint8_t getTeam() {return team;}
                float getGameX() {return game_x;}
                float getGameY() {return game_y;}
                float getDir() {return dir;}
                void move();
                uint16_t action_count;

            private:
                uint8_t type;
                uint8_t status;
                int16_t health;
                uint16_t id;
                uint16_t row, col;
                float speed;
                float game_x, game_y, demo_x, demo_y;
                float dir;
                uint8_t team;
                uint16_t delay;
                uint16_t move_count;
    };

}

#endif // LINKS_H
