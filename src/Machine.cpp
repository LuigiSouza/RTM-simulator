#include "Machine.hpp"

static void stop_program(string message)
{
    cout << "Programa encerrado: " << message << endl;
    exit(-1);
}

void Machine::add_alphabet(string symbol)
{
    if (alphabet.find(symbol) != string::npos)
        stop_program("Caractere duplicado...");
    this->alphabet += symbol;
}
void Machine::add_tape_alphabet(string symbol)
{
    if (tape_alphabet.find(symbol) != string::npos)
        stop_program("Caractere duplicado...");
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

void Machine::print_machine(Transition *transition)
{
    return;
    cout << "-----Tapes-----" << endl;
    cout << "Input  [" << input.get_index() << "]: ";
    input.print_memory();
    cout << "History[" << history.get_index() << "]: ";
    history.print_memory();
    cout << "Output [" << output.get_index() << "]: ";
    output.print_memory();
    if (transition != nullptr)
    {
        pair<string, string> states = transition->get_states();
        pair<Quadruple_t *, Quadruple_t *> quadruple = transition->get_quad();
        Quadruple_t *fst_quad = quadruple.first, *snd_quad = quadruple.second;

        cout << "-----States-----" << endl;
        cout << "Current State: " << states.first << endl;
        cout << states.first;
        cout << " [ '" << fst_quad[0] << "', '" << fst_quad[1] << "', '" << fst_quad[2] << "' ] -> ";
        cout << (transition->get_single() ? states.second + " " : states.first + "`");
        cout << "[ '" << fst_quad[3] << "', '" << fst_quad[4] << "', '" << fst_quad[5] << "' ]" << endl;
        if (!transition->get_single())
        {
            cout << states.first << "`[ '" << snd_quad[0] << "', '" << snd_quad[1] << "', '" << snd_quad[2] << "' ] -> ";
            cout << states.second << " [ '" << snd_quad[3] << "', '" << snd_quad[4] << "', '" << snd_quad[5] << "' ]" << endl;
        }
        cout << "Next State: " << states.second << endl;
    }
}

void Machine::create_retrace()
{
    for (pair<string, State *> state : this->states_RUN)
        this->states_RET.insert(pair<string, State *>(state.first, new State(state.first)));
    cout << (sizeof states_RET) << endl;
    cout << (states_RET.size()) << endl;
    State *new_final = this->states_RET.find(initial_state->get_name())->second;
    new_final->set_type(Enum_type::Final);
    final_state = new_final;
    for (pair<string, State *> state : this->states_RUN)
    {
        for (Transition *transition : state.second->get_transitions())
        {
            if (!this->states_RUN.count(transition->get_next_state()->get_name()))
                continue;
            pair<Quadruple_t *, Quadruple_t *> quadruples = transition->get_reverse_quad();
            State *prev_state = this->states_RET.find(transition->get_next_state()->get_name())->second;
            State *next_state = this->states_RET.find(state.first)->second;
            Transition *new_transition = new Transition(
                prev_state->get_name(),
                next_state->get_name(),
                quadruples.first,
                quadruples.second);
            new_transition->set_next_state(next_state);
            prev_state->add_transition(new_transition);
            delete[] quadruples.first;
            delete[] quadruples.second;
        }
    }
}

void Machine::create_copy()
{
    this->states_CPY.insert(pair<string, State *>("L", new State("L")));
    this->states_CPY.insert(pair<string, State *>("l", new State("l")));
    this->states_CPY.insert(pair<string, State *>("-", new State("-")));
    this->states_CPY.insert(pair<string, State *>("R", new State("R")));
    this->states_CPY.insert(pair<string, State *>("r", new State("r")));
    State *statem = this->states_CPY.find("-")->second;
    State *stateL = this->states_CPY.find("L")->second;
    State *statel = this->states_CPY.find("l")->second;
    State *stateR = this->states_CPY.find("R")->second;
    State *stater = this->states_CPY.find("r")->second;

    // Conects last state to copy states
    Transition *trasiction = new Transition(final_state->get_name(), "l", {Tape::blank, "\\", Tape::blank, Tape::blank, "0", Tape::blank});
    trasiction->set_next_state(statel);
    final_state->add_transition(trasiction);
    // Shifts left to L state
    trasiction = new Transition("l", "L", {"\\", "\\", Tape::blank, "-1", "0", Tape::blank});
    trasiction->set_next_state(stateL);
    statel->add_transition(trasiction);
    // If it has gone too far to the right, keeps shifting left until find the first symbol
    trasiction = new Transition("L", "l", {Tape::blank, "\\", Tape::blank, Tape::blank, "0", Tape::blank});
    trasiction->set_next_state(statel);
    stateL->add_transition(trasiction);
    // Transition to - state
    trasiction = new Transition("L", "-", {"\\", "\\", Tape::blank, "0", "0", Tape::blank});
    trasiction->set_next_state(statem);
    stateL->add_transition(trasiction);
    // Stops shifting when meets the blank symbol
    trasiction = new Transition("-", "r", {Tape::blank, "\\", Tape::blank, Tape::blank, "0", Tape::blank});
    trasiction->set_next_state(stater);
    statem->add_transition(trasiction);
    // Shift left transition
    trasiction = new Transition("-", "-", {"\\", "\\", Tape::blank, "-1", "0", Tape::blank});
    trasiction->set_next_state(statem);
    statem->add_transition(trasiction);
    // Transition to R state
    trasiction = new Transition("r", "R", {"\\", "\\", "\\", "1", "0", "1"});
    trasiction->set_next_state(stateR);
    stater->add_transition(trasiction);
    // Adds a copy transition to each symbol in alphabet tape
    for (char &sym : this->tape_alphabet)
    {
        string str_sym(1, sym);
        if (str_sym == Tape::blank)
        {
            // Attachs last copy state to retrace states
            trasiction = new Transition("R", final_state->get_name(), {str_sym, "\\", "\\", str_sym, "0", "-1"});
            this->states_RET.insert(pair<string, State *>(final_state->get_name(), new State(final_state->get_name())));
            State *new_state = this->states_RET.find(final_state->get_name())->second;
            trasiction->set_next_state(new_state);
            stateR->add_transition(trasiction);
            continue;
        }

        trasiction = new Transition("R", "r", {str_sym, "\\", Tape::blank, str_sym, "0", str_sym});
        trasiction->set_next_state(stater);
        stateR->add_transition(trasiction);
    }

    final_state->set_type(Enum_type::Middle);
}

void Machine::step()
{
    list<Transition *> asdfasdf = curr_state->get_transitions();
    Transition *transition = curr_state->get_transition(input.read(), history.read(), output.read());
    if (transition == nullptr)
    {
        this->machine_state = Enum_state::Failed;
        cout << "Entrada inválida" << endl;
        return;
    }
    print_machine(transition);

    string *quad[2] = {transition->get_quad().first,
                       transition->get_quad().second};
    Tape *tapes[3] = {&input, &history, &output};
    for (int j = 0; j < 2; j += 1 + transition->get_single())
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
    this->curr_state = transition->get_next_state();
    if (this->curr_state->is_final())
        this->machine_state = Enum_state::Finished;

    //cout << endl;
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

void Machine::add_transition(char curr_state, char symbol, char next_state, char next_symbol, int step)
{
    string key(1, curr_state);
    string next_key(1, next_state);
    if (!states_RUN.count(key) || !states_RUN.count(next_key))
        stop_program("Nome do estado inválido...");
    if (tape_alphabet.find(symbol) == string::npos || tape_alphabet.find(next_symbol) == string::npos)
        stop_program("Símbolo inválido...");
    if (step != -1 && step != 1 && step != 0)
        stop_program("Passo de trasição inválido...");

    Transition *transition = new Transition(curr_state, symbol, next_state, next_symbol, step);
    State *next = this->states_RUN.find(next_key)->second;
    transition->set_next_state(next);
    this->states_RUN.find(key)->second->add_transition(transition);
}

void Machine::set_initial(string key)
{
    State *initial = this->states_RUN.find(key)->second;
    this->initial_state = initial;
    this->curr_state = initial;
    initial->set_type(Enum_type::Initial);
}
void Machine::set_final(string key)
{
    State *final = this->states_RUN.find(key)->second;
    this->final_state = final;
    final->set_type(Enum_type::Final);
}

void Machine::add_state(string name)
{
    if (!states_RUN.count(name))
        states_RUN.insert(pair<string, State *>(name, new State(name)));
    else
        stop_program("Nome do estado duplicado...");
}

Machine::Machine()
{
    input = Tape();
    history = Tape();
    output = Tape();
}

Machine::~Machine()
{
    for (pair<string, State *> state : this->states_RUN)
        delete state.second;
    for (pair<string, State *> state : this->states_CPY)
        delete state.second;
    for (pair<string, State *> state : this->states_RET)
        delete state.second;
    this->states_RUN.clear();
    this->states_CPY.clear();
    this->states_RET.clear();
}