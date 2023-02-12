#ifndef customTerminalIO_HPP
#define customTerminalIO_HPP

//got the windows code from : https://stackoverflow.com/questions/23369503/get-size-of-terminal-window-rows-columns
#if defined(_WIN32)
    #error THE WINDOWS VERSION OF THIS LIBRARY IS STILL IN DEVELOPMENT

    #define WIN32_LEAN_AND_MEAN
    #define VC_EXTRALEAN
    
    #include <Windows.h>
#elif defined(__linux__)
    #include <unistd.h>
    #include <termios.h>
    #include <sys/ioctl.h>
    #include <fcntl.h>
#endif

//Defining an ENDLINE containing a carriage return, this because the rawMode doesn't translate \n into \r\n and and it doesn't carriage return the cursor if we just use '\n' so ENDLINE got defined as "\r\n"
#define ENDLINE        "\r\n"

#define CTRL_KEY(in) ((in) & 0x1f)  //This code bitwise-ANDs the first 3 bits of "in" to 0 (being 0x1f = 00011111) to check if a key has been pressed alongside as CTRL
                                    //Got this line from https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html

//got all these codes from : https://en.wikipedia.org/wiki/ANSI_escape_code
//Text types:
#define RESET               0
#define BOLD                1
#define FAINT               2
#define ITALIC              3
#define UNDERLINED          4
#define SLOW_BLINK          5
#define FAST_BLINK          6
#define INVERT              7
#define CONCEAL             8
#define STRIKE              9
#define DEFAULT_FONT        10
#define ALTERNATIVE_FONT    11
#define DOUBLE_UNDERLINE    21
#define NORMAL_INTENSITY    22
#define NO_UNDERLINE        24
#define NO_BLINK            25
#define NO_INVERT           27
#define NO_CONCEAL          28
#define NO_STRIKE           29
#define DEFAULT_FCOLOR      39
#define DEFAULT_BCOLOR      49
#define OVERLINED           53
#define FRAMED              51
#define ENCIRCLED           52
#define NO_FRAMED           54
#define NO_ENCIRCLED        54
#define NO_OVERLINED        55

//Default colors: (RGB is supported too)
#define BLACK               30
#define RED                 31
#define GREEN               32
#define YELLOW              33
#define BLUE                34
#define MAGENTA             35
#define CYAN                36
#define WHITE               37
#define GRAY                90
#define BRIGHT_RED          91
#define BRIGHT_GREEN        92
#define BRIGHT_YELLOW       93
#define BRIGHT_BLUE         94
#define BRIGHT_MAGENTA      95
#define BRIGHT_CYAN         96
#define BRIGHT_WHITE        97

//Defined direct input keys:
#define KEY_UP             -1
#define KEY_DOWN           -2
#define KEY_LEFT           -3
#define KEY_RIGHT          -4
#define KEY_STARTL         -5
#define KEY_ENDL           -6
#define KEY_PGUP           -7
#define KEY_PGDOWN         -8
#define KEY_INSERT         -9
#define KEY_DELETE         -10
#define KEY_CENTER         -11
#define KEY_ESCAPE         -12
#define KEY_F1             -13
#define KEY_F2             -14
#define KEY_F3             -15
#define KEY_F4             -16
#define KEY_F5             -17
#define KEY_F6             -18
#define KEY_F7             -19
#define KEY_F8             -20
#define KEY_F9             -21
#define KEY_F10            -22
#define KEY_F11            -23
#define KEY_F12            -24
#define KEY_PAUSE          -25

//Other keys for direct-input:
#define BACKSPACE           127
#define ENTER               13
#define TAB                 9
#define SPACE               32

//Main terminal namespace which contains all the functions of the header
namespace terminal{
    //This function works on linux and on windows
    //got the windows code from : https://stackoverflow.com/questions/23369503/get-size-of-terminal-window-rows-columns

    //This function gets and returns the number of rows of the terminal
    int rows(){
        #if defined(_WIN32)
            CONSOLE_SCREEN_BUFFER_INFO CSBI;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CSBI);
            return (int)(csbi.srWindow.Right-csbi.srWindow.Left+1);
        #elif defined(__linux__)
            //Explanation for this code from https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html

