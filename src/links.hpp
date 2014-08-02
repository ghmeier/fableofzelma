/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * links.hpp - FableofZelma application. This class contains the
 * enumerated types and class declaration for the main link class.
 *
 *
 * NOTES:
 * 4/04/14 by JAZ::Design created.
 *****************************************************************************/


#pragma once

#include "fableofzelma.hpp"

/* Main enum for the link types. We specify NUM_SPELLINGS to some reasonable amount to minimize
 * the number of compile errors */
#define NUM_LINK_SPELLINGS 4
typedef enum {REGULAR_LINK=0, ARROW_LINK, HEAVY_LINK, BOMB_LINK, NAVI_LINK, NUM_LINK_TYPE} LINK_TYPE;
extern std::string linkNames[NUM_LINK_TYPE][NUM_LINK_SPELLINGS];

namespace foz {



}

