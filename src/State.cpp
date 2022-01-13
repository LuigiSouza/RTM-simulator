#include "State.hpp"
#include "Transition.hpp"

Transition *State::get_transition(char input, char history, char output)
{
    Transition *match = nullptr;
    for (Transition *transition : transitions)
    {
        int match_count = 0;
        auto quad_pair = transition->get_quad().first;
        char check[3] = {input, history, output};
        for (int i = 0; i < 3; i++)
        {
            char it = quad_pair[i][0];
            if (it == check[i] || it == '\\')
                match_count++;
        }
        if (match_count == 3)
        {
            match = transition;
            break;
        }
    }
    return match;
}

string State::get_name()
{
    return this->name;
}

void State::set_type(Enum_type type)
{
    this->state_type = type;
}
bool State::is_final()
{
    return this->state_type == Enum_type::Final;
}

list<Transition *> State::get_transitions()
{
    return transitions;
}

void State::add_transition(Transition *transition)
{
    this->transitions.push_back(transition);
}

State::State(string name)
{
    this->state_type = Enum_type::Middle;
    this->name = name;
}
State::~State()
{
    for (Transition *transition : transitions)
        delete transition;
    this->transitions.clear();
}