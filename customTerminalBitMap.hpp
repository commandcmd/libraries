#ifndef customTerminalBitMap_HPP
#define customTerminalBitMap_HPP

#include "../libraries/customTerminalIO.hpp"

#define FOREGROUND 100
#define BACKGROUND 101

#define LIGHT_SHADE                           -1
#define MEDIUM_SHADE                          -2
#define DARK_SHADE                            -3
#define LIGHT_VERTICAL                        -4
#define VERTICAL_SINGLE_AND_LEFT              -5
#define VERTICAL_SINGLE_AND_LEFT_DOUBLE       -6
#define VERTICAL_DOUBLE_AND_LEFT_SINGLE       -7
#define DOWN_DOUBLE_AND_LEFT_SINGLE           -8
#define DOWN_SINGLE_AND_LEFT_DOUBLE           -9
#define DOUBLE_VERTICAL_AND_LEFT              -10
#define DOUBLE_VERTICAL                       -11
#define DOUBLE_DOWN_AND_LEFT                  -12
#define DOUBLE_UP_AND_LEFT                    -13
#define UP_DOUBLE_AND_LEFT_SINGLE             -14
#define UP_SINGLE_AND_LEFT_DOUBLE             -15
#define LIGHT_DOWN_AND_LEFT                   -16
#define LIGHT_UP_AND_RIGHT                    -17
#define LIGHT_UP_AND_HORIZONTAL               -18
#define LIGHT_DOWN_AND_HORIZONTAL             -19
#define LIGHT_VERTICAL_AND_RIGHT              -20
#define LIGHT_HORIZONTAL                      -21
#define LIGHT_VERTICAL_AND_HORIZONTAL         -22
#define VERTICAL_SINGLE_AND_RIGHT_DOUBLE      -23
#define VERTICAL_DOUBLE_AND_RIGHT_SINGLE      -24
#define DOUBLE_UP_AND_RIGHT                   -25
#define DOUBLE_DOWN_AND_RIGHT                 -26
#define DOUBLE_UP_AND_HORIZONTAL              -27
#define DOUBLE_DOWN_AND_HORIZONTAL            -28
#define DOUBLE_VERTICAL_AND_RIGHT             -29
#define DOUBLE_HORIZONTAL                     -30
#define DOUBLE_VERTICAL_AND_HORIZONTAL        -31
#define UP_SINGLE_AND_HORIZONTAL_DOUBLE       -32
#define UP_DOUBLE_AND_HORIZONTAL_SINGLE       -33
#define DOWN_SINGLE_AND_HORIZONTAL_DOUBLE     -34
#define DOWN_DOUBLE_AND_HORIZONTAL_SINGLE     -35
#define UP_DOUBLE_AND_RIGHT_SINGLE            -36
#define UP_SINGLE_AND_RIGHT_DOUBLE            -37
#define DOWN_SINGLE_AND_RIGHT_DOUBLE          -38
#define DOWN_DOUBLE_AND_RIGHT_SINGLE          -39
#define VERTICAL_DOUBLE_AND_HORIZONTAL_SINGLE -40
#define VERTICAL_SINGLE_AND_HORIZONTAL_DOUBLE -41
#define LIGHT_UP_AND_LEFT                     -42
#define LIGHT_DOWN_AND_RIGHT                  -43
#define FULL_BLOCK                            -44
#define LOWER_HALF_BLOCK                      -45
#define LEFT_HALF_BLOCK                       -46
#define RIGHT_HALF_BLOCK                      -47
#define UPPER_HALF_BLOCK                      -48
#define BLACK_SQUARE                          -49

namespace terminal {
    namespace bm{
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

        template <typename... args>
        extern void drawMenu(int x1, int y1, int x2, int y2, int colorFB, int colorBB, args... entries);

        unsigned int height;
        unsigned int length;

        bool initialized = false;
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
        type *pointerOutput(type *pointer, int x = -1, int y = -1, int lengthY = -1){
            int dimensions;

            if(lengthY == -1) lengthY = terminal::bm::height;

            if(y >= 0)dimensions = 2;
            else if(y == -1)dimensions = 1;
            else if(x == -1)return 0;

            pointer++;

            if(dimensions == 1)return (pointer + x);
            else if(dimensions == 2)return (pointer + x * lengthY + y);
            else return pointer;
        }

