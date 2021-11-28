/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "statement.h"

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class RUN {
private:
    std::string line;
    bool flag_run;
    int linenum;

    void runline(std::string line, Program &program, EvalState &state);

public:
    friend class END;

    friend class GOTO;

    friend class IF;

    RUN() {};

    ~RUN() {};

    void execute(Program &program, EvalState &state);

    void flag_turn() { flag_run = false; }
};

class Statement {
private:
    std::string line;
public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Statement() {};

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

    virtual ~Statement() {};

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

    virtual void execute(Program &program, EvalState &state, RUN &run) {};

};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */


class LIST : public Statement {
private:
    std::string line;
public:
    LIST(std::string &s) { line = s; };

    ~LIST() {};

    void execute(Program &program, EvalState &state, RUN &run);
};

class CLEAR : public Statement {
private:
    std::string line;
public:
    CLEAR(std::string &s) { line = s; };

    ~CLEAR() {};

    void execute(Program &program, EvalState &state, RUN &run);
};

class LET : public Statement {
private:
    std::string line;

    bool name_judge(std::string name) {
        return (name == "LET" || name == "RUN");
    }

public:
    LET(std::string &s) { line = s; };

    ~LET() {};

    void execute(Program &program, EvalState &state, RUN &run);
};

class PRINT : public Statement {
private:
    std::string line;
public:
    PRINT(std::string &s) { line = s; };

    ~PRINT() {};

    void execute(Program &program, EvalState &state, RUN &run);
};

class INPUT : public Statement {
private:
    std::string line, input_variable;

    void input_dg(EvalState &state);

    bool number_judge(std::string number);

public:
    INPUT(std::string &s) { line = s; };

    ~INPUT() {};

    void execute(Program &program, EvalState &state, RUN &run);
};

class REM : public Statement {
private:
    std::string line;
public:
    REM(std::string &s) { line = s; };

    ~REM() {};

    void execute(Program &program, EvalState &state, RUN &run);
};

class GOTO : public Statement {
private:
    std::string line;
public:
    GOTO(std::string &s) { line = s; };

    ~GOTO() {};

    void execute(Program &program, EvalState &state, RUN &run);
};

class IF : public Statement {
private:
    std::string line;
public:
    IF(std::string &s) { line = s; };

    ~IF() {};

    void execute(Program &program, EvalState &state, RUN &run);
};

class END : public Statement {
private:
    std::string line;
public:
    END(std::string &s) { line = s; }

    ~END() {};

    void execute(Program &program, EvalState &state, RUN &run);

    void flag_turn(RUN &run) { run.flag_run = false; }
};
#endif
