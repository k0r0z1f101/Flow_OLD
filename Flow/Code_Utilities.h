#pragma once
#include <iostream>
#include <string>
#include <array>
#include <time.h>
#include <random>
#include <iomanip>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#include "conio.h"
#else
#include <unistd.h>
#include <ncurses.h>
#endif

using namespace std;

namespace BdB
{
    // Reference: ANSI ESC code convention -> https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797

    // CMD
    string ESC = "\x1b[";

    // CURSOR
    string HOME    = "H";
    string UP      = "1A";
    string DOWN    = "1B";
    string RGHT    = "1C";
    string LEFT    = "1D";
    string NEXT_L  = "1E"; // go to beginning of NEXT line
    string PREV_L  = "1F"; // go to beginning of PREVIOUS line

    string SAVE    = "s";  // save current position
    string LOAD    = "u";  // restore last position

    // MODE
    string BOLD    = "1;"; // accentuer -> OK
    string FAINT   = "2;"; // atténuer -> KO
    string ITALC   = "3;"; // -> KO
    string ULINE   = "4;"; // soulign� -> OK
    string BLINK   = "5;"; // -> KO
    string REVRS   = "7;"; // reverse FGRND/BGRND color -> OK
    string SWTCH   = "8;"; // show/hide -> KO
    string STRKE   = "9;"; // barr� -> KO

    // ERASE
    string ERASE_DWN = "0J\r";   // efface du curseur à la fin de l'écran
    string ERASE_UP  = "1J\r";   // efface du curseur au début de l'écran
    string ERASE_ALL = "2J";
    string ERASE_L   = "2K\r"; // efface toute la ligne

    // COLOR
    string RESET_ALL   = "\x1b[0m";
    string RESET_COLOR = "\x1b[39m";

    // normal text
    string Black       = "30m";
    string Red         = "31m";
    string Green       = "32m";
    string Yellow      = "33m";
    string Blue        = "34m";
    string Magenta     = "35m";
    string Cyan        = "36m";
    string White       = "37m";

    // Light text
    string LBlack      = "90m";
    string LRed        = "91m";
    string LGreen      = "92m";
    string LYellow     = "93m";
    string LBlue       = "94m";
    string LMagenta    = "95m";
    string LCyan       = "96m";
    string LWhite      = "97m";

    // normal BG
    string BG_Black    = "40m";
    string BG_Red      = "41m";
    string BG_Green    = "42m";
    string BG_Yellow   = "43m";
    string BG_Blue     = "44m";
    string BG_Magenta  = "45m";
    string BG_Cyan     = "46m";
    string BG_White    = "47m";

    // Light BG
    string BG_LBlack    = "40;1m";
    string BG_LRed      = "41;1m";
    string BG_LGreen    = "42;1m";
    string BG_LYellow   = "43;1m";
    string BG_LBlue     = "44;1m";
    string BG_LMagenta  = "45;1m";
    string BG_LCyan     = "46;1m";
    string BG_LWhite    = "47;1m";

    // SHORCUT
    string CLS = ESC + ERASE_ALL + ESC + HOME;

    // SPECIAL KEY NAME
    const char NB_SPECIAL_KEY = 32;
    const array<string, NB_SPECIAL_KEY> specialKeyName = {
        "NUL",  // null character
        "SOH",  // start of heading
        "STX",  // start of text
        "ETX",  // end of text
        "EOT",  // end of transmission
        "ENQ",  // enquiry
        "ACK",  // acknowledgment
        "BEL",  // audible bell
        "BS",   // back space
        "HT",   // horizontal tab
        "LF",   // line feed
        "VT",   // vertical tab
        "FF",   // form feed
        "CR",   // carriage return
        "SO",   // shift out
        "SI",   // shift in
        "DLE",  // data link escape
        "DC1",  // device control 1
        "DC2",  // device control 2
        "DC3",  // device control 3
        "DC4",  // device control 4
        "NAK",  // negative acknowledge
        "SYN",  // synchronous idle
        "ETB",  // end of transmit block
        "CAN",  // cancel
        "EM",   // end of medium
        "SUB",  // substitution
        "ESC",  // escape
        "FS",   // file separator
        "GS",   // group separator
        "RS",   // record separator
        "US"    // unit separator
    };

#if !(defined(_WIN32) || defined(_WIN64))
    // Unix equivalent to conio _kbhit() function
    // Reference: https://invisible-island.net/ncurses/NCURSES-Programming-HOWTO.html
    inline int kbhit()
    {
        int ch;
        wtimeout(stdscr, 0);
        ch = wgetch(stdscr);
        nodelay(stdscr, FALSE);
        if (ch == ERR)
            return 0;
        ungetch(ch);
        return 1;
    }