        void printOutputCustom(char input){
            switch(input){
                case LIGHT_SHADE                           : terminal::out::print("░");break;
                case MEDIUM_SHADE                          : terminal::out::print("▒");break;
                case DARK_SHADE                            : terminal::out::print("▓");break;
                case LIGHT_VERTICAL                        : terminal::out::print("│");break;
                case VERTICAL_SINGLE_AND_LEFT              : terminal::out::print("┤");break;
                case VERTICAL_SINGLE_AND_LEFT_DOUBLE       : terminal::out::print("╡");break;
                case VERTICAL_DOUBLE_AND_LEFT_SINGLE       : terminal::out::print("╢");break;
                case DOWN_DOUBLE_AND_LEFT_SINGLE           : terminal::out::print("╖");break;
                case DOWN_SINGLE_AND_LEFT_DOUBLE           : terminal::out::print("╕");break;
                case DOUBLE_VERTICAL_AND_LEFT              : terminal::out::print("╣");break;
                case DOUBLE_VERTICAL                       : terminal::out::print("║");break;
                case DOUBLE_DOWN_AND_LEFT                  : terminal::out::print("╗");break;
                case DOUBLE_UP_AND_LEFT                    : terminal::out::print("╝");break;
                case UP_DOUBLE_AND_LEFT_SINGLE             : terminal::out::print("╜");break;
                case UP_SINGLE_AND_LEFT_DOUBLE             : terminal::out::print("╛");break;
                case LIGHT_DOWN_AND_LEFT                   : terminal::out::print("┐");break;
                case LIGHT_UP_AND_RIGHT                    : terminal::out::print("└");break;
                case LIGHT_UP_AND_HORIZONTAL               : terminal::out::print("┴");break;
                case LIGHT_DOWN_AND_HORIZONTAL             : terminal::out::print("┬");break;
                case LIGHT_VERTICAL_AND_RIGHT              : terminal::out::print("├");break;
                case LIGHT_HORIZONTAL                      : terminal::out::print("─");break;
                case LIGHT_VERTICAL_AND_HORIZONTAL         : terminal::out::print("┼");break;
                case VERTICAL_SINGLE_AND_RIGHT_DOUBLE      : terminal::out::print("╞");break;
                case VERTICAL_DOUBLE_AND_RIGHT_SINGLE      : terminal::out::print("╟");break;
                case DOUBLE_UP_AND_RIGHT                   : terminal::out::print("╚");break;
                case DOUBLE_DOWN_AND_RIGHT                 : terminal::out::print("╔");break;
                case DOUBLE_UP_AND_HORIZONTAL              : terminal::out::print("╩");break;
                case DOUBLE_DOWN_AND_HORIZONTAL            : terminal::out::print("╦");break;
                case DOUBLE_VERTICAL_AND_RIGHT             : terminal::out::print("╠");break;
                case DOUBLE_HORIZONTAL                     : terminal::out::print("═");break;
                case DOUBLE_VERTICAL_AND_HORIZONTAL        : terminal::out::print("╬");break;
                case UP_SINGLE_AND_HORIZONTAL_DOUBLE       : terminal::out::print("╧");break;
                case UP_DOUBLE_AND_HORIZONTAL_SINGLE       : terminal::out::print("╨");break;
                case DOWN_SINGLE_AND_HORIZONTAL_DOUBLE     : terminal::out::print("╤");break;
                case DOWN_DOUBLE_AND_HORIZONTAL_SINGLE     : terminal::out::print("╥");break;
                case UP_DOUBLE_AND_RIGHT_SINGLE            : terminal::out::print("╙");break;
                case UP_SINGLE_AND_RIGHT_DOUBLE            : terminal::out::print("╘");break;
                case DOWN_SINGLE_AND_RIGHT_DOUBLE          : terminal::out::print("╒");break;
                case DOWN_DOUBLE_AND_RIGHT_SINGLE          : terminal::out::print("╓");break;
                case VERTICAL_DOUBLE_AND_HORIZONTAL_SINGLE : terminal::out::print("╫");break;
                case VERTICAL_SINGLE_AND_HORIZONTAL_DOUBLE : terminal::out::print("╪");break;
                case LIGHT_UP_AND_LEFT                     : terminal::out::print("┘");break;
                case LIGHT_DOWN_AND_RIGHT                  : terminal::out::print("┌");break;
                case FULL_BLOCK                            : terminal::out::print("█");break;
                case LOWER_HALF_BLOCK                      : terminal::out::print("▄");break;
                case LEFT_HALF_BLOCK                       : terminal::out::print("▌");break;
                case RIGHT_HALF_BLOCK                      : terminal::out::print("▐");break;
                case UPPER_HALF_BLOCK                      : terminal::out::print("▀");break;
                case BLACK_SQUARE                          : terminal::out::print("■");break;
            }
        }

