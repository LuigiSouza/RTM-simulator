#include <iostream>
#include <string>
#include <sstream>

#include "Machine.hpp"

using std::cin;
using std::getline;
using std::string;
using std::stringstream;

int num_states, num_alphabet, num_alph_tape, num_transictions;

void load_info(Machine *machine)
{
    string line;
    getline(cin, line);
    stringstream info(line);

    info >> num_states >> num_alphabet >> num_alph_tape >> num_transictions;

    getline(cin, line);
    stringstream states(line);
    string initial = "";
    string final = "";
    for (int i = 0; i < num_states; i++)
    {
        string name;
        states >> name;
        string state(1, name[0]);
        if (initial.empty())
            initial = state;
        final = state;
        machine->add_state(state);
    }
    getline(cin, line);
    stringstream alph(line);
    for (int i = 0; i < num_alphabet; i++)
    {
        string letter;
        alph >> letter;
        string str_letter(1, letter[0]);
        machine->add_alphabet(str_letter);
    }
    getline(cin, line);
    stringstream alph_tape(line);
    for (int i = 0; i < num_alph_tape; i++)
    {
        string letter;
        alph_tape >> letter;
        string str_letter(1, letter[0]);
        machine->add_tape_alphabet(str_letter);
    }
    machine->set_final(final);
    machine->set_initial(initial);
}

void load_transictions(Machine *machine)
{
    for (int i = 0; i < num_transictions; i++)
    {
        string asd;
        cin >> asd;
        stringstream stream(asd);
        string fst;
        string snd;
        getline(stream, fst, '=');
        getline(stream, snd);

        char curr_state = fst[1], symbol = fst[3], next_state = snd[1], next_symbol = snd[3];
        int move = snd[5] == 'R' ? 1 : snd[5] == 'L' ? -1
                                                     : 0;
        machine->add_transiction(curr_state, symbol, next_state, next_symbol, move);
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
    load_transictions(machine);
    load_input(machine);

    machine->create_copy();
    machine->create_retrace();
    machine->run();

    return 0;
}