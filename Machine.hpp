#ifndef __MACHINE_HPP__
#define __MACHINE_HPP__

#include "Transiction.hpp"
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

static void stop_program(string message);

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
    map<string, State> states;
    map<string, State> states_A;
    map<string, State> states_B;
    map<string, State> states_C;
    string curr_state;
    string alphabet = "";
    string tape_alphabet = "";

    string initial_state;
    string final_state;

    Tape input;
    Tape history;
    Tape output;

    Enum_state machine_state = Enum_state::Stopped;

public:
    void add_state(string name);
    void add_alphabet(string symbol);
    void add_tape_alphabet(string symbol);
    void load_input(string input);

    void add_transiction(char curr_state, char symbol, char next_state, char next_symbol, int step);

    void set_initial(string key);
    void set_final(string key);

    void print_machine();

    void reverse_step();
    void step();
    void run();

    Machine();
    ~Machine();
};

void Machine::add_alphabet(string symbol)
{
    this->alphabet += symbol;
}
void Machine::add_tape_alphabet(string symbol)
{
    this->tape_alphabet += symbol;
}

void Machine::load_input(string input)
{
    int count = 0;
    for (char &i : input)
    {
        if (!alphabet.find(i))
            stop_program("Caractere inválido...");
        this->input.write(i);
        this->input.shift_r();
        count++;
    }
    while (count--)
        this->input.shift_l();
}

void Machine::print_machine()
{
    cout << "---States---" << endl;
    cout << "Current State: " << curr_state << endl;
    cout << "Initial State: " << initial_state << endl;
    cout << "Final State: " << final_state << endl
         << endl;
    cout << "---Tapes---" << endl;
    cout << "Input: ";
    input.print_memory();
    cout << "History: ";
    history.print_memory();
    cout << "Output: ";
    output.print_memory();
}

void Machine::reverse_step() {}

void Machine::step()
{
    cout << "Antes: " << endl;
    print_machine();
    State state = this->states.find(this->curr_state)->second;
    Transiction *transiction = state.get_transiction(input.read(), history.read(), output.read());

    if (transiction == nullptr)
    {
        this->machine_state = Enum_state::Failed;
        cout << "Entrada inválida" << endl;
        return;
    }

    string *quad[2] = {transiction->get_quad().first,
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
    if (this->curr_state == this->final_state)
        this->machine_state = Enum_state::Finished;
    cout << "Depois: " << endl;
    print_machine();
}
void Machine::run()
{
    this->machine_state = Enum_state::Running;
    int i = 0;
    while (this->machine_state == Enum_state::Running)
        step();

    cout << "Programa finalizado, Resultados:" << std::endl;
    print_machine();
}

void Machine::add_transiction(char curr_state, char symbol, char next_state, char next_symbol, int step)
{
    string key(1, curr_state);
    string next_key(1, curr_state);
    if (!states.count(key) || !states.count(next_key))
        stop_program("Nome do estado inválido...");
    if (!tape_alphabet.find(symbol) || !tape_alphabet.find(next_symbol))
        stop_program("Símbolo inválido...");
    if (step != -1 && step != 1 && step != 0)
        stop_program("Passo de trasição inválido...");

    Transiction *transiction = new Transiction(curr_state, symbol, next_state, next_symbol, step);
    this->states.find(key)->second.add_transiction(transiction);
}

void Machine::set_initial(string key)
{
    this->initial_state = key;
    this->curr_state = key;
    this->states.find(key)->second.set_type(Enum_type::Initial);
}
void Machine::set_final(string key)
{
    this->final_state = key;
    this->states.find(key)->second.set_type(Enum_type::Final);
}

void Machine::add_state(string name)
{
    if (!states.count(name))
        states.insert(pair<string, State>(name, State(name)));
    else
        stop_program("Nome do estado duplicado...");
    // string asd = states.find(str_name)->second.get_name();
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

static void stop_program(string message)
{
    cout << "Programa encerrado: " << message << endl;
    exit(-1);
}

#endif