            struct winsize win; //Calling the winsize struct from <sys/ioctl.h> as win
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &win); //using the ioctl command to get the attributes of STDOUT_FILENO and write them into the win (winsize) struct
            return win.ws_row; //return the ws_row (n of rows of terminal) attribute from the win struct
        #endif
    } //int terminal::rows()

    //This function gets and returns the number of columns of the terminal
    int cols(){
        //This function works on linux and on windows
        //got the windows code from : https://stackoverflow.com/questions/23369503/get-size-of-terminal-window-rows-columns

        #if defined(_WIN32)
            CONSOLE_SCREEN_BUFFER_INFO CSBI;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CSBI);
            return (int)(csbi.srWindow.Bottom-csbi.srWindow.Top+1);
        #elif defined(__linux__)
            //Explanation for this code from https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html

            struct winsize win; //Calling the winsize struct from <sys/ioctl.h> as win
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &win); //using the ioctl command to get the attributes of STDOUT_FILENO and write them into the win (winsize) struct
            return win.ws_col; //return the ws_col (n of columns of terminal) attribute from the win struct
        #endif
    }

    //This namespace contains all the functions to read input from the terminal
    //Here the functions are just declared, go to the end of the file to see the actual code.
    namespace in{
        //This function gets a single character from the user and the way it does that can be customized
        extern char get_ch(bool waitForInput = true/*Set to false for the function to not wait for user input*/, bool echo = false/*set to true to show the user the characters printed*/, bool rawMode = true/*Set to false for the rawMode not to be enable (not recommended)*/, bool eSC = true/*set to false to disable special characters (KEY_UP, KEY_DOWN, KEY_F1, KEY_ESCAPE,...)*/, bool eSI = true/*Set to false to disable signal interrupt during input (Ctrl+C / Ctrl+Z)*/);

        //These three functions are used to get a string input from the user and store it in a char array
        //Usage : use the get_str command to get the string from the user, then declare a new char array with the just-filled str_length variable and use store_str(arrayName) to store the input into the array.
        unsigned long str_length = 0;
        extern void get_str(bool echo = true/*read get_ch()*/, bool eSI = true/*read get_ch()*/, bool enterBreaks = true/*set to false to disable the ENTER key terminating input*/, char endChar = '\0'/*set to anything else than 0 to enable a custom char to end the string*/, unsigned int maxLength = 0/*set to anything else than 0 to enable a maximum input length*/);
        extern void store_str(char output[]);
    } //namespace terminal::in;

    //got all the ANSI escape codes from : https://en.wikipedia.org/wiki/ANSI_escape_code

    //This namespace contains all the functions to write output to the terminal
    //Here the functions are just declared, go to the end of the file to see the actual code.
    namespace out{
        //Declared a variadic template to create variadic functions where it can be inputted every type of variable and it will be accepted
        template <typename... args>
        extern void sprint(args... text);   //Used to print a sequence of characters to the terminal without the automatic ENDLINE at the end
                                            //This function will NOT convert integers to char arrays printing them out to the terminal

        template <typename... args>
        extern void sprintln(args... text); //Used to print a sequence of characters to the terminal with the automatic ENDLINE at the end
                                            //This function will NOT convert integers to char arrays printing them out to the terminal

        template <typename... args>
        extern void print(args... text);    //Used to print a sequence of characters to the terminal without the automatic ENDLINE at the end

        template <typename... args>
        extern void println(args... text);  //Used to print a sequence of characters to the terminal with the automatic ENDLINE at the end

        unsigned int print_precision = 6; //When printing a decimal number this integer determines the precision to use when printing it

        extern void printch(char input); //Used to print only a single character to the terminal and accepts only a single char input
        
        //Changes the color of the text written after this command is enabled, accepts either RGB input or normal predefined colors (written as preprocessor code at the start of this header)
        void set_color(int foreground, int background = BLACK, short rf = -1, short gf = -1, short bf = -1, short rb = -1, short gb = -1, short bb = -1){if(rf < 0 || gf < 0 || bf < 0 || rb < 0 || gb < 0 || bb < 0)terminal::out::print("\e[", foreground, ";", background + 10, "m"); else terminal::out::print("\e[38;2;", rf, ";", gf, ";", bf, "\e[48;2;", rb, ";", gb, ";", bb, "m");}
        void set_attribute(int attribute){terminal::out::print("\e[", attribute, "m");} //Enables a certain terminal attribute in the defined ones at the preprocessor code at the start of this header
    } //Namespace terminal::out;

    //This namespace contains all the functions used to manage the cursor on the terminal
    //Here the functions are just declared, go to the end of the file to see the actual code.
    namespace cur{
        void move_up(int n = 1){terminal::out::print("\e[", n, "A");} //Moves the cursor up n times
        void move_down(int n = 1){terminal::out::print("\e[", n, "B");} //Moves the cursor down n times
        void move_forward(int n = 1){terminal::out::print("\e[", n, "C");} //Moves the cursor forward n times
        void move_back(int n = 1){terminal::out::print("\e[", n, "D");} //Moves the cursor back n times

        void set_position(unsigned int x, unsigned int y){terminal::out::print("\e[", x, ";", y, "H");} //Sets a specific cursor position defined by an x and y axis

        void show(){terminal::out::sprint("\e[?25h");} //Shows the cursor (if hidden)
        void hide(){terminal::out::sprint("\e[?25l");} //Hides the cursor (if shown)
    } //Namespace cur;

    //This namespace contains all the functions used to manage the terminal's entire screen
    //Here the functions are just declared, go to the end of the file to see the actual code.
    namespace scr{
        void clear(unsigned short n = 2){terminal::out::print("\e[", n, "J");} //Clears the screen
        void init_alt(){terminal::out::sprint("\e[?1049h");} //Starts an alternate screen for the terminal
        void end_alt(){terminal::out::sprint("\e[?1049l");} //Exits from the alternate terminal screen
        void reset(){terminal::out::sprint("\ec");} //Resets and clears the terminal

        void scroll_up(int n = 1){terminal::out::print("\e[", n, "S");} //Scrolls up on the terminal
        void scroll_down(int n = 1){terminal::out::print("\e[", n, "T");} //Scrolls down on the terminal
    } //Namespace scr;

    //This namespace contains all internal functions which are only used in this header
    namespace internal{
        char *input; //This char pointer is used to save a stored string to then store it into a char array defined by the used (see get_str() function)

        //Namespace used to manage terminal's rawmode
        namespace rawMode{
            static struct termios raw, noRaw; //Calls the termios struct (Containted in <termios.h> library) as raw and noRaw (to be used later)

            void disable(){
                #if defined(_WIN32)
                    //I'm still trying to write a windows version of this program but i still cannot find enough documentation to do this stuff on <windows.h>
                #elif defined(__linux__)
                    //wrote this code using : https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html

                    tcsetattr(STDIN_FILENO, TCSANOW, &noRaw);   //Applying to the terminal the original attributes to exit raw mode
                                                                //TCSANOW is used to apply the attributes immediately after sending the command

                    return;
                #endif
            }//terminal::internal::rawMode::disable();

            void enable(bool echoOn = true, bool SIGSTPINT = true){
                #if defined(_WIN32)

                #elif defined(__linux__)
                    //wrote this code using : https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html

                    tcgetattr(STDIN_FILENO, &raw);      //Reads the STDIN_FILENO (terminal) attributes into the raw (termios) struct
                    tcgetattr(STDIN_FILENO, &noRaw);    //Reads the current terminal attributes into noRaw to be restored later

                    if(!echoOn)raw.c_lflag &= ~(ECHO);                          //c_lflag is for the "local flags" field, if ECHO was requested to be on we turn it on
                                                                                //ECHO is a bitflag, used to print the characters pressed by the user in the terminal, defined as 00000000000000000000000000001000, NOT (~) operator is being used to invert it (11111111111111111111111111110111) and bitwise-AND (&=) it into the flag causing the 29th bit to flip to 0, thus setting the ECHO to OFF

                    if(!SIGSTPINT)raw.c_lflag &= ~(ISIG);                       //ISIG is another bitflag used to disable the CTRL+C (terminate) and CTRL+Z (suspend) sequences
                                                                                //It's another local flag and if it was requested to be off then turn it off.
                                                                
                    raw.c_lflag &= ~(ICANON | IEXTEN);                          //ICANON is used to exit from terminal's canonical (or cooked) mode and entering raw mode to read input byte by byte instead of line by line
                                                                                //IEXTEN is yet another bitflag used to turn off the CTRL-V sequence, used to send bytes directly to the terminal.

                    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);   //c_iflag is for the input flags
                                                                                //IXON Comes from <termios.h> and activates the software flow control using the CTRL+S and CTRL+Q commands, we bitwise-NOT (~) and bitwise-AND (&=) it into the input flags to turn it off
                                                                                //ICRNL is used to disable the CTRL-M sequence which sends a carriage return ('\r') that gets translated by the terminal into a newline ('\n')
                                                                                //
                                                                                //All these other flags are part of entering raw mode even though they are either already turned off by default or don't apply for modern terminal emulators, explenation for these goes as follow:
                                                                                //disabling BRKINT prevents a break condition (like CTRL+C) to send a SIGINT signal to the terminal (i guess it stands for SIGnal INTerrupt)
                                                                                //disabling INPCK turns off parity checking even though it doesn't apply to modern terminal emulators.
                                                                                //disabling ISTRIP prevents the 8th bit of each input to be stripped (set to 0)

                    raw.c_cflag |= (CS8);                                       //CS8 is not a flag, it's part of the c_flags and it's a bitmask with multiple bits, which we set using bitwise-OR (|=), it sets all the character size to be 8 bits per byte, on many sistems it is already like this.



                    raw.c_oflag &= ~(OPOST);                                    //c_oflag is for the output flags
                                                                                //OPOST is an output flag used to disable the output processing by the terminal, likely the only output processing done by the terminal in this case is the conversion from newline ('\n') to newline and carriage return ("\r\n")

                    raw.c_cc[VMIN] = 0;                                         //VMIN is a variable contained under the c_cc array (which stands for "control characters") of the termios struct, it tells the terminal how many bytes we wait for the input to read, setting it to 0 means that the read() function returns as long as there is any input on the terminal
                    raw.c_cc[VTIME] = 1;                                        //VTIME on the other hand, sets the maximum amount of time that read() waits before returning the input, we set it to 1 so that read returns when there is input to read

                    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);   //Applying modified attributes to the terminal, TCSAFLUSH specifies when to apply the changes, in this case it waits for all pending output to be written to the terminal and discards any not read input

                    return;
                #endif
            }//terminal::internal::rawMode::enable();
        }//Namespace terminal::internal::rawMode;

        //Function that executes a power (used in terminal::internal::convert namespace to convert a normal number to a string)
        long double power(long base, long exponent){
            long double output = 1; //Starts with the output as 1 (if the exponent is 0 then the output is 1)
            if(exponent != 0) //If the exponent is not 0 execute the power, otherwise return the outpu which was set to 1
            {
                //Multiply the output by the base as many times as the absolute value of the exponent
                if(exponent > 0)for(long i = 1;i <= exponent;i++)output *= base; 
                else if(exponent < 0) for(long i = 1;i <= exponent * -1;i++)output *= base;

                //If the exponent was negative return 1/output
                return (exponent > 0 ? output : 1.000000000000000L / output);
            } else return output;
        }//terminal::internal::power();

        //Namespace that contains all the code used to convert numbers to char arrays
        namespace convert {
            //Function that returns a single digit of a long double
            int returnSingleDigit(long double input, int digit){
                if(input < 0)input *= -1; //Take the absolute value of the input

                //If the digit is positive then return the asked digit for the decimal part, otherwise return the asked digit for the whole part
                if(digit >= 0){
                    for(int i = 0;i < digit;i++)input /= 10; //Divide the input by 10 as many times as the number of digits (if it's positive)
                } else {
                    for(int i = 0;i > digit;i--)input *= 10; //Else, multiply the input by 10 as many times as the negative number of digits
                }

                return (int)(input - (unsigned long)(input / 10) * 10); //return the output rounded down (since now its got a huge decimal part, delete all of it)
            }//terminal::internal::convert::returnSingleDigit();

            //Function used to get the number of digits of a long double input
            //This is needed so that when print functions have to concatenate variadic arguments into a char array they can use this function to get the size of all the numbers to print into the array and sum them
            //This is more or less a copy of the toCharArray() function
            int numberOfDigits(long double input){
                unsigned int nOfDigits; //used to store the number of whole digits
                unsigned int nOfDecimals; //used to store the number of decimal digits
                unsigned int nOfTotalDigits; //The sum of the two previous variables

                long double absInput = (input < 0 ? input * -1 : input); //Store the absolute value of the input (invert the input if it's less than 0 since that can cause problems during reading it)

                for(nOfDigits = 1;true;nOfDigits++){
                    //IF the input is less than 10 to the power of the current digits then it means that we found the number of digits of the input and we can break the loop
                    if(absInput < terminal::internal::power(10, nOfDigits))break;
                }
                if(input < 0)nOfDigits++; //Add one digit to the total number of them if the number is negative (for the minus sign)

                nOfTotalDigits = nOfDigits; //For now set the number of total digits to the number of digits (now we'll add the decimals)
                        
                if((long double)absInput > (long)absInput && (long double)absInput < (long)absInput + 1){ //Test if the input number is decimal to see if the (long double) form is higher than the long form (if its higher it means that it has some decimals in the long double to make it higher)
                    nOfDecimals = terminal::out::print_precision; //Initially the number of decimals is defined by the default print_precision which can be changed

                    for(int i = terminal::out::print_precision + 1;i > 0;i--){ //For loop that sets iterator as print precision and then decreases it till 0
                        if(returnSingleDigit(input, (i - 1) * -1) == 0)nOfDecimals--; //If the current decimal digit of the number is 0 then decrease the number of decimal digits
                        else break; //If the current decimal digit is a number then just end the loop and print all the decimal places from here
                    }

                    nOfTotalDigits += ++nOfDecimals + 1; //Add the number of decimals we just got to the total number of digits (add one to it to account for the decimal point)
                }

                return nOfTotalDigits; //Return the total number of digits
            }//terminal::internal::convert::numberOfDigits();

            //This function is used to convert a long double to a char array trough pointers
            //This function is used in sumAll() to sum all the variadic arguments of the print functinos into a single char array, this gets called when the input of sumAll is a number and converts is to a char array appending it to the output
            //This is more or less a copy of the numberOfDigits() function just that it stores more stuff along the way to use it during copying into char array
            void toCharArray(long double input, char *output, unsigned int output_length){
                const char negativeSign = '-'; //defining a character for the negative number sign
                const char decimalSign = '.'; //defining a character for the decimal point sign

                unsigned int nOfDigits; //Used to store the number of whole digits
                unsigned int nOfDecimals; //Used to store the number of decimal digits
                unsigned int nOfTotalDigits; //Used to store the total number of digits
                
                bool negative = false; //Used to store wether the number is negative or not to append the const char negativeSign at the beginning of the number
                bool decimal = false; //Used to store wether the number got a decimal part to insert the const char decimalSign into the number

                if(input < 0)negative=true; //Set negative to true if the number is less than 0
                
                //Find the number of digits for the whole part of the number
                for(nOfDigits = 1;true;nOfDigits++)
                    if( //TL;DR this just tests if the number is less than 10 to the power of the iterator then just breaks the loop knowing the iterator is equal to the number of digits
                        (negative ? input * -1 : input) < (terminal::internal::power(10, nOfDigits)) //If the absolute value of the input is less than the power of 10 to the current iterator of the loop then break it)
                    )break; //If the number exceeds the power between the number of 10 and the iterator then stop the loop
                
                if(negative)nOfDigits++; //If the number is negative add 1 to the number of digits to count for the minus sign

                nOfTotalDigits = nOfDigits; //We found the number of digits for the whole part, add it to the total number of digits

                //Find the number of digits for the decimal part of the number
                if( //TL;DR this just tests if the number got a decimal part
                    (long double)input > (long)input        //If the long double form of the number is higher than the integer form of the same number
                        &&                                  //AND
                    (long double)input < (long)input + 1    //If the long double form of the number is lower than the integer form of the same number + 1
                    ||                                  //OR (This because the number can be either positive or negative)
                    (long double)input < (long)input        //If the long double form of the number is lower than the integer form of the same number
                        &&                                  //AND
                    (long double)input > (long)input - 1    //If the long double form of the number is higher than the integer form of the same number + 1
                ){//Then the number has a decimal part
                    nOfDecimals = terminal::out::print_precision; //We start with the number of decimals equal to the print precision set in the code and we're gonna decrease it.
                    decimal = true; //Since this code is reached only if the number got a decimal part then set decimal to true

                    for(int i = terminal::out::print_precision + 1;i > 0;i--){ //For loop to start from the last digit (according to print_precision) and going down till we find a digit
                        if(returnSingleDigit(input, (i - 1) * -1) == 0)nOfDecimals--; //Decrease nOfDecimals if no digits found (if the number at that position is 0)
                        else break; //If a valid digit is found then close the loop
                    }

                    nOfTotalDigits += nOfDecimals + 1; //Add the nOfDecimals to the total length of the number (add one to it to account for the decimal point)
                }

                char converted[nOfTotalDigits]; //This char array will contain the converted number

                if(negative)converted[0] = negativeSign; //If the number is negative the first char of the array will be the negative sign
                for(int i = (unsigned int)negative;i < nOfDigits;i++)converted[i] = (char)returnSingleDigit(input, nOfDigits - i - 1) + 48; //Iterate trough the number of whole digits and for every whole digits add to this function the single digit casted to char and converted it into a number

                if(decimal){ //If the number got a decimal part
                    converted[nOfDigits] = decimalSign; //At the end of the whole part (so at the nOfDigits position since thats where the last for loop stopped iterating)
                    for(int i = 1;i <= nOfDecimals;i++)converted[nOfDigits + i] = returnSingleDigit(input, i * -1) + 48; //to this part add every decimal character of the number
                }

                for(int i = 0;i <= nOfTotalDigits && i < output_length;i++)*(output + i) = converted[i]; //Then append all the converted stuff to the output trough pointer

                return;
            }//terminal::internal::convert::toCharArray();
        }//Namespace terminal::internal::convert;

        namespace concat { //This namespace contains all the functions used to concatenate the variadic arguments took as an input for print functions
            //This function is used to get the sum of the length of all the variadic arguments
            //This is the const char part, since variadic arguments can be of different data types this is the one that gets the length of the const char as an input and will be used to sum it to the total
            int sumAllLength(const char input[]){ 
                int inputLength; //Create a variable
                for(inputLength = 0;input[inputLength] != '\0';inputLength++); //use a for loop to iterate trough the char array till we find a '\0' character indicating the end of it
                return inputLength; //When the for loop ended and we reached the end of the char array then return the length we found
            } //terminal::internal::concat::sumAllLength(const char)

            //This overloaded function is used to get the length of a long double number, which will then be summed to the rest of the numbers
            //It just returns the numberOfDigits function defined in convert namespace
            int sumAllLength(long double input){
                return terminal::internal::convert::numberOfDigits(input);
            } //terminal::internal::concat::sumAllLength(long double)

            //This function is used to copy all the variadic arguments into the char array
            //This takes a const char array as input and outputs it to an output trough pointer
            void sumAll(const char input[], char *output, bool onlyChar = false){ 
                int start; //Create a variable to know where to start copying the const char to the output without overwriting stuff
                for(start = 0;true;start++)if(*(output + start) == 0)break; //Iterate trough the output array and find where it is empty so that we can copy it

                for(int i = 0;input[i] != '\0';i++)*(output + start + i) = input[i]; //Copy the const char into the output starting from where we found it is empty

                return; //End the function after copying
            } //terminal::internal::concat::sumAll(const char)

            //This function is an overloading of the other one
            //What it does is converting a long double number as an input to a char array then it copies it to the output trough pointer
            void sumAll(long double input, char *output, bool onlyChar = false){
                if(onlyChar)return; //If the function was asked only to convert char inputs then skip this code

                int char_length = terminal::internal::convert::numberOfDigits(input); //Set the length of the char array to the length in digits of the number
                char char_input[char_length]; //Create a char array with the found length

                terminal::internal::convert::toCharArray(input, char_input, char_length); //Convert the number to char array and output it to the just created one

                int start; //Find the start of the output pointer
                for(start = 0;true;start++)if(*(output + start) == 0)break; //Iterate trough the output to see where can we start to copy into it

                for(int i = 0;i < char_length;i++)*(output + start + i) = char_input[i]; //Copy the array starting from the start we found

                return; //Finish the function
            } //terminal::internal::concat::sumAll(long double)
        } //namespace terminal::internal::concat;
    } //namespace terminal::internal;
} //namespace terminal;


