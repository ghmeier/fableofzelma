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

namespace foz{
    Link::Link(uint8_t type, uint16_t myid) {
        id = myid;
    };


}
