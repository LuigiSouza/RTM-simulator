#ifndef __STATE_HPP__
#define __STATE_HPP__

class Transition;

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
    list<Transition *> transitions;

public:
    string get_name();
    void set_type(Enum_type type);
    bool is_final();

    list<Transition *> get_transitions();
    Transition *get_transition(char input, char history, char output);
    void add_transition(Transition *transition);

    State(string name);
    ~State();
};

#endif