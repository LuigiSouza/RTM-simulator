#ifndef __STATE_HPP__
#define __STATE_HPP__

class Transiction;

#include <string>
#include <iostream>
#include <list>

using std::list;
using std::pair;
using std::string;

enum Enum_type
{
    Final,
    Initial,
    Middle,
};

class State
{
private:
    Enum_type state_type;
    string name;
    list<Transiction *> transictions;

public:
    string get_name();
    void set_type(Enum_type type);
    bool is_final();

    list<Transiction *> get_transictions();
    Transiction *get_transiction(char input, char history, char output);
    void add_transiction(Transiction *transiction);

    State(string name);
    ~State();
};

#endif