        char *buffer;
        int *colorForeground;
        int *colorBackground;

        namespace menuDrawing {
            bool bufN = false;
            bool firstExec = true;
            unsigned int menuArrayLength = 0;

            char *bufferA;
            char *bufferB;

            void expandToArray(const char input[128]){
                if(bufN) bufferA = new char[128 * (++menuArrayLength + 1)]; else bufferB = new char[128 * (++menuArrayLength + 1)];

                for(unsigned int b = 0; b < menuArrayLength - 1; b++){
                    for(unsigned short a = 0;a < 128 && *(terminal::internal::pointerOutput<char>((bufN ? bufferB : bufferA), a, b, 1)) != '\0';a++){
                        *(terminal::internal::pointerOutput<char>((bufN ? bufferA : bufferB), a, b, 1)) = *(terminal::internal::pointerOutput<char>((bufN ? bufferB : bufferA), a, b, 1));
                    }
                }

                for(short x = 0;x < 128 && input[x] != '\0';x++){*(terminal::internal::pointerOutput<char>((bufN ? bufferA : bufferB), x, menuArrayLength - 1, 1)) = input[x];}

                if(bufN && !firstExec) delete bufferB; else if(!bufN && !firstExec) delete bufferA;
                bufN = !bufN;
                firstExec = false;

                out::println(ENDLINE);
            }

            void copyOutputToArray(char *output){
                for(short y = 0;y < menuArrayLength;y++){
                    for(short x = 0;x < 128;x++)*(terminal::internal::pointerOutput<char>(output, x, y, 1)) = *(terminal::internal::pointerOutput<char>((bufN ? bufferB : bufferA), x, y, 1));
                }

                if(bufN) delete bufferA; else delete bufferB;
            }
        }
    }
}


void terminal::bm::init(unsigned int x, unsigned int y){
    if(initialized)return;

    terminal::scr::init_alt();

    if(!x)x = terminal::cols() - 1;
    if(!y)y = terminal::rows() - 1;

    int arrayLength = (x + 1) * (y + 1);

    terminal::bm::height = y;
    terminal::bm::length = x;

    terminal::internal::buffer = new char[arrayLength];
    terminal::internal::colorForeground = new int[arrayLength];
    terminal::internal::colorBackground = new int[arrayLength];
    initialized = true;

    terminal::bm::fillScreen(' ');
    terminal::bm::updateDisplay();

    return;
}

void terminal::bm::end(){
    if(!initialized)return;

    terminal::scr::reset();
    terminal::scr::end_alt();

    terminal::bm::height = 0;
    terminal::bm::length = 0;

    delete terminal::internal::buffer;
    delete terminal::internal::colorForeground;
    delete terminal::internal::colorBackground;

    initialized = false;

    return;
}

void terminal::bm::updateDisplay(){
    if(!initialized)return;

    terminal::scr::reset();

    char currentBuffer;

    for(int y = 0; y < terminal::bm::height; y++){
        for(int x = 0; x < terminal::bm::length; x++){
            terminal::out::set_color(*(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x, y)), *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x, y)));
            
            currentBuffer = *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x, y));
            if(currentBuffer > 0)terminal::out::printch(currentBuffer);
            else terminal::internal::printOutputCustom(currentBuffer);
        }
        terminal::out::print(ENDLINE);
    }

    return;
}

char terminal::bm::readPixel(int x, int y){
    if(!initialized)return '\0';
    
    return *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x, y));
}

int terminal::bm::readPixelcolor(int x, int y, int type){
    if(!initialized)return 0;

    if(type = FOREGROUND) return *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x, y)); else if(type = BACKGROUND) return *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x, y));
    return 0;
}

void terminal::bm::drawPixel(int x, int y, char character, int foreground, int background){
    if(!initialized)return;

    *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x, y)) = character;

    *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x, y)) = foreground;
    *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x, y)) = background;

    return;
}

void terminal::bm::fillScreen(char character, int foreground, int background){
    if(!initialized)return;

    for(int y = 0;y < terminal::bm::height;y++){
        for(int x = 0;x < terminal::bm::length;x++){
            *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x, y)) = character;

            *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x, y)) = foreground;
            *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x, y)) = background;
        }
    }

    return;
}

