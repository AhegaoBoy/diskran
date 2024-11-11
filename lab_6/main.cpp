#include <iostream>
#include <sstream>
#include "big_integer.h"

int main()
{
    big_integer operand_1("0");
    big_integer operand_2("0");



    while(!std::cin.eof())
    {
        std::cin>>operand_1>>operand_2;
        char operation = 0;

        std::cin>>operation;

        switch (operation) {
            case '+':
                std::cout << operand_1 + operand_2 << std::endl;
                break;

            case '-':
            {
                std::stringstream ss;
                ss << operand_1 - operand_2;
                if(ss.str()[0] == '-')
                    std::cout<<"Error"<<std::endl;
                else
                    std::cout<<ss.str()<<std::endl;
                break;
            }
            case '*':
                std::cout<<operand_1 * operand_2<<std::endl;
                break;
            case '/':
                std::cout<<operand_1 / operand_2<<std::endl;
                break;
            case '>':
                std::cout<<((operand_1 > operand_2) ? "true" : "false")<<std::endl;
                break;
            case '<':
                std::cout<<((operand_1 < operand_2) ? "true" : "false")<<std::endl;
                break;
            case '=':
                std::cout<<((operand_1 == operand_2) ? "true" : "false")<<std::endl;
                break;
            case '^':
                break;
            default:
                std::cout<<"Idi nahui"<<std::endl;
                break;
        }
    }
}
