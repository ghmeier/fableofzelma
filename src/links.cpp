#include "fableofzelma.hpp"

std::string linkNames[NUM_LINK][NUM_LINK_SPELLINGS] = {
    {"regular"}
    };

/* Command spellings */
std::string cmdNames[NUM_CMD_TYPES][NUM_CMD_SPELLINGS] = {
    {"select"},
    {"move"},
    {"left"},
    {"right"},
    {"goto"}
    };
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
namespace foz{
    Link::Link(uint8_t type, uint16_t myid) {
        id = myid;
    };


}
