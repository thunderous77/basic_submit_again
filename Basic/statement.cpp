/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
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
#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"

using namespace std;

/* Implementation of the Statement class */
Statement *statetype_judge(std::string line) {
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

void RUN::runline(std::string line, Program &program, EvalState &state) {
    if (line == "") {
        cout << "LINE NUMBER ERROR" << "\n";
        error("LINE NUMBER ERROR");
    }
    Statement *st = statetype_judge(line);
    st->execute(program, state, *this);
}

void RUN::execute(Program &program, EvalState &state) {
    flag_run = true;
    linenum = program.getFirstLineNumber();
    if (linenum == -1) return;
    if (!flag_run) return;
    runline(program.getSourceLine(linenum), program, state);
    while (program.getNextLineNumber(linenum) != -1) {
        linenum = program.getNextLineNumber(linenum);
        if (!flag_run) return;
        runline(program.getSourceLine(linenum), program, state);
    }
}

void REM::execute(Program &program, EvalState &state, RUN &run) {
    return;
}

void END::execute(Program &program, EvalState &state, RUN &run) {
    run.flag_run = false;
}

void GOTO::execute(Program &program, EvalState &state, RUN &run) {
    int goto_num = 0;
    for (int goto_wz = 5; goto_wz < line.length(); ++goto_wz)
        goto_num = goto_num * 10 + int(line[goto_wz] - '0');
    run.linenum = goto_num;
    run.runline(program.getSourceLine(goto_num), program, state);
}

void IF::execute(Program &program, EvalState &state, RUN &run) {
    int if_wz1 = 0, if_wz2, value1, value2;
    std::string l, r;
    while (line[if_wz1] != '=' && line[if_wz1] != '<' && line[if_wz1] != '>') {
        if_wz1++;
    }
    if_wz2 = if_wz1 + 1;
    while (line[if_wz2] != 'T') { if_wz2++; }
    l = line.substr(3, if_wz1 - 4);
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(l);
    Expression *exp = parseExp(scanner);
    value1 = exp->eval(state);
    delete exp;
    r = line.substr(if_wz1 + 2, if_wz2 - 3 - if_wz1);
    TokenScanner scannerr;
    scannerr.ignoreWhitespace();
    scannerr.scanNumbers();
    scannerr.setInput(r);
    Expression *expp = parseExp(scannerr);
    value2 = expp->eval(state);
    delete expp;
    if (line[if_wz1] == '=') {
        if (value1 == value2) {
            int goto_num = 0;
            for (int goto_wz = if_wz2 + 5; goto_wz < line.length(); ++goto_wz)
                goto_num = goto_num * 10 + int(line[goto_wz] - '0');
            run.linenum = goto_num;
            run.runline(program.getSourceLine(goto_num), program, state);
            return;
        }
    }
    if (line[if_wz1] == '<') {
        if (value1 < value2) {
            int goto_num = 0;
            for (int goto_wz = if_wz2 + 5; goto_wz < line.length(); ++goto_wz)
                goto_num = goto_num * 10 + int(line[goto_wz] - '0');
            run.linenum = goto_num;
            run.runline(program.getSourceLine(goto_num), program, state);
            return;
        }
    }
    if (line[if_wz1] == '>') {
        if (value1 > value2) {
            int goto_num = 0;
            for (int goto_wz = if_wz2 + 5; goto_wz < line.length(); ++goto_wz)
                goto_num = goto_num * 10 + int(line[goto_wz] - '0');
            run.linenum = goto_num;
            run.runline(program.getSourceLine(goto_num), program, state);
            return;
        }
    }
}

void LIST::execute(Program &program, EvalState &state, RUN &run) {
    int linenum = program.getFirstLineNumber();
    if (linenum == -1) return;
    while (program.getNextLineNumber(linenum) != -1) {
        cout << linenum << " " << program.getSourceLine(linenum) << "\n";
        linenum = program.getNextLineNumber(linenum);
    }
    cout << linenum << " " << program.getSourceLine(linenum) << "\n";
}

void CLEAR::execute(Program &program, EvalState &state, RUN &run) {
    program.clear();
    state.clear();
}

void variable_undefined(std::string line, int beginn, EvalState &state) {
    int line_wz = beginn, equal_wz = beginn, line_wz_pre;
    while (line[equal_wz] != '=' && equal_wz < line.length()) { equal_wz++; }
    if (equal_wz == line.length()) {
        while (line_wz < line.length()) {
            line_wz_pre = line_wz;
            while ((line[line_wz] >= 'A' && line[line_wz] <= 'Z') ||
                   (line[line_wz] >= 'a' && line[line_wz] <= 'z')) { line_wz++; }
            std::string print_variable = line.substr(line_wz_pre, line_wz - line_wz_pre);
            if ((!state.isDefined(print_variable)) && print_variable != "") {
                cout << "VARIABLE NOT DEFINED" << "\n";
                error("VARIABLE NOT DEFINED");
                return;
            }
            while ((line[line_wz] < 'A' || line[line_wz] > 'Z') &&
                   (line[line_wz] < 'a' || line[line_wz] > 'z') && line_wz < line.length()) { line_wz++; }
        }
    } else {
        line_wz = equal_wz + 1;
        while (line_wz < line.length()) {
            line_wz_pre = line_wz;
            while ((line[line_wz] >= 'A' && line[line_wz] <= 'Z') ||
                   (line[line_wz] >= 'a' && line[line_wz] <= 'z')) { line_wz++; }
            std::string print_variable = line.substr(line_wz_pre, line_wz - line_wz_pre);
            if ((!state.isDefined(print_variable)) && print_variable != "") {
                cout << "VARIABLE NOT DEFINED" << "\n";
                error("VARIABLE NOT DEFINED");
                return;
            }
            while ((line[line_wz] < 'A' || line[line_wz] > 'Z') &&
                   (line[line_wz] < 'a' || line[line_wz] > 'z') && line_wz < line.length()) { line_wz++; }
        }
    }
}

void LET::execute(Program &program, EvalState &state, RUN &run) {
    int line_wz = 4;
    while (line[line_wz] != ' ' && line[line_wz] != '=') { line_wz++; }
    std::string let_variable = line.substr(4, line_wz - 4);
    if (name_judge(let_variable)) {
        cout << "SYNTAX ERROR" << "\n";
        error("SYNTAX ERROR");
        return;
    }
    variable_undefined(line, 4, state);
    std::string token = line.substr(4);
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(token);
    Expression *exp = parseExp(scanner);
    int value = exp->eval(state);
    delete exp;
}


void PRINT::execute(Program &program, EvalState &state, RUN &run) {
    variable_undefined(line, 6, state);
    std::string token = line.substr(6);
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(token);
    Expression *exp = parseExp(scanner);
    int value = exp->eval(state);
    cout << value << endl;
    delete exp;
}

void INPUT::execute(Program &program, EvalState &state, RUN &run) {
    cout << " " << "?" << " ";
    std::string input;
    input_variable = line.substr(6);
    input = getLine();
    if (number_judge(input)) {
        int value = 0;
        for (int input_wz = 0; input_wz < input.length(); ++input_wz) {
            if (input[input_wz] != '-')
                value = value * 10 + int(input[input_wz] - '0');
        }
        if (input[0] == '-') value = 0 - value;
        state.setValue(input_variable, value);
    } else {
        cout << "INVALID NUMBER" << "\n";
        cout << " " << "?" << " ";
        input_dg(state);
    }
}

void INPUT::input_dg(EvalState &state) {
    std::string inp;
    inp = getLine();
    if (number_judge(inp)) {
        int value = 0;
        for (int inp_wz = 0; inp_wz < inp.length(); ++inp_wz) {
            if (inp[inp_wz] != '-')
                value = value * 10 + int(inp[inp_wz] - '0');
        }
        if (inp[0] == '-') value = 0 - value;
        state.setValue(input_variable, value);
        state.setValue(input_variable, value);
    } else {
        cout << "INVALID NUMBER" << "\n";
        cout << " " << "?" << " ";
        input_dg(state);
    }
}

bool INPUT::number_judge(std::string number) {
    for (int num_wz = 0; num_wz < number.length(); ++num_wz) {
        if ((number[num_wz] < '0' || number[num_wz] > '9') && number[num_wz] != '-')
            return false;
    }
    return true;
}