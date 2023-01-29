#ifndef evaluateExpression_HPP
#define evaluateExpression_HPP

#include <string>

#define ADDITION       -1
#define SUBTRACTION    -2
#define MULTIPLICATION -3
#define DIVISION       -4
#define POWER          -5
#define EMPTY          -6
#define NEGATIVE       -7

#define OPEN_PARENTHESYS   64
#define CLOSE_PARENTHESYS  96

namespace expression {
    namespace internal {
        struct {
            unsigned int componentsAmount;
            unsigned int currentComponent;
            unsigned int digitsSinceDecimalPoint;

            long double *buffer;

            bool decimalPointReached;
            bool isFirstNumber;

            bool isan(char input){return (input > 47 && input < 58);}
            bool isas(char input){return (input == '+' || input == '-' || input == '*' || input == '/' || input == '^' || input == '<');}

            bool isNegative(double input){return (input < 0);}
        } expression;

        int absolute(int input){
            if(input >= 0)return input;
            else return (input *= -1);
        }

        double power(int base, int exponent){
            int output = 1;

            for(int i = 0;i < expression::internal::absolute(exponent);++i){
                output *= base;
            }

            if(expression.isNegative(exponent))return (1.000000 / output);
            else return output;
        }

        int round(double input){
            int roundedDownInput = int(input);
            if(input - roundedDownInput <= 0.5)return roundedDownInput;
            else return ++roundedDownInput;
        }

