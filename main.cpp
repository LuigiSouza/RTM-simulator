#include <iostream>
#include <string>
#include <sstream>

#include "Machine.hpp"

int num_states, num_alphabet, num_alph_tape, num_transictions;

void load_info(Machine *machine)
{
    std::string line;
    std::getline(std::cin, line);
    std::stringstream info(line);

    info >> num_states >> num_alphabet >> num_alph_tape >> num_transictions;

    std::getline(std::cin, line);
    std::stringstream states(line);
    std::string initial = "";
    std::string final = "";
    for (int i = 0; i < num_states; i++)
    {
        std::string name;
        states >> name;
        std::string state(1, name[0]);
        if (initial.empty())
            initial = state;
        final = state;
        machine->add_state(state);
    }
    std::getline(std::cin, line);
    std::stringstream alph(line);
    for (int i = 0; i < num_alphabet; i++)
    {
        std::string letter;
        states >> letter;
        std::string str_letter(1, letter[0]);
        machine->add_alphabet(str_letter);
    }
    std::getline(std::cin, line);
    std::stringstream alph_tape(line);
    for (int i = 0; i < num_alph_tape; i++)
    {
        std::string letter;
        states >> letter;
        std::string str_letter(1, letter[0]);
        machine->add_tape_alphabet(str_letter);
    }
    machine->set_final(final);
    machine->set_initial(initial);
}

void load_transictions(Machine *machine)
{
    for (int i = 0; i < num_transictions; i++)
    {
        std::string asd;
        std::cin >> asd;
        std::stringstream stream(asd);
        std::string fst;
        std::string snd;
        std::getline(stream, fst, '=');
        std::getline(stream, snd);

        char curr_state = fst[1], symbol = fst[3], next_state = snd[1], next_symbol = snd[3];
        int move = snd[5] == 'R' ? 1 : snd[5] == 'L' ? -1
                                                     : 0;
        machine->add_transiction(curr_state, symbol, next_state, next_symbol, move);
    }
}

void load_input(Machine *machine)
{
    std::string input;
    std::cin >> input;
    machine->load_input(input);
}

int main()
{
    Machine *machine = new Machine();

    load_info(machine);
    load_transictions(machine);
    load_input(machine);

    machine->run();

    return 0;
}