#include "embfetch_config.h"

#ifndef LOGO_H
#define LOGO_H

#define ANSI_RED    "\033[31m"
#define ANSI_GREEN  "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_BLUE  "\033[34m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_CYAN   "\033[36m"
#define ANSI_RESET "\033[0m"
#define NULL ((const char *)0)

#if defined(EMBFETCH_FREERTOS)

#define LOGO_INFO_COLUMN "\033[40G"
const char *logo[] = {
    ANSI_GREEN "----------------------------------" ANSI_RESET,
    ANSI_GREEN "|  " ANSI_RESET "  _____ _______ ____   _____  " ANSI_GREEN "|" ANSI_RESET,
    ANSI_GREEN "| f" ANSI_RESET " |  __ \\__   __/ __ \\ / ____| " ANSI_GREEN "|" ANSI_RESET,
    ANSI_GREEN "| r" ANSI_RESET " | |__> | | | | |  | | <___   " ANSI_GREEN "|" ANSI_RESET,
    ANSI_GREEN "| e" ANSI_RESET " |  _  /  | | | |  | |\\___ \\  " ANSI_GREEN "|" ANSI_RESET,
    ANSI_GREEN "| e" ANSI_RESET " | | \\ \\  | | | |__| |____> | " ANSI_GREEN "|" ANSI_RESET,
    ANSI_GREEN "|  " ANSI_RESET " |_|  \\_\\ |_|  \\____/|_____/  " ANSI_GREEN "|" ANSI_RESET,
    ANSI_GREEN "|  " ANSI_RESET "                              " ANSI_GREEN "|" ANSI_RESET,
    ANSI_GREEN "----------------------------------" ANSI_RESET,
    NULL
};

#elif defined(EMBFETCH_ESP_IDF)

#define LOGO_INFO_COLUMN "\033[50G"
const char *logo[] = {
    ANSI_RED "                   <<<<<<<<<~           " ANSI_RESET,
    ANSI_RED "      ~    <<<<<<!   <<<<<<<<<<<        " ANSI_RESET,
    ANSI_RED "    ~<<< <<<<<<<<<<<<=   <<<<<<<<<      " ANSI_RESET,
    ANSI_RED "   <<<t =<<<<<<<<<<<<<<<<  <<<<<<<<<    " ANSI_RESET,
    ANSI_RED "  <<<     =<<<<<<<<<<<<<<<<<  <<<<<<<   " ANSI_RESET,
    ANSI_RED " <<<    <<<<<    <<<<<<<<<<<<< <<<<<<<! " ANSI_RESET,
    ANSI_RED "=<<  =<<<<<<<<<<<<  !<<<<<<<<<<  <<<<<< " ANSI_RESET,
    ANSI_RED "<<< <<<<<<<<<<<<<<<<<I <<<<<<<<<< !<<<<<" ANSI_RESET,
    ANSI_RED "<< <<<<<<<<<<<<<<<<<<<<! <<<<<<<<< !<<<<" ANSI_RESET,
    ANSI_RED "<< <<<<<<<   t<<<<<<<<<<<! <<<<<<<<  <<<" ANSI_RESET,
    ANSI_RED "<< <<<<<<<<<<<   ><<<<<<<<= <<<<<<<< <<<" ANSI_RESET,
    ANSI_RED "<<  <<<<<<<<<<<<<~ <<<<<<<<< <<<<<<<<   " ANSI_RESET,
    ANSI_RED "<<<  <<<<<<<<<<<<<<  <<<<<<<= <<<<<<<!  " ANSI_RESET,
    ANSI_RED "<<<      !<<<<<<<<<<! <<<<<<<  <<<<<<<  " ANSI_RESET,
    ANSI_RED " <<<         <<<<<<<< !<<<<<<< <<<<<<<  " ANSI_RESET,
    ANSI_RED "  <<<  !<<<<<  <<<<<<= <<<<<<< <<<<<<=  " ANSI_RESET,
    ANSI_RED "   <<< <<<<<<  <<<<<<< <<<<<<< <<<<<    " ANSI_RESET,
    ANSI_RED "    =<< <<<<<  <<<<<<< <<<<<<<          " ANSI_RESET,
    ANSI_RED "      <<<=    !<<<<<<< <<<<<<<   !<<<<  " ANSI_RESET,
    ANSI_RED "       i<<<<   <<<<<<  ~<<<<<  <<<<=    " ANSI_RESET,
    ANSI_RED "          <<<<<<<<       <<<<<<<<       " ANSI_RESET,
    ANSI_RED "              <<<<<<<<<<<<<<<~          " ANSI_RESET,
    NULL
};