        long double evaluateSimpleExpression(std::string input){
            expression.componentsAmount = 0;
            expression.currentComponent = 0;
            expression.digitsSinceDecimalPoint = 0;

            expression.decimalPointReached = false;

            for(int i = 0;i <= input.length();++i){
                if(expression.isas(input[i])){
                    if(input[i] == '<'){expression.componentsAmount++;continue;}
                    expression.componentsAmount += 2;
                    continue;
                } else continue;
            }

            expression.buffer = new long double[++expression.componentsAmount];

            for(int i = 0;i < input.length();++i){
                if(expression.isas(input[i])){
                    expression.decimalPointReached = false;
                    expression.isFirstNumber = true;
                    expression.digitsSinceDecimalPoint = 0;
                    if(input[i]!='<')expression.currentComponent++;

                    switch(input[i]){
                        case '+' : *(expression.buffer + expression.currentComponent++) = ADDITION;break;
                        case '-' : *(expression.buffer + expression.currentComponent++) = SUBTRACTION;break;
                        case '*' : *(expression.buffer + expression.currentComponent++) = MULTIPLICATION;break;
                        case '/' : *(expression.buffer + expression.currentComponent++) = DIVISION;break;
                        case '^' : *(expression.buffer + expression.currentComponent++) = POWER;break;
                        case '<' : *(expression.buffer + expression.currentComponent++) = NEGATIVE;break;
                    }

                    continue;
                } else if(input[i] == '.'){
                    expression.decimalPointReached = true;

                    continue;
                } else if(expression.isan(input[i])) {
                    if(*(expression.buffer + expression.currentComponent) < 0)*(expression.buffer + expression.currentComponent) = 0;

                    if(expression.isFirstNumber && !expression.decimalPointReached && input[i] == '0'){
                        continue;
                    } else {
                        expression.isFirstNumber = false;

                        if(!expression.decimalPointReached){
                            *(expression.buffer + expression.currentComponent) = *(expression.buffer + expression.currentComponent) * 10 + (input[i] - 48);
                        } else {
                            *(expression.buffer + expression.currentComponent) += (input[i] - 48) * (1.000000000000000L / expression::internal::power(10, ++expression.digitsSinceDecimalPoint));
                        }

                        continue;
                    }

                } else continue;
            }

            for(int i = 0;i < expression.componentsAmount;++i){
                if(*(expression.buffer + i) == POWER){
                    int base;
                    int exponent;

                    bool negativeBase = false;
                    bool negativeExponent = false;

                    if(*(expression.buffer + i - 1) < 0){
                        for(int j = i;j >= 0;j--){
                            if(*(expression.buffer + j - 1) < 0)continue; else {
                                base = round(*(expression.buffer + j - 1));
                                *(expression.buffer + j - 1) = EMPTY;
                                if(*(expression.buffer + j - 2) == NEGATIVE)negativeBase = true;
                                
                                break;
                            }
                        }
                    } else {
                        base = expression::internal::round(*(expression.buffer + i - 1));
                        *(expression.buffer + i - 1) = EMPTY;

                        if(*(expression.buffer + i - 2) == NEGATIVE)negativeBase = true;
                    }
                    
                    if(*(expression.buffer + i + 1) < 0){
                        for(int j = i;j <= expression.componentsAmount;j++){
                            if(*(expression.buffer + j + 1) < 0)continue; else {
                                exponent = round(*(expression.buffer + j + 1));
                                *(expression.buffer + j + 1) = EMPTY;
                                if(*(expression.buffer + j) == NEGATIVE)negativeExponent = true;

                                break;
                            }
                        }
                    } else {
                        exponent = expression::internal::round(*(expression.buffer + i + 1));
                        *(expression.buffer + i + 1) = EMPTY;
                    }

                    if(negativeBase)base *= -1;
                    if(negativeExponent)exponent *= -1;

                    *(expression.buffer + i) = expression::internal::power(base, exponent);
                }
            }

            for(int i = 0;i < expression.componentsAmount;++i){
                if(*(expression.buffer + i) == MULTIPLICATION || *(expression.buffer + i) == DIVISION){
                    long double firstOperator;
                    long double secondOperator;
                    bool firstOperatorNegative = false;
                    bool secondOperatorNegative = false;

                    if(*(expression.buffer + i - 1) < 0){
                        for(int j = i;j > 0;j--){
                            if(*(expression.buffer + j - 1) < 0)continue; else {
                                firstOperator = *(expression.buffer + j - 1);
                                *(expression.buffer + j - 1) = EMPTY;

                                if(*(expression.buffer + j - 2) == NEGATIVE){
                                    firstOperatorNegative = true;
                                    *(expression.buffer + j - 2) = EMPTY;
                                }

                                break;
                            }
                        }
                    } else {
                        firstOperator = *(expression.buffer + i - 1);
                        *(expression.buffer + i - 1) = EMPTY;

                        if(*(expression.buffer + i - 2) == NEGATIVE){
                            firstOperatorNegative = true;
                            *(expression.buffer + i - 2) = EMPTY;
                        }
                    }
                    
                    if(*(expression.buffer + i + 1) < 0){
                        for(int j = i;j <= expression.componentsAmount;j++){
                            if(*(expression.buffer + j + 1) < 0)continue; else {
                                secondOperator = *(expression.buffer + j + 1);
                                *(expression.buffer + j + 1) = EMPTY;

                                if(*(expression.buffer + j) == NEGATIVE){
                                    secondOperatorNegative = true;
                                    *(expression.buffer + j) = EMPTY;
                                }

                                break;
                            }
                        }
                    } else {
                        secondOperator = *(expression.buffer + i + 1);
                        *(expression.buffer + i + 1) = EMPTY;
                    }

                    long double result;

                    if(firstOperatorNegative)firstOperator *= -1;
                    if(secondOperatorNegative)secondOperator *= -1;

                    if(*(expression.buffer + i) == MULTIPLICATION) result = firstOperator * secondOperator;
                    else if(*(expression.buffer + i) == DIVISION) result = firstOperator / secondOperator;

                    if(expression.isNegative(result)){
                        *(expression.buffer + i) = (result * (-1));
                        *(expression.buffer + i - 1) = NEGATIVE;
                    } else {
                        *(expression.buffer + i) = result;
                    }
                }
            }

            for(int i = 0;i < expression.componentsAmount;++i){
                if(*(expression.buffer + i) == ADDITION || *(expression.buffer + i) == SUBTRACTION){
                    long double firstOperator;
                    long double secondOperator;
                    bool firstOperatorNegative = false;
                    bool secondOperatorNegative = false;

                    if(*(expression.buffer + i - 1) < 0){
                        for(int j = i;j > 0;j--){
                            if(*(expression.buffer + j - 1) < 0)continue; else {
                                firstOperator = *(expression.buffer + j - 1);
                                *(expression.buffer + j - 1) = EMPTY;

                                if(*(expression.buffer + j - 2) == NEGATIVE){
                                    firstOperatorNegative = true;
                                    *(expression.buffer + j - 2) = EMPTY;
                                }

                                break;
                            }
                        }
                    } else {
                        firstOperator = *(expression.buffer + i - 1);
                        *(expression.buffer + i - 1) = EMPTY;

                        if(*(expression.buffer + i - 2) == NEGATIVE){
                            firstOperatorNegative = true;
                            *(expression.buffer + i - 2) = EMPTY;
                        }
                    }
                    
                    if(*(expression.buffer + i + 1) < 0){
                        for(int j = i;j < expression.componentsAmount;j++){
                            if(*(expression.buffer + j + 1) < 0)continue; else {

                                secondOperator = *(expression.buffer + j + 1);
                                *(expression.buffer + j + 1) = EMPTY;

                                if(*(expression.buffer + j) == NEGATIVE){
                                    secondOperatorNegative = true;
                                    *(expression.buffer + j) = EMPTY;
                                }

                                break;
                            }
                        }
                    } else {
                        secondOperator = *(expression.buffer + i + 1);
                        *(expression.buffer + i + 1) = EMPTY;
                    }

                    long double result;

                    if(firstOperatorNegative)firstOperator *= -1;
                    if(secondOperatorNegative)secondOperator *= -1;
                    
                    if(*(expression.buffer + i) == ADDITION) result = firstOperator + secondOperator;
                    else if(*(expression.buffer + i) == SUBTRACTION) result = firstOperator - secondOperator;

                    if(expression.isNegative(result)){
                        *(expression.buffer + i) = (result * (-1));
                        *(expression.buffer + i - 1) = NEGATIVE;
                    } else {
                        *(expression.buffer + i) = result;
                    }
                }
            }

            for(int i = 0;i < expression.componentsAmount;++i){
                if(*(expression.buffer + i) >= 0){
                    long double output = *(expression.buffer + i);
                    if(*(expression.buffer + i - 1) == NEGATIVE)output *= -1;
                    delete expression.buffer;
                    return output;
                }
            }

            return EMPTY;
        }

