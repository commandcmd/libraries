#ifndef customTerminalBitMap_HPP
#define customTerminalBitMap_HPP

//Calling customTerminalIO.hpp, the IO library this code uses
#include "customTerminalIO.hpp"

//Defining names used in readPixel function
#define FOREGROUND 100
#define BACKGROUND 101

//Defining all the special characters we can use in all the functions in this code (these names get converted in characters in the function printOutputCustom(char))
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

//Main namespace that contains the whole program
namespace terminal {
    //BitMap namespace, contains the main commands that the user should use in this library
    namespace bm{
        void init(unsigned int length = 0, unsigned int height = 0); //Initialises the terminal, if no length and height are specified then read them from the terminal
        void drawPixel(int x, int y, char character, int foreground = WHITE, int background = BLACK); //Draws a character on the terminal at a specified x and y position
        char readPixel(int x, int y); //Returns what character does the program's buffer contain in the specified position
        int readPixelColor(int x, int y, int type); //Returns what color (user specifies wether foreground or background) does the program's buffer contain in the specified position
        void drawLine(int x1, int y1, int x2, int y2, char character, int foreground = WHITE, int background = BLACK); //Draws a line on screen specifying start and end position
        void drawRect(int x1, int y1, int x2, int y2, char character, int foreground = WHITE, int background = BLACK); //Draws a rectangle on screen specifying start and end position
        void fillRect(int x1, int y1, int x2, int y2, char character, int foreground = WHITE, int background = BLACK); //Draws a rectangle on screen knowing start and end position then fills it with (char)character
        void fillScreen(char character, int foreground = WHITE, int background = BLACK); //Fills the entire screen with a character
        void updateDisplay(bool optimized = true); //Updates the display with the changes made to the buffer, if "optimized" is specified as false then update the whole display not checking if there's been a change or not
        void end(); //Close the display and delete the buffer (not calling this function at the end of the program will cause a memory leak)

        void editColor(int x, int y, int colorForeground, int colorBackground); //Edit the color of a certain pixel
        void editColorForeground(int x, int y, int color); //Edit only the foreground color of a certain pixel
        void editColorBackground(int x, int y, int color); //Edit only the background color of a certain pixel
        void editText(int x, int y, char text); //Edit only the text of a certain pixel (without changing the color)

        template <typename... ARGS> //The template is so that the method accepts a variadic argument pack as an input, concatenating it all into a single string that then gets printed out to the terminal
        void printText(int x, int y, int foreground, int background, ARGS... args); //Print some text on the screen

        //These variables refer to the terminal and they get filled when calling the init() function
        unsigned int height;
        unsigned int length;

        bool initialized = false; //This variable determines wether the terminal has been initialised or not, it prevents to call other methods like drawPixel() or end() without having initialised the terminal or init() with the terminal already initialised
    }

    //This namespace contains all the internal functions of the terminal which only need to be accessed from other methods in the main namespace
    namespace internal{
        //Mapping function, it's used in drawLine to calculate the values of where should the pixel be drawn
        int map(int input, int min1, int max1, int min2, int max2){
            //Calculate the ratio of the two scales with a precision of 8 decimal digits
            double ratio = ((max2 - min2) * 1.00000000) / ((max1 - min1) * 1.00000000);

            //Calculate the value of the input relative to its scale
            int input_relative = input - min1;

            //Use the relative input we found and multiply it to the ratio with a precision of 8 decimal digits
            double input_converted_relative = input_relative * ratio * 1.00000000;

            //Now that we did all the calculations we just need to add to the output the start of scale 2 making it not relative to the scale anymore
            //Rounding is done by casting the converted input to int which always rounds it down
            int output = (int)input_converted_relative + min2;

            return output;
        } //terminal::internal::map

        //A template allows this function to be inputted and return any type
        template <class type>
        //This is an internal function that returns a 2D pointer array position by just being inputted the x and y position of it
        type *pointerOutput(type *pointer, int x, int y){
            return (++pointer + x * terminal::bm::height + y); //Do the math and return the result that's it
        } //terminal::internal::pointerOutput

        //This method returns the non-ASCII character staticly defined at the beginning of the code and prints it out to the terminal
        void printOutputCustom(char input){ //Take a char input
            switch(input){
                //if the input is:                           print out to the terminal the corresponding character:
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
            } //switch(input)
        }//terminal::internal::printOutputCustom

        //These are the declarations for the various buffer which this library uses
        char *buffer; //Main buffer, it stores a 2D char array of all the characters on the terminal
        int *colorForeground; //colorForeground buffer, it stores a 2D int array representing the color of the text of each character on the terminal
        int *colorBackground; //colorBackground buffer, it stores a 2D int array representing the color of the background of each character on the terminal
        bool *bufferChanged; //bufferChanged buffer, it stores a 2D bool array memorizing if any pixel got changed or not, so that the updateDisplay() function can update only the changed pixels instead of all of them
    }
}

