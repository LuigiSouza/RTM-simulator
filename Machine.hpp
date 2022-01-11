#ifndef __MACHINE_HPP__
#define __MACHINE_HPP__

#include "Transiction.hpp"
#include "Tape.hpp"

#include <list>
#include <map>

enum Enum_type
{
    Initial,
    Middle,
    Final
};

class Machine
{
private:
    class State
    {
    private:
        Enum_type state_type;
        std::string name;
        std::list<Transiction> transictions;

    public:
        std::string get_name() { return this->name; };
        void set_type(Enum_type type) { this->state_type = type; };

        Transiction *get_transiction(char input, char history, char output);
        void add_transiction(Transiction transiction) { this->transictions.push_back(transiction); };

        State(std::string name);
        ~State();
    };
    std::string curr_state;
    std::map<std::string, Machine::State> states;
    std::string alphabet = "";
    std::string tape_alphabet = "";

    std::string initial_state;
    std::string final_state;

    Tape input;
    Tape history;
    Tape output;

public:
    void add_state(std::string name);
    void add_alphabet(std::string symbol);
    void add_tape_alphabet(std::string symbol);
    void load_input(std::string input);

    void add_transiction(char curr_state, char symbol, char next_state, char next_symbol, int step);

    void set_initial(std::string key);
    void set_final(std::string key);

    void print_machine();

    void step();
    void run();

    Machine();
    ~Machine();
};

void Machine::add_alphabet(std::string symbol)
{
    this->alphabet += symbol;
}
void Machine::add_tape_alphabet(std::string symbol)
{
    this->tape_alphabet += symbol;
}

void Machine::load_input(std::string input)
{
    int count = 0;
    for (char &i : input)
    {
        if (!alphabet.find(i))
            throw std::invalid_argument("Caractere inválido...");
        this->input.write(i);
        this->input.shift_r();
        count++;
    }
    while (count--)
        this->input.shift_l();
}

void Machine::print_machine()
{
    std::cout << "---States---" << std::endl;
    std::cout << "Current State: " << curr_state << std::endl;
    std::cout << "Initial State: " << initial_state << std::endl;
    std::cout << "Final State: " << final_state << std::endl
              << std::endl;
    std::cout << "---Tapes---" << std::endl;
    std::cout << "Input: ";
    input.print_memory();
    std::cout << "History: ";
    history.print_memory();
    std::cout << "Output: ";
    output.print_memory();
}

void Machine::step()
{
    std::cout << "Antes: " << std::endl;
    print_machine();
    State state = this->states.find(this->curr_state)->second;
    Transiction *transiction = state.get_transiction(input.read(), history.read(), output.read());
    std::string *quad[2] = {transiction->get_quad().first,
                            transiction->get_quad().second};
    Tape *tapes[3] = {&input, &history, &output};
    for (int j = 0; j < 2; j++)
        for (int i = 0; i < 3; i++)
        {
            if (quad[j][i][0] == '\\')
            {
                if (quad[j][i + 3] == "-1")
                    tapes[i]->shift_l();
                else if (quad[j][i + 3] == "1")
                    tapes[i]->shift_r();
            }
            else
            {
                tapes[i]->write(quad[j][i + 3][0]);
            }
        }
    this->curr_state = transiction->get_states().second;
    std::cout << "Depois: " << std::endl;
    print_machine();
}
void Machine::run()
{

    int i = 0;
    while (curr_state != final_state)
    {
        step();
        std::cout << i++ << std::endl;
    }
}

void Machine::add_transiction(char curr_state, char symbol, char next_state, char next_symbol, int step)
{
    std::string key(1, curr_state);
    std::string next_key(1, curr_state);
    if (!states.count(key) || !states.count(next_key))
        throw std::invalid_argument("Nome do estado inválido...");
    if (!tape_alphabet.find(symbol) || !tape_alphabet.find(next_symbol))
        throw std::invalid_argument("Símbolo inválido...");
    if (step != -1 && step != 1 && step != 0)
        throw std::invalid_argument("Passo de trasição inválido...");

    Transiction transiction = Transiction(curr_state, symbol, next_state, next_symbol, step);
    this->states.find(key)->second.add_transiction(transiction);
}

void Machine::set_initial(std::string key)
{
    this->initial_state = key;
    this->curr_state = key;
    this->states.find(key)->second.set_type(Enum_type::Initial);
}
void Machine::set_final(std::string key)
{
    this->final_state = key;
    this->states.find(key)->second.set_type(Enum_type::Final);
}

void Machine::add_state(std::string name)
{
    if (!states.count(name))
        states.insert(std::pair<std::string, Machine::State>(name, Machine::State(name)));
    else
        throw std::invalid_argument("Nome do estado duplicado...");
    // std::string asd = states.find(str_name)->second.get_name();
}

Transiction *Machine::State::get_transiction(char input, char history, char output)
{
    Transiction *match = nullptr;
    for (Transiction &transiction : transictions)
    {
        int match_count = 0;
        auto quad_pair = transiction.get_quad().first;
        char check[3] = {input, history, output};
        for (int i = 0; i < 3; i++)
        {
            char it = quad_pair[i][0];
            if (it == check[i] || it == '\\')
                match_count++;
        }
        std::cout << "match: " << match_count << std::endl;
        if (match_count == 3)
        {
            match = &transiction;
            break;
        }
    }
    return match;
}

Machine::State::State(std::string name)
{
    this->state_type = Enum_type::Middle;
    this->name = name;
}
Machine::State::~State()
{
    this->transictions.clear();
}

Machine::Machine()
{
    input = Tape();
    history = Tape();
    output = Tape();
}

Machine::~Machine()
{
    this->states.clear();
}

#endif