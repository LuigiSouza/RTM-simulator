#include "State.hpp"
#include "Transiction.hpp"

Transiction *State::get_transiction(char input, char history, char output)
{
    Transiction *match = nullptr;
    for (Transiction &transiction : transictions)
    {
        int match_count = 0;
        auto quad_pair = transiction->get_quad().first;
        char check[3] = {input, history, output};
        for (int i = 0; i < 3; i++)
        {
            char it = quad_pair[i][0];
            if (it == check[i] || it == '\\')
                match_count++;
        }
        if (match_count == 3)
        {
            match = transiction;
            break;
        }
    }
    return match;
}

void State::add_transiction(Transiction *transiction)
{
    this->transictions.push_back(transiction);
}

State::State(string name)
{
    this->state_type = Enum_type::Middle;
    this->name = name;
}
State::~State()
{
    this->transictions.clear();
}