//This function is used to initialise the terminal
//In the specific, it assigns as much space to the buffer pointer as either defined by the function arguments or, if they're empty, by the terminal's dimensions read by the function terminal::cols() and terminal::rows() contained in customTerminalIO.hpp
void terminal::bm::init(unsigned int _length, unsigned int _height){
    if(initialized)return; //If the terminal has already been initialised then don't execute this code

    terminal::scr::init_alt(); //Initialises an alternative display (only works on xterm) - this makes it so that when the terminal gets closed it restores what was showing before it was initialised


    //If either the length or the height hasn't been specified fill them with the values read from the terminal
    if(!_length)_length = terminal::cols() - 1;
    if(!_height)_height = terminal::rows() - 1;

    unsigned int arrayLength = (_length + 1) * (_height + 1); //This variable stores the size of the terminal's buffer arrays

    //Fill the global variables for height and length with the values inputted from the user or the values read from the terminal
    terminal::bm::height = _height;
    terminal::bm::length = _length;

    //Allocate the space to all the dynamic arrays with a size of arrayLength which was found before
    terminal::internal::buffer = new char[arrayLength];
    terminal::internal::colorForeground = new int[arrayLength];
    terminal::internal::colorBackground = new int[arrayLength];
    terminal::internal::bufferChanged = new bool[arrayLength];

    //All the other arrays are no problem because any value stored in them is going to get overridden anyway, but in the bufferChanged array only certain values get set to true and the program expects the rest to be false, so we initialise it by setting all of them to this state.
    for(unsigned int i = 0;i < arrayLength;i++) //Loop trough all the values of bufferChanged
        *(terminal::internal::bufferChanged + i) = false; //Set the current value to false

    terminal::cur::hide(); //Hide the cursor, this because when the screen draws stuff around the cursor can get stuck at the center of it until another updateDisplay occurs, so we hide it to prevent it to be visible in the middle of the screen

    initialized = true; //Tell the rest of the code the terminal has been initialised and now we can run all the other functions with no problem

    terminal::bm::fillScreen(' '); //Clear the screen
    terminal::bm::updateDisplay(); //Update the display after having cleared the screen

    return; //End the function
} //terminal::bm::init()

//This function deallocates the space occupied by the buffer and clears the terminal
void terminal::bm::end(){
    if(!initialized)return; //If the terminal has been already initialised then skip this code

    terminal::scr::reset(); //Reset the terminal
    terminal::scr::end_alt(); //End the alternate screen

    terminal::cur::show(); //Show the cursor

    //Reset the values for terminal dimensions
    terminal::bm::height = 0;
    terminal::bm::length = 0;

    //Delete all the buffers
    delete[] terminal::internal::buffer;
    delete[] terminal::internal::colorForeground;
    delete[] terminal::internal::colorBackground;
    delete[] terminal::internal::bufferChanged;

    //Tell the rest of the program the terminal is not initialised anymore
    initialized = false;

    return; //End the function
} //terminal::bm::end()

//This function is used to fetch the changes made in the buffer to the screen on the terminal
//The "optimized" variable determines wether to optimize this process by updating only changed pixels on the terminal or updating all, by default this is set to true
void terminal::bm::updateDisplay(bool optimized){
    if(!initialized)return; //If the terminal hasn't been initialised then skip this function

    char currentBuffer; //Used to store the current character contained in the loop

    for(int y = 0; y < terminal::bm::height; y++){ //Looping trough all the rows of the terminal
        for(int x = 0; x < terminal::bm::length; x++){ //Looping trough all the columns of the terminal
            if(optimized){ //If we want to optimize this process
                if(*(terminal::internal::pointerOutput <bool> (terminal::internal::bufferChanged, x, y)) == false)continue; //If the current buffer hasn't been changed then skip the current iteration of the loop
                *(terminal::internal::pointerOutput <bool> (terminal::internal::bufferChanged, x, y)) = false; //If it has been changed set it to false, we always want to reset all the values of this array to false
            }

            terminal::cur::set_position(y, x); //Set the position of the cursor to where we want to write the character
            terminal::out::set_color(*(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x, y)), *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x, y))); //Set the color of the cursor for when we print

            currentBuffer = *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x, y)); //Fill the current buffer with the value of the buffer array at the current position indicated by the iterators of the for loop
            if(currentBuffer > 0)terminal::out::printch(currentBuffer); //If the current buffer is a normal character then print it out
            else terminal::internal::printOutputCustom(currentBuffer); //If it's a special character pass it trough a function to handle it and it is going to print it out.
        } //For loop iterator X
    } //For loop iterator Y

    return; //End of the function
} //terminal::bm::updateDisplay()

