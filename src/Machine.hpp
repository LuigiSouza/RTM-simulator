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
    map<string, State *> states_A;
    map<string, State *> states_B;
    map<string, State *> states_C;
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
    void load_input(string input);

    void add_transiction(char curr_state, char symbol, char next_state, char next_symbol, int step);

    void set_initial(string key);
    void set_final(string key);

    void print_machine(Transiction *transiction);

    void create_copy();
    void create_retrace();
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
        if (alphabet.find(i) == string::npos)
            stop_program("Caractere inválido...");
        this->input.write(i);
        this->input.shift_r();
        count++;
    }
    while (count--)
        this->input.shift_l();
}

void Machine::print_machine(Transiction *transiction)
{
    if (transiction != nullptr)
    {
        pair<string, string> states = transiction->get_states();
        pair<Quadruple_t *, Quadruple_t *> quadruple = transiction->get_quad();
        Quadruple_t *fst_quad = quadruple.first, *snd_quad = quadruple.second;

        cout << "-----States-----" << endl;
        cout << "Current State: " << states.first << endl;
        cout << states.first;
        cout << " [ '" << fst_quad[0] << "', '" << fst_quad[1] << "', '" << fst_quad[2] << "' ] -> ";
        cout << (transiction->get_single() ? states.second + " " : states.first + "`");
        cout << "[ '" << fst_quad[3] << "', '" << fst_quad[4] << "', '" << fst_quad[5] << "' ]" << endl;
        if (!transiction->get_single())
        {
            cout << states.first << "`[ '" << snd_quad[0] << "', '" << snd_quad[1] << "', '" << snd_quad[2] << "' ] -> ";
            cout << states.second << " [ '" << snd_quad[3] << "', '" << snd_quad[4] << "', '" << snd_quad[5] << "' ]" << endl;
        }
        cout << "Next State: " << states.second << endl;
    }
    cout << "-----Tapes-----" << endl;
    cout << "Input  [" << input.get_index() << "]: ";
    input.print_memory();
    cout << "History[" << history.get_index() << "]: ";
    history.print_memory();
    cout << "Output [" << output.get_index() << "]: ";
    output.print_memory();
}

void Machine::create_retrace()
{
    for (pair<string, State *> state : states_A)
        states_C.insert(pair<string, State *>(state.first, new State(state.first)));
    State *new_final = states_C.find(initial_state->get_name())->second;
    new_final->set_type(Enum_type::Final);
    final_state = new_final;
    for (pair<string, State *> state : states_A)
    {
        for (Transiction *transiction : state.second->get_transictions())
        {
            if (!states_A.count(transiction->get_next_state()->get_name()))
                continue;
            pair<Quadruple_t *, Quadruple_t *> quadruples = transiction->get_rev_quad();
            State *prev_state = states_C.find(transiction->get_next_state()->get_name())->second;
            State *next_state = states_C.find(state.first)->second;
            Transiction *new_transiction = new Transiction(
                prev_state->get_name(),
                next_state->get_name(),
                quadruples.first,
                quadruples.second);
            new_transiction->set_next_state(next_state);
            prev_state->add_transiction(new_transiction);
        }
    }
}

