/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "statement.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "statement.h"
#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"

using namespace std;

/* Function prototypes */

Statement *statetype_jg(std::string line) {
    if (line[0] == 'E' && line[1] == 'N' && line[2] == 'D')//END
        return new END(line);
    if (line[0] == 'R' && line[1] == 'E' && line[2] == 'M')//REM
        return new REM(line);
    if (line[0] == 'G' && line[1] == 'O' && line[2] == 'T')//GOTO
        return new GOTO(line);
    if (line[0] == 'I' && line[1] == 'F')//IF
        return new IF(line);
    if (line[0] == 'L' && line[1] == 'I' && line[2] == 'S')//LIST
        return new LIST(line);
    if (line[0] == 'C' && line[1] == 'L' && line[2] == 'E')//CLEAR
        return new CLEAR(line);
    if (line[0] == 'L' && line[1] == 'E' && line[2] == 'T')//LET
        return new LET(line);
    if (line[0] == 'P' && line[1] == 'R' && line[2] == 'I')//PRINT
        return new PRINT(line);
    if (line[0] == 'I' && line[1] == 'N' && line[2] == 'P')//INPUT
        return new INPUT(line);
}

/* Main program */
std::string input_variable;
EvalState state;
Program program;
RUN run;//摆设，没用的

int main() {
    int num, iter;
    while (true) {
        try {
            string input, line;
            input = getLine();
            if (input.empty())
                return 0;
            if (input[0] == 'Q' && input[1] == 'U')//QUIT
                return 0;
            if (input[0] >= '0' && input[0] <= '9') {//有行号
                num = 0;
                for (iter = 0; iter < input.length(); ++iter) {
                    if (input[iter] == ' ') break;
                    else num = num * 10 + int(input[iter] - '0');
                }
                if (iter == input.length()) {
                    program.removeSourceLine(num);
                    continue;
                }
                line = input.substr(iter + 1);
                program.addSourceLine(num, line);
            } else {//没行号
                if (input[0] == 'R' && input[1] == 'U' && input[2] == 'N')
                    run.execute(program, state);
                else {
                    Statement *st = statetype_jg(input);
                    st->execute(program, state, run);
                    delete st;
                }
            }
        } catch (ErrorException &ex) {
            cerr << ex.getMessage() << endl;
        }
    }
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

