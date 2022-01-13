#ifndef __MACHINE_HPP__
#define __MACHINE_HPP__

#include "Transition.hpp"
#include "Tape.hpp"
#include "State.hpp"

#include <list>
#include <map>

using std::cout;
using std::endl;
using std::list;
using std::map;
using std::pair;
using std::string;

class Machine
{
private:
    enum Enum_state
    {
        Failed,
        Finished,
        Running,
        Stopped,
    };
    map<string, State *> states_RUN;
    map<string, State *> states_CPY;
    map<string, State *> states_RET;
    string alphabet = "";
    string tape_alphabet = "";

    State *curr_state;
    State *initial_state;
    State *final_state;

    Tape input;
    Tape history;
    Tape output;

    Enum_state machine_state = Enum_state::Stopped;

public:
    void add_state(string name);
    void add_alphabet(string symbol);
    void add_tape_alphabet(string symbol);
    void add_transition(char curr_state, char symbol, char next_state, char next_symbol, int step);
    void load_input(string input);

    void set_initial(string key);
    void set_final(string key);

    void print_machine(Transition *transition);

    void create_copy();
    void create_retrace();
    void step();
    void run();

    Machine();
    ~Machine();
};

#endif