#elif defined(EMBFETCH_ZEPHYR)

#define LOGO_INFO_COLUMN "\033[50G"
const char *logo[] = {
    ANSI_BLUE "                            =:::::::------=  " ANSI_RESET,
    ANSI_BLUE "                           ++++-:-------=++  " ANSI_RESET,
    ANSI_BLUE "                           ++++++=--==+++++  " ANSI_RESET,
    ANSI_BLUE "                          +++++++++++++++++  " ANSI_RESET,
    ANSI_BLUE "                         =++++++++****+++++  " ANSI_RESET,
    ANSI_BLUE "                         +++++++**##***++++  " ANSI_RESET,
    ANSI_BLUE "                        ++++++*#####*****++  " ANSI_RESET,
    ANSI_BLUE "                        ++++*#######******+  " ANSI_RESET,
    ANSI_MAGENTA " ======                " ANSI_BLUE "+++*########*****     " ANSI_RESET,
    ANSI_MAGENTA "  ====                 " ANSI_BLUE "+*##****+             " ANSI_RESET,
    ANSI_MAGENTA "    ==      ====    **+                      " ANSI_RESET,
    ANSI_MAGENTA "     -+++++======+###*                       " ANSI_RESET,
    ANSI_MAGENTA "      ++++          **                       " ANSI_RESET,
    NULL
};

#elif defined(EMBFETCH_NRF_CONNECT)

#define LOGO_INFO_COLUMN "\033[50G"
const char *logo[] = {
    ANSI_CYAN "       ++++++              :::+++       " ANSI_RESET,
    ANSI_CYAN "    ++++++++++++       :::::::++++++    " ANSI_RESET,
    ANSI_CYAN " +++++++++++++++++++::::::::::++++++++++" ANSI_RESET,
    ANSI_CYAN "-=+++++++++++++++++++=::::::::++++++++++" ANSI_RESET,
    ANSI_CYAN "::::=+++++++++++++++++++=:::::++++++++++" ANSI_RESET,
    ANSI_CYAN ":::::::=++++++++++++++++++++-:++++++++++" ANSI_RESET,
    ANSI_CYAN "::::::::::=+++++++++++++++++++++++++++++" ANSI_RESET,
    ANSI_CYAN ":::::::::::::-++++++++++++++++++++++++++" ANSI_RESET,
    ANSI_CYAN "::::::::::::::::-=++++++++++++++++++++++" ANSI_RESET,
    ANSI_CYAN ":::::::::::::::::::-=+++++++++++++++++++ " ANSI_RESET,
    ANSI_CYAN ":::::::::::::::::::::::=++++++++++++++++" ANSI_RESET,
    ANSI_CYAN "::::::::::::::::::::::::::=+++++++++++++" ANSI_RESET,
    ANSI_CYAN ":::::::::::::::::::::::::::::-++++++++++" ANSI_RESET,
    ANSI_CYAN "::::::::::==::::::::::::::::::::-=++++++" ANSI_RESET,
    ANSI_CYAN "::::::::::=+++=-::::::::::::::::::::=+++" ANSI_RESET,
    ANSI_CYAN "::::::::::=+++++++-::::::::::::::::::::=" ANSI_RESET,
    ANSI_CYAN " :::::::::=+++++++++ :::::::::::::::::: " ANSI_RESET,
    ANSI_CYAN "    ::::::=+++++        ::::::::::::    " ANSI_RESET,
    ANSI_CYAN "       :::=++              ::::::       " ANSI_RESET,
    NULL
};

#elif defined(EMBFETCH_RT_THREAD)

#define LOGO_INFO_COLUMN "\033[50G"
const char *logo[] = {
    ANSI_BRIGHT_CYAN "     =+++++++++++++++++++=                   " ANSI_RESET,
    ANSI_CYAN "  *" ANSI_BRIGHT_CYAN "+++++++++++++++++++++++++++++             " ANSI_RESET,
    ANSI_CYAN " #**" ANSI_BRIGHT_CYAN "+++++++++++++++++++++++++++++++          " ANSI_RESET,
    ANSI_CYAN "###" ANSI_BRIGHT_CYAN "++++=++++                 ++++++++++      " ANSI_RESET,
    ANSI_BRIGHT_CYAN "                                    =++++    " ANSI_RESET,
    ANSI_BRIGHT_CYAN "                                       ++=+  " ANSI_RESET,
    ANSI_BRIGHT_CYAN "                                           ++" ANSI_RESET,
    ANSI_BRIGHT_CYAN "                                             " ANSI_RESET,
    ANSI_BRIGHT_CYAN "                                             " ANSI_RESET,
    NULL
};