//This function prints a single pixel on the terminal
//In the specific it modifies all the buffers to the specified values, and sets the bufferChanged array's current position to true to tell updateDisplay that the current pixel changed
//This is used by all the other fancy functions to loop trough certain values calling this function every iteration to draw lines, rectangles, etc.
void terminal::bm::drawPixel(int x, int y, char character, int foreground, int background){ 
    if(!initialized)return; //If the terminal hasn't been initialised then skip the function

    //If there's any change to any buffer then set the bufferChanged for the current position to true so that it updates the current pixel during updateDisplay
    //This makes it so that if we write the same pixel twice it won't actually update it twice
    if(                                                                                                      //If
        *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x, y)) != character          //The text we're trying to write is different than what's already there
            ||                                                                                               //OR
        *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x, y)) != foreground //The foreground color of the text we're trying to write is different than what's already there
            ||                                                                                               //OR 
        *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x, y)) != background //The background color of the text we're trying to write is different than what's already there
    )                                                                                                        //Then:
        *(terminal::internal::pointerOutput <bool> (terminal::internal::bufferChanged, x, y)) = true;        //Update the current pixel

    //Update the buffers at the specified position to the specified values
    *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x, y)) = character;
    *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x, y)) = foreground;
    *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x, y)) = background;

    return; //End of the function
} //terminal::bm::drawPixel

//This function returns the text contained in the program's buffer at the position specified
char terminal::bm::readPixel(int x, int y){
    if(!initialized)return '\0'; //If the terminal hasn't been initialised then return nothing
    
    return *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x, y)); //Access the buffer and return what does it contain at specified position
} //terminal::bm::readPixel()

//This function returns what does the specified color buffer return at the specified position
int terminal::bm::readPixelColor(int x, int y, int type){
    if(!initialized)return 0;//If the terminal hasn't been initialised then return nothing

    //Depending on the type input access the specified buffer and return the specified position
    if(type == FOREGROUND) return *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x, y));
    else if(type == BACKGROUND) return *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x, y));
    
    return 0; //If the type is invalid return nothing
} //terminal::bm::readPixelColor()

//This function loops trough the entire buffer and fills it all up with the specified character and the specified color
void terminal::bm::fillScreen(char character, int foreground, int background){
    if(!initialized)return; //If the terminal hasn't been initialised then skip the function

    //Loop trough all the pixels on the screen in x and y direction
    for(int y = 0;y < terminal::bm::height;y++){
        for(int x = 0;x < terminal::bm::length;x++){
            terminal::bm::drawPixel(x, y, character, foreground, background); //Update the pixel
        }
    }

    return; //End the function
} //terminal::bm::fillScreen

//This function is used to draw a line on the screen
void terminal::bm::drawLine(int x1, int y1, int x2, int y2, char character, int foreground, int background){
    if(!initialized)return; //If the terminal hasn't been initialised then skip this function

    //We want to make sure that x2 is always greater than x1:
    if(x2 < x1){ //If x2 is smaller than x1 then
        int copyX = x1; //Store x1 since we want to swap x2 and x1
        x1 = x2; //Set x1 to the value that x2 has
        x2 = copyX; //Set x2 to the value we stores previously
    }
    //We need to do the same for y:
    if(y2 < y1){ //If y2 is smaller than y1 then
        int copyY = y1; //Store y1
        y1 = y2; //Set y1 to the current y2 holds
        y2 = copyY; //Set y2 to the value we stored previously for y1
    }

    if((x2 - x1) >= (y2 - y1)){
        for(int outputX = x1;outputX <= x2;outputX++){ //Loop trough all the x values from x1 to x2
            //For the current x value we get the y value by mapping the x one from the scale that goes from x1 to x2 to the scale that goes from y1 to y2
            int outputY = terminal::internal::map(outputX, x1, x2, y1, y2);
            
            //Now that we found its coordinates, update the pixel
            terminal::bm::drawPixel(outputX, outputY, character, foreground, background);
        }
    } else if((x2 - x1) < (y2 - y1)) {
        for(int outputY = y1;outputY <= y2;outputY++){ //Loop trouch all the value from y1 to y2
            //Get the x value by mapping the y value from the scale that goes from y1 to y2 to the one that goes from x1 to x2
            int outputX = terminal::internal::map(outputY, y1, y2, x1, x2);

            //Now that we found its coordinates, update the pixel
            terminal::bm::drawPixel(outputX, outputY, character, foreground, background);
        }
    }

    return; //End the function
} //terminal::bm::drawLine