//This function is used to get a string and temporarily storing it into a char pointer defined in terminal::internal
void terminal::in::get_str(bool echo, bool eSI, bool enterBreaks, char endChar, unsigned int maxLength){
    bool outputN = false; //Since this code uses two different char arrays and copies between them to update them, this variable stores what array we're currently using

    char* output1; //Creating the two arrays that will get updated in real time
    char* output0;

    for(char current_char;true;current_char = terminal::in::get_ch(true, echo, true, false, eSI)){ //This loop saves every read character to the current_char variable
        if(current_char == '\n' && enterBreaks)break; //If the read character is \n and the user asked for enter to break the string then do it
        if(current_char == endChar && endChar != '\0')break; //If the current read character is equal to the user-defined one then break the string
        if(terminal::in::str_length > maxLength && maxLength > 0)break; //If the string length exceeds the maximum one defined by the user break the string

        if(current_char == BACKSPACE){ //If the read character is backspace:
            if(terminal::in::str_length < 1)continue; //If we hit backspace when the string is empty don't do anything

            if(outputN == 0)*(output0 + --terminal::in::str_length) = '\0'; //Clear the last character of the string 0 if outputN is 0
            else if(outputN == 1)*(output1 + --terminal::in::str_length) = '\0'; //If outputN is 1 then clear the last character of the string 1

            //After having modified the string move the cursor back, print a whitespace to clear the actual character on screen and then move the cursor back again
            terminal::cur::move_back();
            terminal::out::printch(' ');
            terminal::cur::move_back();
            
            continue; //Continue with the loop
        }

        if(outputN == 1){ //If the string 1 is selected:
            output1 = new char[++terminal::in::str_length]; //create a new char array as the incremented str_length in string 1
            outputN = !outputN; //Invert outputN (this sets the selected string to 0)

            for(unsigned int i = 0;i < terminal::in::str_length - 1;i++)*(output1 + i) = *(output0 + i); //Copy everything from the string 0 to string 1
            *(output1 + terminal::in::str_length - 1) = current_char; //Set the last character of string 1 as the just read one 

            delete output0; //Delete the old string 0
        } else if(outputN == 0){ //But if the string 0 is selected instead
            output0 = new char[++terminal::in::str_length]; //create a new char array as the incremented str_length in string 0
            outputN = !outputN; //Invert outputN (this sets the selected string to 1)

            for(unsigned int i = 0;i < terminal::in::str_length - 1;i++)*(output0 + i) = *(output1 + i); //Copy everything from the string 1 to string 0
            *(output0 + terminal::in::str_length - 1) = current_char; //Set the last character of string 0 as the just read one

            delete output1; //Delete the old string 1
        }
    }

    delete terminal::internal::input; //Delete the old variable for the input that contains the old stuff
    terminal::internal::input = new char[terminal::in::str_length]; //Create a new input variable

    for(unsigned int i = 0;i < terminal::in::str_length;i++)*(terminal::internal::input + i) = (outputN ? *(output0 + i) : *(output1 + i)); //Copy all the stuff from one of the strings to the input variable

    return; //end the code
} //terminal::in::get_str()