    inline void initializeNCurses()
    {
        initscr(); // initialiser le programme en utilisant ncurses
        cbreak(); // rend disponible le caractère saisi immédiatement.
        noecho(); // désactive l'affichage sur l'écran d'une touche frappée au clavier
        nodelay(stdscr, TRUE); //empêche getch() d'être bloquant
        keypad(stdscr, TRUE); // active les touche étendu du clavier (au delà de 256)
    }

    inline void releaseNCurses()
    {
        keypad(stdscr, FALSE); // active les touche étendu du clavier (au delà de 256)
        nodelay(stdscr, FALSE); //empêche getch() d'être bloquant
        echo(); // rétablit l'affichage du caractère saisi
        nocbreak(); // rétablit le mode par défaut.
        endwin(); // doit être appelée avant de quitter le programme en mode curses. Elle permet de rétablir l'ancien mode du terminal.
    }
#endif

    // INPUT SECTION
    inline char getSpecialKeyCode(const string& keyCodeName)
    {
        for (int i = 0; i < NB_SPECIAL_KEY; ++i)
            if (specialKeyName[i] == keyCodeName)
                return (char)i;

        return 0;
    }

    inline string getSpecialKeyCodeName(char keyCode)
    {
        if (keyCode >= 0 && keyCode < NB_SPECIAL_KEY)
            return specialKeyName[keyCode];
        return "";
    }

    inline int waitKeyToPress(char keyToPress = {})
    {
        int keyPressed = {};

#if _WIN32 || _WIN64
        for (;;)
        {
            if (_kbhit())
            {
                keyPressed = _getch();
                if (keyPressed && keyPressed == keyToPress || !keyToPress)
                    break;
            }
        }
#else
        initializeNCurses();
        for (;;)
        {
            if (kbhit())
            {
                keyPressed = getch();
                if ((keyPressed && keyPressed == keyToPress) || !keyToPress)
                    break;
            }
        }
        releaseNCurses();
#endif

        return keyPressed;
    }

    inline void pressToContinue(string s = "")
    {
        if (s == "")
            s = "Appuis sur une touche pour continuer ... \n";
        cout << s;
        waitKeyToPress();
    }

    inline default_random_engine& getRand()
    {
        static default_random_engine ran;
        return ran;
    };

    // RANDOM SECTION
    inline void srandInt(int s) { getRand().seed(s); }
    inline int randInt(int min, int max) { return uniform_int_distribution<>{min, max}(getRand()); }
    inline int randInt(int max) { return randInt(0, max); }

    // WINDOW SECTION
    inline void showCursor(bool visible)
    {
#if defined(_WIN32) || defined(_WIN64)
        CONSOLE_CURSOR_INFO ci;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
        ci.bVisible = visible? TRUE: FALSE;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
#else
        // TODO
#endif
    }

    void setConsoleFontSize(const string& fontName, int size = 24)
    {
#if defined(_WIN32) || defined(_WIN64)
        if (size > 0 && size < 72)
        {
            CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
            HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
            if (!GetCurrentConsoleFontEx(hStdout, FALSE, &cfi))
            {
                cout << "Got font error :" << GetLastError() << endl;
                return;
            }

            cfi.dwFontSize.Y = size;
            std::wstring wsFontName(fontName.begin(), fontName.end());
            wcscpy_s(cfi.FaceName, wsFontName.c_str());

            if (!SetCurrentConsoleFontEx(hStdout, FALSE, &cfi))
                cout << "Set font Error :" << GetLastError() << endl;
        }
#else
        // TODO
#endif
    }
}