#elif defined(EMBFETCH_NUTTX)

#define LOGO_INFO_COLUMN "\033[50G"
const char *logo[] = {
    ANSI_BLUE "                  #***                  " ANSI_RESET,
    ANSI_BLUE "                #*******                " ANSI_RESET,
    ANSI_BLUE "              #***********              " ANSI_RESET,
    ANSI_BLUE "       " ANSI_RESET "---- " ANSI_BLUE "#*****" ANSI_RESET "+==+" ANSI_BLUE "*****+" ANSI_RESET "  ----=" ANSI_BLUE "     " ANSI_RESET,
    ANSI_BLUE "      " ANSI_RESET "-:::=" ANSI_BLUE "*******" ANSI_RESET "=:::+" ANSI_BLUE "******" ANSI_RESET "=-::=*#%" ANSI_BLUE "   " ANSI_RESET,
    ANSI_BLUE "      " ANSI_RESET "-::::=" ANSI_BLUE "#*****" ANSI_RESET "=::::+#" ANSI_BLUE "****" ANSI_RESET "-::-*##    " ANSI_RESET,
    ANSI_BLUE "      " ANSI_RESET "-:::::=" ANSI_BLUE "#****" ANSI_RESET "=:::::+#" ANSI_BLUE "**" ANSI_RESET "-::-*##     " ANSI_RESET,
    ANSI_BLUE "    #*" ANSI_RESET "+::::::=" ANSI_BLUE "#***" ANSI_RESET "=::::::+*-::-*##" ANSI_BLUE "**    " ANSI_RESET,
    ANSI_BLUE "  #***" ANSI_RESET "+:::=-::=" ANSI_BLUE "#**" ANSI_RESET "=::-=:::-::-*##" ANSI_BLUE "*****  " ANSI_RESET,
    ANSI_BLUE "#*****" ANSI_RESET "+:::*+-::=" ANSI_BLUE "#*" ANSI_RESET "=::-" ANSI_BLUE "*" ANSI_RESET "+::::-##" ANSI_BLUE "*********" ANSI_RESET,
    ANSI_BLUE "+*****" ANSI_RESET "+:::*" ANSI_BLUE "#" ANSI_RESET "+:::=" ANSI_BLUE "*" ANSI_RESET "=::-" ANSI_BLUE "**" ANSI_RESET "-:::=##" ANSI_BLUE "********#" ANSI_RESET,
    ANSI_BLUE "  ****" ANSI_RESET "+:::*" ANSI_BLUE "#*" ANSI_RESET "+:::==::-" ANSI_BLUE "+" ANSI_RESET "-:::::=#" ANSI_BLUE "*******  " ANSI_RESET,
    ANSI_BLUE "    **" ANSI_RESET "+:::*" ANSI_BLUE "#**" ANSI_RESET "+-::-::-:::-+:::=#" ANSI_BLUE "***#    " ANSI_RESET,
    ANSI_BLUE "      " ANSI_RESET "-:::*" ANSI_BLUE "#***" ANSI_RESET "+-:::::::-##+:::=##      " ANSI_RESET,
    ANSI_BLUE "      " ANSI_RESET "-:::*" ANSI_BLUE "#****" ANSI_RESET "+-:::::-*#" ANSI_BLUE "**" ANSI_RESET "+:::=#      " ANSI_RESET,
    ANSI_BLUE "      " ANSI_RESET "-:::*" ANSI_BLUE "#*****" ANSI_RESET "+-:::=*#" ANSI_BLUE "****" ANSI_RESET  "=:::+#%    " ANSI_RESET,
    ANSI_BLUE "      " ANSI_RESET "-::-*" ANSI_BLUE "#******" ANSI_RESET "+--=*#" ANSI_BLUE "***#  " ANSI_RESET "---+##" ANSI_BLUE "    " ANSI_RESET,
    ANSI_BLUE "      " ANSI_RESET "%\\###" ANSI_BLUE "  *******##**#        " ANSI_RESET "%\\#%" ANSI_BLUE "     " ANSI_RESET,
    ANSI_BLUE "                *******#                " ANSI_RESET,
    ANSI_BLUE "                  ***#                  " ANSI_RESET,
    NULL
};

#elif defined(EMBFETCH_THREADX)