//This variable is used to copy terminal::input saved during get_str into a char array output trough pointer
void terminal::in::store_str(char* output){
    for(int i = 0;i < terminal::in::str_length;i++)*(output + i) = *(terminal::internal::input + i); //Copying terminal::internal::input char array pointer to the output pointer
    *(output + terminal::in::str_length) = '\0'; //Set the last char of output to \0

    terminal::in::str_length = 0; //Reset str_length
    delete terminal::internal::input; //Delete the input since we don't need it anymore

    return; //End the function
} //terminal::in::store_str()

//This code gets a single character from the terminal
char terminal::in::get_ch(bool waitForInput, bool echo, bool rawMode, bool eSC, bool eSI){
    #if defined(_WIN32) //The windows code still isn't available, i'm going to work on it soon
    #elif defined(__linux__) //Linux code
        if(rawMode)terminal::internal::rawMode::enable(echo, eSI); //If rawmode was requested then enable rawmode with the other parameters

        char input[4];  //Create a char array containing the 4 characters we're gonna read from the terminal
                        //We're reading 4 chars from the terminal even tho we're gonna get one char only because the other chars are used for defining special characters like KEY_UP, KEY_DOWN and stuff like that
        for(int i = 0;i < 4;i++)input[i] = 0; //Reset the just-created array to all zeroes

        if(waitForInput)while(!read(STDIN_FILENO, &input, 4)); //If waitForInput is turned on start a while loop and till the read() function doesn't return don't do anything
        else read(STDIN_FILENO, &input, 4); //If waitForInput isn't turned on then just return what read from the terminal wether the user pressed a key or not

        if(rawMode)terminal::internal::rawMode::disable(); //If rawmode was requested we can disable it now

        if(eSC){ //If special characters are enabled then look out for them.
            if(input[0] == '\e' && input[1] == 79){ //If the first char of the char array is an escape character and the second one's code is '79' then the input keyboard is the XFree4 one, start the decoding for that keyboard
                //Special key decoding for the XFree4 keyboard (default linux terminal keyboard)

                switch(input[2]){ //Look for what's on input[2] and return accordingly
                    case 51 : if(input[3] == 126)return KEY_DELETE;
                    case 53 : if(input[3] == 126)return KEY_PGUP;
                    case 54 : if(input[3] == 126)return KEY_PGDOWN;

                    case 65 : return KEY_UP;
                    case 66 : return KEY_DOWN;
                    case 67 : return KEY_RIGHT;
                    case 68 : return KEY_LEFT;

                    case 69 : return KEY_CENTER;
                    case 70 : return KEY_ENDL;
                    case 72 : return KEY_STARTL;

                    case 80 : return KEY_F1;
                    case 81 : return KEY_F2;
                    case 82 : return KEY_F3;
                    case 83 : return KEY_F4;

                    case 49 : {
                        switch(input[3]){
                            case 53 : return KEY_F5;
                            case 55 : return KEY_F6;
                            case 56 : return KEY_F7;
                            case 57 : return KEY_F8;
                        }
                    }

                    case 50 : {
                        switch(input[3]){
                            case 48 : return KEY_F9;
                            case 49 : return KEY_F10;
                            case 51 : return KEY_F11;
                            case 52 : return KEY_F12;

                            case 126 : return KEY_INSERT;
                        }
                    }
                }
            } else if(input[0] == '\e' && input[1] == 91){ //But if the first character is an escape one and the second char's code is '91' then we're working with a linux console / macOS keyboard, we'll start the decoding for it
                //Special key decoding for the linux console / macOS keyboard

                switch(input[2]){
                    case 52 : if(input[3] == 126)return KEY_ENDL;
                    case 51 : if(input[3] == 126)return KEY_DELETE;
                    case 53 : if(input[3] == 126)return KEY_PGUP;
                    case 54 : if(input[3] == 126)return KEY_PGDOWN;

                    case 65 : return KEY_UP;
                    case 66 : return KEY_DOWN;
                    case 67 : return KEY_RIGHT;
                    case 68 : return KEY_LEFT;
                    case 71 : return KEY_CENTER;

                    case 80 : return KEY_PAUSE;

                    case 91 : {
                        switch(input[3]){
                            case 65 : return KEY_F1;
                            case 66 : return KEY_F2;
                            case 67 : return KEY_F3;
                            case 68 : return KEY_F4;
                            case 69 : return KEY_F5;
                        }
                    }

                    case 49 : {
                        switch(input[3]){
                            case 55 : return KEY_F6;
                            case 56 : return KEY_F7;
                            case 57 : return KEY_F8;

                            case 126 : return KEY_STARTL;
                        }
                    }

                    case 50 : {
                        switch(input[3]){
                            case 48 : return KEY_F9;
                            case 49 : return KEY_F10;
                            case 51 : return KEY_F11;
                            case 52 : return KEY_F12;

                            case 126 : return KEY_INSERT;
                        }
                    }
                }
            } else if(input[0] == '\e' && input[1] == 0)return KEY_ESCAPE; //If the first character is an escape one and the second one's code is 0 then the pressed key is ESCAPE so return it
            else return input[0]; //If no escape characters found or no valid keyboard found then just return the first control character
        } else if(!eSC && input[0] >= 32 && input[0] != 127)return input[0];    //ASCII codes 0-31 and 127 are all control characters so we ignore em in the output
                                                                                //We'll just return a normal character if the special characters weren't enabled
    #endif //End of checking for linux system

    return '\0'; //If special characters were disabled and the input was a special character return \0
} //terminal::in::get_ch()

