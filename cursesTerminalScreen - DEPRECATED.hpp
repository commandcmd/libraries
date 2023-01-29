#include <ncurses.h>
#include <sstream>

#ifndef cursesTerminalScreen_HPP
#define cursesTerminalScreen_HPP

#define BLACK   COLOR_BLACK
#define RED     COLOR_RED
#define GREEN   COLOR_GREEN
#define YELLOW  COLOR_YELLOW
#define BLUE    COLOR_BLUE
#define MAGENTA COLOR_MAGENTA
#define CYAN    COLOR_CYAN
#define WHITE   COLOR_WHITE

#define FOREGROUND 100
#define BACKGROUND 101

namespace terminal {
    extern void init(unsigned int x = 0, unsigned int y = 0);
    extern void drawPixel(int x, int y, char character, int foreground = WHITE, int background = BLACK);
    extern char readPixel(int x, int y);
    extern int readPixelcolor(int x, int y, int type);
    extern void drawLine(int x1, int y1, int x2, int y2, char character, int foreground = WHITE, int background = BLACK);
    extern void drawRect(int x1, int y1, int x2, int y2, char character, int foreground = WHITE, int background = BLACK);
    extern void fillRect(int x1, int y1, int x2, int y2, char character, int foreground = WHITE, int background = BLACK);
    extern void fillScreen(char character, int foreground = WHITE, int background = BLACK);
    extern void updateDisplay();
    extern void end();

    template <typename... ARGS>
    extern void printText(int x, int y, int foreground, int background, ARGS... args);

    int height;
    int length;

    bool initialized = false;
    bool color_initialized = false;

    namespace input{
        std::string getString(bool echo = true, bool enterBreaks = true, char endchar = 0, int maxLength = 0){
            std::string output;

            for(int i = 0;true;i++){
                char input = getch();
                if(endchar > 0 && input == endchar || maxLength > 0 && i >= maxLength || enterBreaks && input == '\n')break;
                if((input == KEY_BACKSPACE || input == KEY_DC || input == 127)){
                    if(!output.empty())output.erase(output.end() - 1);
                    terminal::updateDisplay();
                    if(!output.empty())addstr(output.c_str());
                    continue;
                }

                output += input;
                if(echo)addch(input);
            }

            return output;
        }

        char getChar(bool echo = false){
            char input = getch();
            if(echo)addch(input);
            return input;
        }
    }

    namespace internal{
        int map(int input, int min1, int max1, int min2, int max2){
            return (int)((input - min1) * (((max2 - min2) * 1.00000000) / ((max1 - min1) * 1.00000000)) + min2);
        }

        int absolute(int input){
            if(input >= 0)return input;
            else return (input * -1);
        }

        template <class type>
        type *pointerOutput(type *pointer, int x = -1, int y = -1){
            int dimensions;

            int lengthY = terminal::height;

            if(y >= 0)dimensions = 2;
            else if(y == -1)dimensions = 1;
            else if(x == -1)return 0;

            pointer++;

            if(dimensions == 1)return (pointer + x);
            else if(dimensions == 2)return (pointer + x * lengthY + y);
            else return pointer;
        }

        int pair_number[8][8];
        int pair_number_count = 1;

        char *buffer;
        int *colorForeground;
        int *colorBackground;
    }
}


void terminal::init(unsigned int x, unsigned int y){
    if(initialized)return;

    initscr();
    noecho();

    if(has_colors()){
        start_color() == OK ? terminal::color_initialized = true : terminal::color_initialized = false;
    }

    if(!x)x = COLS - 1;
    if(!y)y = LINES - 1;

    int arrayLength = (x + 1) * (y + 1);

    terminal::height = y;
    terminal::length = x;

    terminal::internal::buffer = new char[arrayLength];

    if(terminal::color_initialized){
        terminal::internal::colorForeground = new int[arrayLength];
        terminal::internal::colorBackground = new int[arrayLength];
    }

    initialized = true;

    terminal::fillScreen(' ');

    terminal::updateDisplay();

    return;
}

void terminal::end(){
    if(!initialized)return;

    clear();
    endwin();

    terminal::height = 0;
    terminal::length = 0;

    delete terminal::internal::buffer;
    if(terminal::color_initialized){
        delete terminal::internal::colorForeground;
        delete terminal::internal::colorBackground;
        terminal::color_initialized = false;
    }

    initialized = false;

    return;
}