#define LOGO_INFO_COLUMN "\033[40G"
const char *logo[] = {
    ANSI_YELLOW "         ************         " ANSI_RESET,
    ANSI_YELLOW "     *******************      " ANSI_RESET,
    ANSI_YELLOW "    **********************    " ANSI_RESET,
    ANSI_YELLOW "  *****    *******     *****  " ANSI_RESET,
    ANSI_YELLOW " ***          **     ******** " ANSI_RESET,
    ANSI_YELLOW "**    " ANSI_RESET "-------" ANSI_YELLOW "      ***********" ANSI_RESET,
    ANSI_YELLOW "*  " ANSI_RESET "---------------" ANSI_YELLOW "   *********" ANSI_RESET,
    ANSI_RESET " ----------      ----  " ANSI_YELLOW "*******" ANSI_RESET,
    ANSI_RESET " --------    ---     --- " ANSI_YELLOW "*****" ANSI_RESET,
    ANSI_RESET "-------     ------     --- " ANSI_YELLOW "***" ANSI_RESET,
    ANSI_RESET "--------------------------- " ANSI_YELLOW "**" ANSI_RESET,
    ANSI_RESET " ----------------------------" ANSI_RESET,
    ANSI_RESET "  --------------------------  " ANSI_RESET,
    ANSI_RESET "    ----------------------    " ANSI_RESET,
    ANSI_RESET "      ------------------      " ANSI_RESET,
    ANSI_RESET "         ------------         " ANSI_RESET,
    NULL
};

#elif defined(EMBFETCH_KEIL_RTX5)

#define LOGO_INFO_COLUMN "\033[40G"
const char *logo[] = {
    ANSI_GREEN "**  *******************************" ANSI_RESET,
    ANSI_GREEN "****  *****                      **" ANSI_RESET,
    ANSI_GREEN "******  *****                    **" ANSI_RESET,
    ANSI_GREEN "** *****  ****                   **" ANSI_RESET,
    ANSI_GREEN "**   ****   ****                 **" ANSI_RESET,
    ANSI_GREEN "**     *****  ****               **" ANSI_RESET,
    ANSI_GREEN "**       *****  ****             **" ANSI_RESET,
    ANSI_GREEN "**         *****  ****           **" ANSI_RESET,
    ANSI_GREEN "**           *****  ****         **" ANSI_RESET,
    ANSI_GREEN "**           *****  *****        **" ANSI_RESET,
    ANSI_GREEN "**         *****  *****          **" ANSI_RESET,
    ANSI_GREEN "**       *****  *****            **" ANSI_RESET,
    ANSI_GREEN "**     *****  *****              **" ANSI_RESET,
    ANSI_GREEN "**    ****  *****                **" ANSI_RESET,
    ANSI_GREEN "** *****  *****                  **" ANSI_RESET,
    ANSI_GREEN "******  *****                    **" ANSI_RESET,
    ANSI_GREEN "****  *****                      **" ANSI_RESET,
    ANSI_GREEN "**  *******************************" ANSI_RESET,
    NULL
};

#elif defined(EMBFETCH_KEIL_RTX4)

#define LOGO_INFO_COLUMN "\033[40G"
const char *logo[] = {
    ANSI_CYAN "**  *******************************" ANSI_RESET,
    ANSI_CYAN "****  *****                      **" ANSI_RESET,
    ANSI_CYAN "******  *****                    **" ANSI_RESET,
    ANSI_CYAN "** *****  ****                   **" ANSI_RESET,
    ANSI_CYAN "**   ****   ****                 **" ANSI_RESET,
    ANSI_CYAN "**     *****  ****               **" ANSI_RESET,
    ANSI_CYAN "**       *****  ****             **" ANSI_RESET,
    ANSI_CYAN "**         *****  ****           **" ANSI_RESET,
    ANSI_CYAN "**           *****  ****         **" ANSI_RESET,
    ANSI_CYAN "**           *****  *****        **" ANSI_RESET,
    ANSI_CYAN "**         *****  *****          **" ANSI_RESET,
    ANSI_CYAN "**       *****  *****            **" ANSI_RESET,
    ANSI_CYAN "**     *****  *****              **" ANSI_RESET,
    ANSI_CYAN "**    ****  *****                **" ANSI_RESET,
    ANSI_CYAN "** *****  *****                  **" ANSI_RESET,
    ANSI_CYAN "******  *****                    **" ANSI_RESET,
    ANSI_CYAN "****  *****                      **" ANSI_RESET,
    ANSI_CYAN "**  *******************************" ANSI_RESET,
    NULL
};

#else    /* Default fallback */

#define LOGO_INFO_COLUMN "\033[30G"
static const char *logo[] = {
    "       ______       ",
    "      / ____ \\      ",
    "     / /    \\ \\     ",
    "    | |      | |    ",
    "            / /     ",
    "           / /      ",
    "          | |       ",
    "          |_|       ",
    "          (_)       ",
    NULL
};

#endif // RTOS-specific logos

#endif /* LOGO_H */