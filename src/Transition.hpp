#ifndef __TRANSICTION_HPP__
#define __TRANSICTION_HPP__

class State;

#include "Tape.hpp"
#include <list>
#include <string>

using std::list;
using std::pair;
using std::stoi;
using std::string;
using std::to_string;

typedef std::string Quadruple_t;

class Transition
{
private:
    static char index;

    string curr_state, next_state;
    State *next_state_obj = nullptr;

    Quadruple_t fst_quad[6], snd_quad[6];

    bool single_quad = false;

public:
    pair<Quadruple_t *, Quadruple_t *> get_quad();
    pair<Quadruple_t *, Quadruple_t *> get_reverse_quad();
    pair<string, string> get_states();

    bool get_single();

    State *get_next_state();
    void set_next_state(State *state);

    Transition(char curr_state, char symbol, char next_state, char next_symbol, int step);
    Transition(string curr_state, string next_state, list<string> quad);
    Transition(string curr_state, string next_state, Quadruple_t *fst_quad, Quadruple_t *snd_quad);

    ~Transition();
};

#endif