void Machine::create_copy()
{
    states_B.insert(pair<string, State *>("L", new State("L")));
    states_B.insert(pair<string, State *>("l", new State("l")));
    states_B.insert(pair<string, State *>("-", new State("-")));
    states_B.insert(pair<string, State *>("R", new State("R")));
    states_B.insert(pair<string, State *>("r", new State("r")));
    states_B.insert(pair<string, State *>("B", new State("B")));
    states_B.insert(pair<string, State *>("F", new State("F")));
    State *statemin = states_B.find("-")->second;
    State *stateL = states_B.find("L")->second;
    State *statel = states_B.find("l")->second;
    State *stateR = states_B.find("R")->second;
    State *stater = states_B.find("r")->second;
    State *stateb = states_B.find("B")->second;
    State *statef = states_B.find("F")->second;

    Transiction *trasiction = new Transiction(final_state->get_name(), "l", {Tape::blank, "\\", Tape::blank, Tape::blank, "0", Tape::blank});
    trasiction->set_next_state(statel);
    final_state->add_transiction(trasiction);
    trasiction = new Transiction("l", "L", {"\\", "\\", Tape::blank, "-1", "0", Tape::blank});
    trasiction->set_next_state(stateL);
    statel->add_transiction(trasiction);
    trasiction = new Transiction("L", "l", {Tape::blank, "\\", Tape::blank, Tape::blank, "0", Tape::blank});
    trasiction->set_next_state(statel);
    stateL->add_transiction(trasiction);
    trasiction = new Transiction("L", "-", {"\\", "\\", Tape::blank, "0", "0", Tape::blank});
    trasiction->set_next_state(statemin);
    stateL->add_transiction(trasiction);
    trasiction = new Transiction("-", "r", {Tape::blank, "\\", Tape::blank, Tape::blank, "0", Tape::blank});
    trasiction->set_next_state(stater);
    statemin->add_transiction(trasiction);
    trasiction = new Transiction("-", "-", {"\\", "\\", Tape::blank, "-1", "0", Tape::blank});
    trasiction->set_next_state(statemin);
    statemin->add_transiction(trasiction);
    trasiction = new Transiction("r", "R", {"\\", "\\", "\\", "1", "0", "1"});
    trasiction->set_next_state(stateR);
    stater->add_transiction(trasiction);
    for (char &sym : this->tape_alphabet)
    {
        string str_sym(1, sym);
        if (str_sym == Tape::blank)
        {
            trasiction = new Transiction("R", final_state->get_name(), {str_sym, "\\", "\\", str_sym, "0", "-1"});
            states_C.insert(pair<string, State *>(final_state->get_name(), new State(final_state->get_name())));
            State *new_state = states_C.find(final_state->get_name())->second;
            trasiction->set_next_state(new_state);
            stateR->add_transiction(trasiction);
            continue;
        }

        trasiction = new Transiction("R", "r", {str_sym, "\\", Tape::blank, str_sym, "0", str_sym});
        trasiction->set_next_state(stater);
        stateR->add_transiction(trasiction);
    }

    final_state->set_type(Enum_type::Middle);
}

void Machine::step()
{
    list<Transiction *> asdfasdf = curr_state->get_transictions();
    Transiction *transiction = curr_state->get_transiction(input.read(), history.read(), output.read());
    if (transiction == nullptr)
    {
        this->machine_state = Enum_state::Failed;
        cout << "Entrada inválida" << endl;
        return;
    }
    print_machine(transiction);

    string *quad[2] = {transiction->get_quad().first,
                       transiction->get_quad().second};
    Tape *tapes[3] = {&input, &history, &output};
    for (int j = 0; j < 2; j += 1 + transiction->get_single())
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
    this->curr_state = transiction->get_next_state();
    if (this->curr_state->is_final())
        this->machine_state = Enum_state::Finished;

    cout << endl;
}
void Machine::run()
{
    this->machine_state = Enum_state::Running;
    int i = 0;
    while (this->machine_state == Enum_state::Running && ++i)
        step();

    cout << "Programa finalizado, passos: " << i << std::endl;
    print_machine(nullptr);
}

void Machine::add_transiction(char curr_state, char symbol, char next_state, char next_symbol, int step)
{
    string key(1, curr_state);
    string next_key(1, next_state);
    if (!states_A.count(key) || !states_A.count(next_key))
        stop_program("Nome do estado inválido...");
    if (tape_alphabet.find(symbol) == string::npos || tape_alphabet.find(next_symbol) == string::npos)
        stop_program("Símbolo inválido...");
    if (step != -1 && step != 1 && step != 0)
        stop_program("Passo de trasição inválido...");

    Transiction *transiction = new Transiction(curr_state, symbol, next_state, next_symbol, step);
    State *next = this->states_A.find(next_key)->second;
    transiction->set_next_state(next);
    this->states_A.find(key)->second->add_transiction(transiction);
}

void Machine::set_initial(string key)
{
    State *initial = this->states_A.find(key)->second;
    this->initial_state = initial;
    this->curr_state = initial;
    initial->set_type(Enum_type::Initial);
}
void Machine::set_final(string key)
{
    State *final = this->states_A.find(key)->second;
    this->final_state = final;
    final->set_type(Enum_type::Final);
}

void Machine::add_state(string name)
{
    if (!states_A.count(name))
        states_A.insert(pair<string, State *>(name, new State(name)));
    else
        stop_program("Nome do estado duplicado...");
    // string asd = states_A.find(str_name)->second.get_name();
}

Machine::Machine()
{
    input = Tape();
    history = Tape();
    output = Tape();
}

Machine::~Machine()
{
    this->states_A.clear();
}

static void stop_program(string message)
{
    cout << "Programa encerrado: " << message << endl;
    exit(-1);
}

#endif