void terminal::updateDisplay(){
    if(!initialized)return;

    erase();

    int foreground;
    int background;

    for(int y = 0; y < terminal::height; y++){
        for(int x = 0; x < terminal::length; x++){
            if(terminal::color_initialized){
                foreground = *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x, y));
                background = *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x, y));

                if(!terminal::internal::pair_number[foreground][background]){
                    terminal::internal::pair_number[foreground][background] = terminal::internal::pair_number_count;
                    terminal::internal::pair_number_count++;
                    init_pair(terminal::internal::pair_number[foreground][background], foreground, background);
                }
                attron(COLOR_PAIR(terminal::internal::pair_number[foreground][background]));
            }

            addch(*(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x, y)));

            if(terminal::color_initialized)attroff(COLOR_PAIR(terminal::internal::pair_number[foreground][background]));
        }
        addch('\n');
    }

    refresh();

    return;
}

char terminal::readPixel(int x, int y){
    if(!initialized)return '\0';
    
    return *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x, y));
}

int terminal::readPixelcolor(int x, int y, int type){
    if(!initialized || !terminal::color_initialized)return 0;

    if(type = FOREGROUND) return *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x, y)); else if(type = BACKGROUND) return *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x, y));
    return 0;
}

void terminal::drawPixel(int x, int y, char character, int foreground, int background){
    if(!initialized)return;

    *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x, y)) = character;
    if(terminal::color_initialized){
        *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x, y)) = foreground;
        *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x, y)) = background;
    }

    return;
}

void terminal::fillScreen(char character, int foreground, int background){
    if(!initialized)return;

    for(int y = 0;y < terminal::height;y++){
        for(int x = 0;x < terminal::length;x++){
            *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x, y)) = character;
            if(terminal::color_initialized){
                *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x, y)) = foreground;
                *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x, y)) = background;
            }
        }
    }

    return;
}

void terminal::drawLine(int x1, int y1, int x2, int y2, char character, int foreground, int background){
    if(!initialized)return;

    if(terminal::internal::absolute(x2 - x1) >= terminal::internal::absolute(y2 - y1)){
        for(int outputX = x1;outputX <= x2;outputX++){
            int outputY = terminal::internal::map(outputX, x1, x2, y1, y2);
            *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, outputX, outputY)) = character;
            if(terminal::color_initialized){
                *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, outputX, outputY)) = foreground;
                *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, outputX, outputY)) = background;
            }
        }
    } else if(terminal::internal::absolute(x2 - x1) < terminal::internal::absolute(y2 - y1)) {
        for(int outputY = y1;outputY <= y2;outputY++){
            int outputX = terminal::internal::map(outputY, y1, y2, x1, x2);
            *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, outputX, outputY)) = character;
            if(terminal::color_initialized){
                *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, outputX, outputY)) = foreground;
                *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, outputX, outputY)) = background;
            }
        }
    }

    return;
}

void terminal::drawRect(int x1, int y1, int x2, int y2, char character, int foreground, int background){
    if(!initialized)return;

    for(int i = x1;i <= x2;i++){
        *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, i, y1)) = character;
        *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, i, y2)) = character;
        if(terminal::color_initialized){
            *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, i, y1)) = foreground;
            *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, i, y1)) = background;
            *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, i, y2)) = foreground;
            *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, i, y2)) = background;
        }
    }
    for(int i = y1;i <= y2;i++){
        *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x1, i)) = character;
        *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x2, i)) = character;
        if(terminal::color_initialized){
            *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x1, i)) = foreground;
            *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x1, i)) = background;
            *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x2, i)) = foreground;
            *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x2, i)) = background;
        }
    }

    return;
}

void terminal::fillRect(int x1, int y1, int x2, int y2, char character, int foreground, int background){
    if(!initialized)return;

    for(int x = x1;x <= x2;x++){
        for(int y = y1;y <= y2;y++){
            *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x, y)) = character;
            if(terminal::color_initialized){
                *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x, y)) = foreground;
                *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x, y)) = background;
            }
        }
    }

    return;
}

template <typename... ARGS>
void terminal::printText(int x, int y, int foreground, int background, ARGS... args){
    if (!initialized)return;

    std::stringstream join(std::ios::in | std::ios::out);

    (join << ... << args);

    std::string text = join.str();

    for(int i = 0;i < text.length();i++){
        if (text[i]=='\0')break;
        if (x + i >= terminal::length)break;

        terminal::drawPixel(x + i, y, text[i], foreground, background);
    }

    return;
}

#endif