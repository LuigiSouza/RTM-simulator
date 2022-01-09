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
        std::string get_name();
        void set_type(Enum_type type) { this->state_type = type; };
        void add_transiction(Transiction transiction) { this->transictions.push_back(transiction); };

        State(std::string name);
        ~State();
    };
    std::map<std::string, Machine::State> states;
    std::list<std::string> alphabet;
    std::list<std::string> tape_alphabet;

    std::string initial_state;
    std::string final_state;

    Tape input;
    Tape history;
    Tape output;

public:
    void add_state(std::string name);
    void add_alphabet(std::string symbol);
    void add_tape_alphabet(std::string symbol);

    void add_transiction(char curr_state, char symbol, char next_state, char next_symbol, int step);

    void set_initial(std::string key);
    void set_final(std::string key);

    void step();
    void run();

    Machine();
    ~Machine();
};

void Machine::add_alphabet(std::string symbol)
{
    this->alphabet.push_back(symbol);
}
void Machine::add_tape_alphabet(std::string symbol)
{
    this->tape_alphabet.push_back(symbol);
}

void Machine::step()
{
}
void Machine::run()
{
}

void Machine::add_transiction(char curr_state, char symbol, char next_state, char next_symbol, int step)
{
    Transiction transiction = Transiction(curr_state, symbol, next_state, next_symbol, step);
    std::string key(1, curr_state);
    this->states.find(key)->second.add_transiction(transiction);
}

void Machine::set_initial(std::string key)
{
    this->initial_state = key;
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

std::string Machine::State::get_name()
{
    return this->name;
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
    this->tape_alphabet.clear();
    this->alphabet.clear();
}

#endif