        std::string copyStringSegment(std::string input, int start, int end){
            std::string output;
            output.clear();

            for(int i = start;i <= end;i++){
                output += input.at(i);
            }

            return output;
        }

        struct{
            unsigned int highestParenthesysLevel;
            unsigned int currentParenthesysLevel;
            unsigned int openedParenthesysAddress;

            std::string parenthesysContent;
        } evaluate;
    }

    long double evaluate(std::string input){
        internal::evaluate.highestParenthesysLevel = 0;
        internal::evaluate.currentParenthesysLevel = 0;
 
        for(int i = 0;i < input.length();++i){
            if(input[i]=='('){
                input[i] = (char)(64 + ++internal::evaluate.currentParenthesysLevel);
                if(internal::evaluate.currentParenthesysLevel > internal::evaluate.highestParenthesysLevel)internal::evaluate.highestParenthesysLevel = internal::evaluate.currentParenthesysLevel;
            } else if(input[i]==')'){
                input[i] = (char)(96 + internal::evaluate.currentParenthesysLevel--);
            }
        }

        for(int level = internal::evaluate.highestParenthesysLevel;level > 0;level--){
            for(int i = 0;i < input.length();i++){
                if(input[i] == (OPEN_PARENTHESYS + level))internal::evaluate.openedParenthesysAddress = i;
                if(input[i] == (CLOSE_PARENTHESYS + level) && internal::evaluate.openedParenthesysAddress != 0){
                    internal::evaluate.parenthesysContent = input.substr(internal::evaluate.openedParenthesysAddress + 1, i - internal::evaluate.openedParenthesysAddress - 1); //Copying the content of the parenthesys into a separate string
                    input.erase(internal::evaluate.openedParenthesysAddress, i - internal::evaluate.openedParenthesysAddress + 1); //erasing the content of the parenthesys from the main string
                    input.insert(internal::evaluate.openedParenthesysAddress, std::to_string(internal::evaluateSimpleExpression(internal::evaluate.parenthesysContent))); //sobstituting the content of the parenthesys with the evaluated output of it

                    internal::evaluate.openedParenthesysAddress = 0;
                    internal::evaluate.parenthesysContent.clear();
                }
            }
        }

        return internal::evaluateSimpleExpression(input);
    }
}

#endif