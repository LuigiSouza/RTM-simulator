#include <iostream>
#include <string>
#include <sstream>

#include "Machine.hpp"

using std::cin;
using std::getline;
using std::string;
using std::stringstream;

int num_states, num_alphabet, num_alph_tape, num_transitions;

void load_info(Machine *machine)
{
    string symbol;

    cin >> num_states >> num_alphabet >> num_alph_tape >> num_transitions;

    string initial = "", final = "";
    for (int i = 0; i < num_states; i++)
    {
        cin >> symbol;
        string state(1, symbol[0]);
        if (initial.empty())
            initial = state;
        final = state;
        machine->add_state(state);
    }
    for (int i = 0; i < num_alphabet; i++)
    {
        cin >> symbol;
        string str_symbol(1, symbol[0]);
        machine->add_alphabet(str_symbol);
    }
    for (int i = 0; i < num_alph_tape; i++)
    {
        cin >> symbol;
        string str_symbol(1, symbol[0]);
        machine->add_tape_alphabet(str_symbol);
    }
    machine->set_final(final);
    machine->set_initial(initial);
}

void load_transitions(Machine *machine)
{
    for (int i = 0; i < num_transitions; i++)
    {
        string line;
        cin >> line;
        stringstream stream(line);
        string fst, snd;
        getline(stream, fst, '=');
        getline(stream, snd);

        char curr_state = fst[1], symbol = fst[3], next_state = snd[1], next_symbol = snd[3];
        int move = snd[5] == 'R' ? 1 : snd[5] == 'L' ? -1
                                                     : 0;
        machine->add_transition(curr_state, symbol, next_state, next_symbol, move);
    }
}

void load_input(Machine *machine)
{
    string input;
    cin >> input;
    machine->load_input(input);
}

int main()
{
    Machine *machine = new Machine();

    load_info(machine);
    load_transitions(machine);
    load_input(machine);

    machine->create_copy();
    machine->create_retrace();
    machine->run();

    delete machine;

    return 0;
}