//This function just prints out variadic char array input, it doesn't convert numbers to char array, it just skips them and it doesn't add an ENDLINE at the end of the printed char array so the text won't go to a newline after finishing
template <typename... args> //Creating a variadic template to make this function work with any data type
void terminal::out::sprint(args... text){
    int totSize = (sizeof(text) + ...);  //Creating a variable called totSize and it will contain the number of characters contained into all the char arrays
    char totText[totSize]; //Creating a char array as the just found size
    for(int i = 0;i < totSize;i++)totText[i] = 0; //Clearing all the char array setting all its values to 0

    //Expanding the char arrays into sumAll copying all their content into totText
    using expander = int[]; 
    (void)expander{0, ((void)terminal::internal::concat::sumAll(text, totText, true /*Telling sumAll to not accept numbers and only accept char arrays*/), 0)...}; 

    #if defined(_WIN32)
    #elif defined(__linux__) //If linux is defined write totText to terminal
        write(STDOUT_FILENO, totText, totSize);
    #endif

    return; //End the function
} //terminal::out::sprint()

//This function prints out variadic char array input but it doesn't convert numbers into char array, it just ignores them, and it prints an ENDLINE at the end so the cursor goes to the next line
template <typename... args> //Creating a variadic template so that the function accepts any number of any type of input
void terminal::out::sprintln(args... text){
    int totSize = (sizeof(text) + ...); //Creating a variable called totSize and it will contain the number of characters contained into all the char arrays
    char totText[totSize]; //Creating a char array as the just found size
    for(int i = 0;i < totSize;i++)totText[i] = 0; //Clearing all the char array setting all its values to 0

    //Expanding the char arrays into sumAll copying all their content into totText
    using expander = int[]; 
    (void)expander{0, ((void)terminal::internal::concat::sumAll(text, totText, true/*Telling sumAll to not accept numbers and only accept char arrays*/), 0)...}; 

    #if defined(_WIN32)
    #elif defined(__linux__)//If linux is defined write totText to terminal and add an ENDLINE at the end
        write(STDOUT_FILENO, totText, totSize);
        write(STDOUT_FILENO, ENDLINE, 2);
    #endif

    return; //End the function
} //terminal::out::sprintln()

