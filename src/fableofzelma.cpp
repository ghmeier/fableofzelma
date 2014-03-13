/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * fableofzelma.cpp - The Fable of Zelma application. Takes as input move files
 * and an optional map file and runs a Legend of Zelda-style simulation
 * for up to four teams.
 *
 *
 * NOTES:
 * 1/24/14 by JAZ::Design created.
 *****************************************************************************/


#include "fableofzelma.hpp"

foz::Game *myGame;

int main(int argc, char *argv[]) {

    myGame = new(std::nothrow)foz::Game(argc,argv);
    myGame->init();
    myGame->printConfig();

    myGame->initSFML();
    myGame->loadResources();

    myGame->mainLoop();

    delete myGame;
    return 0;
}

namespace foz {

    /*****************************************************************************
    * Function: Game::init()
    * Description: Initializes the game environment after compiling the team and
    * map files.
    *****************************************************************************/
    void Game::init() {

        // Set the random seed here
        srand(0);


    }


    /*****************************************************************************
    * Function: Game::reset()
    * Description: Resets the game environment
    *****************************************************************************/
    void Game::reset() {

    }

    /*****************************************************************************
    * Function: Game::mainLoop()
    * Description: Runs the main update, drawing, and event processing loop.
    *****************************************************************************/
    void Game::mainLoop() {


        while (myWindow.isOpen()) {
            processEvents();
            testDraw();
            myWindow.display();
        }

        return;

    }


    /*****************************************************************************
    * Function: Game::updateGame()
    * Description: Updates all the main elements of the game.
    *****************************************************************************/
    void Game::updateGame() {


    }


    /*****************************************************************************
    * Function: Game::endGame()
    * Description: Runs the end game sequence.
    *****************************************************************************/
    void Game::endGame() {



    }



    /*****************************************************************************
    * Function: Game::Game()
    * Description: Constructor for foz::Game() class. Reads in and parses the
    * input command line in order to set configuration data
    *****************************************************************************/
    Game::Game(int argc, char **argv) {
        int8_t i, nfiles=1;

        /* We require at least two arguments on the command line */
        if (argc < 2) {
            raise_error(ERR_USAGE, NULL);
        }

        /* If the last command-line argument is -h or --help that is ok */
        if (!strncmp(argv[argc-1], "-h", 2) || !strncmp(argv[argc-1], "--help", 6)) {
            print_help();
        }

        /* Otherwise, either the last or the last four arguments should be files */
        if (argv[argc-1][0] == '-') {
            raise_error(ERR_USAGE, NULL);
        }
        else {
            if (argc > 2) {
                if ((argv[argc-2][0] == '-') || (argv[argc-3][0] == '-')) {
                    nfiles = 1;
                }
                else {
                    if (argc < 5) {
                        raise_error(ERR_USAGE, NULL);
                    }
                    else if ((argv[argc-2][0] == '-') || (argv[argc-3][0] == '-') ||
                             (argv[argc-4][0] == '-')) {
                        raise_error(ERR_USAGE, NULL);
                    }
                    else {
                        nfiles = 4;
                    }
                }
            }
            else {
                nfiles = 1;
            }
        }

        /* Set the default values */
        myConfig.debug_level = DEBUG_DEFAULT;
        myConfig.screen_height = SCREEN_HEIGHT_DEFAULT;
        myConfig.screen_width = SCREEN_WIDTH_DEFAULT;
        myConfig.screen_depth = SCREEN_DEPTH_DEFAULT;
        myConfig.map_fname = (char *)malloc(strlen(MAP_FNAME_DEFAULT)+1);

        if (!myConfig.map_fname) {
            raise_error(ERR_NOMEM, NULL);
        }
        strcpy(myConfig.map_fname, MAP_FNAME_DEFAULT);
        for (i = 0; i < 4; i++) {
            myConfig.team_fname[i] = (char *)malloc(strlen(TEAM_FNAME_DEFAULT)+1);
            if (!myConfig.team_fname[i]) {
                raise_error(ERR_NOMEM, NULL);
            }
            strcpy(myConfig.team_fname[i], TEAM_FNAME_DEFAULT);
        }

        for (i = 1; i < argc-nfiles; i++) {
            /* Arguments must begin with '-' or '--' */
            if (argv[i][0] != '-' || strlen(argv[i]) < 2) {
                raise_error(ERR_USAGE, NULL);
            }

            /* Decode the single dash arguments */
            if (argv[i][1] != '-') {
                /* Single dash arguments should have just one character after the dash */
                if (strlen(argv[i]) > 2) {
                    raise_error(ERR_USAGE, NULL);
                }
                switch (argv[i][1]) {
                    case 'H':
                        i++;
                        sscanf(argv[i], "%hu", &myConfig.screen_height);
                        break;
                    case 'W':
                        i++;
                        sscanf(argv[i], "%hu", &myConfig.screen_width);
                        break;
                    case 'D':
                        i++;
                        sscanf(argv[i], "%d", &myConfig.debug_level);
                        break;
                    case 'M':
                        i++;
                        myConfig.map_fname = (char *)realloc(myConfig.map_fname, strlen(argv[i])+1);
                        if (!myConfig.map_fname) {
                            raise_error(ERR_NOMEM, NULL);
                        }
                        strcpy(myConfig.map_fname, argv[i]);
                        break;
                    case 'h':
                        print_help();
                        break;
                    default:
                        raise_error(ERR_USAGE, NULL);
                        break;
                }
            }

            /* Decode the double dash arguments */
            else {
                if (!strncmp(argv[i], "--height", 8)) {
                    i++;
                    sscanf(argv[i], "%hu", &myConfig.screen_height);
                }
                else if (!strncmp(argv[i], "--width", 7)) {
                    i++;
                    sscanf(argv[i], "%hu", &myConfig.screen_width);
                }
                else if (!strncmp(argv[i], "--debug", 7)) {
                    i++;
                    sscanf(argv[i], "%d", &myConfig.debug_level);
                }
                else if (!strncmp(argv[i], "--map", 5)) {
                    i++;
                    myConfig.map_fname = (char *)realloc(myConfig.map_fname, strlen(argv[i])+1);
                    if (!myConfig.map_fname) {
                        raise_error(ERR_NOMEM, NULL);
                    }
                    strcpy(myConfig.map_fname, argv[i]);
                }
                else if (!strncmp(argv[i], "--help", 6)) {
                    print_help();
                }
                else {
                    raise_error(ERR_USAGE, NULL);
                }
            }
        }

        /* The last nfiles command arguments are the input file names */
        for (i = argc-nfiles; i < argc; i++) {
            myConfig.team_fname[i-(argc-nfiles)] =
                (char *)realloc(myConfig.team_fname[i-(argc-nfiles)], strlen(argv[i])+1);
            if (!myConfig.team_fname[i-(argc-nfiles)]) {
                raise_error(ERR_NOMEM, NULL);
            }
            strcpy(myConfig.team_fname[i-(argc-nfiles)], argv[i]);
        }

        return;
    }



















    /*****************************************************************************
    * Function: Game::~Game()
    * Description: Destructor for fvu::Game() class
    *****************************************************************************/
    Game::~Game() {
    }


    Room::Room(){

    }

    Room::Room(int x,int y){
        yTiles = x;
        xTiles = y;
    }

    Room::~Room(){
    }

    int Room::getXTiles() {
        return xTiles;
    }

    int Room::getYTiles() {
        return yTiles;
    }

    void Room::drawSelf() {
        //Jonathan your implementation can go here
    }

    void Room:: setLayout(int* lay) {
        layout = lay;
    }


} //namespace foz    test