//This function draws a rectangle on screen
void terminal::bm::drawRect(int x1, int y1, int x2, int y2, char character, int foreground, int background){
    if(!initialized)return; //If the terminal has already been initialised then skip the function

    for(int i = x1;i <= x2;i++){ //Loop trough all the x values
        //Draw two pixels, one for the lower side of the rectangle, one for the higher one
        terminal::bm::drawPixel(i, y1, character, foreground, background);
        terminal::bm::drawPixel(i, y2, character, foreground, background);
    }
    for(int i = y1;i <= y2;i++){ //Loop trough all the y values
        //Draw two pixels, one for the left side of the rectangle and one for the right one
        terminal::bm::drawPixel(x1, i, character, foreground, background);
        terminal::bm::drawPixel(x2, i, character, foreground, background);
    }

    return; //End the function
} //terminal::bm::drawRect

//This function draws a rectangle on screen filling it with characters
void terminal::bm::fillRect(int x1, int y1, int x2, int y2, char character, int foreground, int background){
    if(!initialized)return; //If the terminal has already been initialised then return

    //These for loops loop trough all the pixels in the rectangle filling them
    for(int x = x1;x <= x2;x++){ //Loop from x1 to x2 
        for(int y = y1;y <= y2;y++){ //Loop from y1 to y2
            terminal::bm::drawPixel(x, y, character, foreground, background); //Fill the current pixel with the specified characte and colors
        }
    }

    return; //End the function
} //terminal::bm::fillRect

//This function edits only the colors of a pixel without modifying the character
void terminal::bm::editColor(int x, int y, int colorForeground, int colorBackground){
    if(!initialized) return; //If the terminal hasn't been initialised then skip this function

    *(terminal::internal::pointerOutput <bool> (terminal::internal::bufferChanged, x, y)) = true; //Update the specified pixel

    *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x, y)) = colorForeground; //Update colorForeground's buffer
    *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x, y)) = colorBackground; //Update colorBackground's buffer

    return; //End the function
} //terminal::bm::editColor

//This function just edits colorForeground buffer without changing anything else
void terminal::bm::editColorForeground(int x, int y, int color){
    if(!initialized) return; //If the terminal hasn't been initalised then skip this function

    *(terminal::internal::pointerOutput <bool> (terminal::internal::bufferChanged, x, y)) = true; //Update the specified pixel

    *(terminal::internal::pointerOutput <int> (terminal::internal::colorForeground, x, y)) = color; //Update the colorForeground buffer with the specified color

    return; //End the function
} //terminal::bm::editColorForeground

//This function just edits the colorBackground buffer without changing anything else
void terminal::bm::editColorBackground(int x, int y, int color){
    if(!initialized) return; //If the terminal hasn't been initialised then skip this function

    *(terminal::internal::pointerOutput <bool> (terminal::internal::bufferChanged, x, y)) = true; //Update the specified pixel

    *(terminal::internal::pointerOutput <int> (terminal::internal::colorBackground, x, y)) = color; //Update the colorBackground buffer with he specified color

    return; //End the function
} //terminal::bm::editColorBackground

//This function just edits the character in a specified position without changing anything else
void terminal::bm::editText(int x, int y, char text){
    if(!initialized) return; //If the terminal hasn't been initialised then return

    *(terminal::internal::pointerOutput <bool> (terminal::internal::bufferChanged, x, y)) = true; //Update the specified piel

    *(terminal::internal::pointerOutput <char> (terminal::internal::buffer, x, y)) = text; //Update the text buffer with the specified character

    return; //End the function
} //terminal::bm::editText

//This template allows this function to get an input of any number of any type of arguments for the text that'll get outputted to the screen
template <typename... ARGS>
//This function prints a c string to the terminal (which is just a char array) at a specified position
void terminal::bm::printText(int x, int y, int foreground, int background, ARGS... args){
    if (!initialized)return; //If the terminal hasn't been initialised then skip this function

    //Send the whole variadic argument pack to the sumAllLength function that accepts any type trough overloading
    //This is to get the length of the entire variadic argument pack when it'll be all concatenated in the char array that we're going to create, and to create it we need its length, so we sum all the lengths and store them in a variable
    int text_length = (terminal::internal::concat::sumAllLength(args) + ...);
    char text[text_length]; //Create the char array which will store the string
    for(int i = 0;i < text_length;i++)text[i] = 0; //Initialise the array by filling it with zeroes

    //Expand all the variadic arguments into the sumAll function to concatenate all the arguments of all the types into the char array we created
    using expander = int[]; 
    (void)expander{0, ((void)terminal::internal::concat::sumAll(args, text), 0)...};

    for(int i = 0;i < text_length;i++){ //Loop trough all the characters of the char array we created
        if (text[i]=='\0')break; //If the current character in the char array is NULL then break the loop
        if (x + i >= terminal::bm::length)break; //if the text we're printing goes out of the terminal's screen then break the loop otherwise we'll get segfaults for writing out of the buffer

        terminal::bm::drawPixel(x + i, y, text[i], foreground, background); //Write on screen the current character
    }

    return; //End the function
} //terminal::bm::printText

#endif //#ifndef customTerminalBitMap_HPP