void terminal::bm::drawLine(int x1, int y1, int x2, int y2, char character, int foreground, int background){
    if(!initialized)return;

    if(terminal::internal::absolute(x2 - x1) >= terminal::internal::absolute(y2 - y1)){
        for(int outputX = x1;outputX <= x2;outputX++){
            int outputY = terminal::internal::map(outputX, x1, x2, y1, y2);
            *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, outputX, outputY)) = character;

            *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, outputX, outputY)) = foreground;
            *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, outputX, outputY)) = background;
        }
    } else if(terminal::internal::absolute(x2 - x1) < terminal::internal::absolute(y2 - y1)) {
        for(int outputY = y1;outputY <= y2;outputY++){
            int outputX = terminal::internal::map(outputY, y1, y2, x1, x2);
            *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, outputX, outputY)) = character;

            *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, outputX, outputY)) = foreground;
            *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, outputX, outputY)) = background;
        }
    }

    return;
}

void terminal::bm::drawRect(int x1, int y1, int x2, int y2, char character, int foreground, int background){
    if(!initialized)return;

    for(int i = x1;i <= x2;i++){
        *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, i, y1)) = character;
        *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, i, y2)) = character;

        *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, i, y1)) = foreground;
        *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, i, y1)) = background;
        *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, i, y2)) = foreground;
        *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, i, y2)) = background;
    }
    for(int i = y1;i <= y2;i++){
        *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x1, i)) = character;
        *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x2, i)) = character;

        *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x1, i)) = foreground;
        *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x1, i)) = background;
        *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x2, i)) = foreground;
        *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x2, i)) = background;
    }

    return;
}

void terminal::bm::fillRect(int x1, int y1, int x2, int y2, char character, int foreground, int background){
    if(!initialized)return;

    for(int x = x1;x <= x2;x++){
        for(int y = y1;y <= y2;y++){
            *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x, y)) = character;

            *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x, y)) = foreground;
            *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x, y)) = background;
        }
    }

    return;
}

template <typename... ARGS>
void terminal::bm::printText(int x, int y, int foreground, int background, ARGS... args){
    if (!initialized)return;

    int text_length = (terminal::internal::concat::sumAllLength(args) + ...);
    char text[text_length];
    for(int i = 0;i < text_length;i++)text[i] = 0;

    using expander = int[]; 
    (void)expander{0, ((void)terminal::internal::concat::sumAll(args, text), 0)...};

    for(int i = 0;i < text_length;i++){
        if (text[i]=='\0')break;
        if (x + i >= terminal::bm::length)break;
   
        terminal::bm::drawPixel(x + i, y, text[i], foreground, background);
    }

    return;
}

template <typename... args>
void terminal::bm::drawMenu(int x1, int y1, int x2, int y2, int colorFB, int colorBB, args... entries){
    if(!bm::initialized)return;

    terminal::bm::drawPixel(x1, y1, DOUBLE_DOWN_AND_RIGHT, colorFB, colorBB);
    terminal::bm::drawPixel(x1, y2, DOUBLE_UP_AND_RIGHT, colorFB, colorBB);
    terminal::bm::drawPixel(x2, y1, DOUBLE_DOWN_AND_LEFT, colorFB, colorBB);
    terminal::bm::drawPixel(x2, y2, DOUBLE_UP_AND_LEFT, colorFB, colorBB);

    terminal::bm::drawLine(x1 + 1, y1, x2 - 1, y1, DOUBLE_HORIZONTAL, colorFB, colorBB);
    terminal::bm::drawLine(x1 + 1, y2, x2 - 1, y2, DOUBLE_HORIZONTAL, colorFB, colorBB);

    terminal::bm::drawLine(x1, y1 + 1, x1, y2 - 1, DOUBLE_VERTICAL, colorFB, colorBB);
    terminal::bm::drawLine(x2, y1 + 1, x2, y2 - 1, DOUBLE_VERTICAL, colorFB, colorBB);

    using expander = int[];
    (void)expander{0, ((void)terminal::internal::menuDrawing::expandToArray(entries), 0)...};

    char *bufferOutput = new char[128 * terminal::internal::menuDrawing::menuArrayLength];
    terminal::internal::menuDrawing::copyOutputToArray(bufferOutput);

    for(unsigned int y = 0;y < terminal::internal::menuDrawing::menuArrayLength;y++){
        for(unsigned short x = 0;x < 128 && *(terminal::internal::pointerOutput<char>(bufferOutput, x, y, 1)) != '\0';x++){
            out::println("bufferOutput[", x, "][", y, "] = ", *(terminal::internal::pointerOutput<char>(bufferOutput, x, y, 1)));
        }
    }

    delete bufferOutput;

    return;
}

#endif