//This function prints out a char array to the terminal taking as an input any data type in a variadic argument form converting all of it into char array printing out it to the terminal
template <typename... args> //Creating a variadic template to accept any number of any data type of input
void terminal::out::print(args... text){ //Creating the function using the variadic template as an input
    int totSize = (terminal::internal::concat::sumAllLength(text) + ...); //Summing the size in characters of all the values of the variadic arguments
    char totText[totSize]; //Creating a char array that will contain all the characters of the variadic arguments
    for(int i = 0;i < totSize;i++)totText[i] = 0; //Clearing the array by filling it with zeroes

    //Expanding all the arguments into sumAll to write them into totText
    using expander = int[]; 
    (void)expander{0, ((void)terminal::internal::concat::sumAll(text, totText), 0)...};

    #if defined(_WIN32)
    #elif defined(__linux__) //If the code is running on linux
        write(STDOUT_FILENO, totText, totSize); //Write the contents of totText[] to the terminal
    #endif

    return; //End the funcion
} //terminal::out::print()

//This function takes any number of any data type of input concatening all of it together into a single char array printing it out to the terminal with a newline at the end
template <typename... args> //Creating a variadic template to accept any number of any data type of input
void terminal::out::println(args... text){ //Creating the function using the variadic template as an input
    int totSize = (terminal::internal::concat::sumAllLength(text) + ...);//Summing the size in characters of all the values of the variadic arguments
    char totText[totSize];//Creating a char array that will contain all the characters of the variadic arguments
    for(int i = 0;i < totSize;i++)totText[i] = 0;//Clearing the array by filling it with zeroes

    //Expanding all the arguments into sumAll to write them into totText
    using expander = int[];
    (void)expander{0, ((void)terminal::internal::concat::sumAll(text, totText), 0)...}; 

    #if defined(_WIN32)
    #elif defined(__linux__)//If the code is running on linux
        write(STDOUT_FILENO, totText, totSize); //Write the contents of totText[] to the terminal
        write(STDOUT_FILENO, ENDLINE, 2); //Add an ENDLINE at the end to go to a newline
    #endif

    return; //End the function
} // terminal::out::println()

//This function prints a single character to the terminal taking a char as an input
void terminal::out::printch(char input){
    //Converting the input char to a char array, this because the write() function for the buffer takes a char array modifying it trough pointer and it doesn't accept a normal char as input and we have to pass it trough a char array
    char output[1]; //Creating a char array to use as an output
    output[0] = input; //Setting the first char of the array to the input of the function

    #if defined(_WIN32)
    #elif defined(__linux__) //If we're on linux
        write(STDOUT_FILENO, output, 1); //Writing the char passed to char array to the terminal
    #endif

    return; //End the function
} //terminal::out::printch()

#endif //ifndef customTerminalIO_HPP
