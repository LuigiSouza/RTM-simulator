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
        std::list<Transiction> transactions;

    public:
        std::string get_name();
        void set_type(Enum_type type) { this->state_type = type; };

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

    void add_transiction(std::string curr_state, std::string symbol, std::string next_state, std::string next_symbol);

    void set_initial(std::string key);
    void set_final(std::string key);

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

void Machine::add_transiction(std::string curr_state, std::string symbol, std::string next_state, std::string next_symbol)
{
}

void Machine::set_initial(std::string key)
{
    this->initial_state = key;
    states.find(key)->second.set_type(Enum_type::Initial);
}
void Machine::set_final(std::string key)
{
    this->final_state = key;
    states.find(key)->second.set_type(Enum_type::Final);
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
    transactions.clear();
}

Machine::Machine()
{
}

Machine::~Machine()
{
    states.clear();
    tape_alphabet.clear();
    alphabet.clear();
}