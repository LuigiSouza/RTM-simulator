#ifndef __STATE_HPP__
#define __STATE_HPP__

class Transiction;

#include <list>

using std::list;
using std::pair;
using std::string;

enum Enum_type
{
    Final,
    Initial,
    Middle,
};

class State
{
private:
    Enum_type state_type;
    string name;
    list<Transiction *> transictions;

public:
    string get_name() { return this->name; };
    void set_type(Enum_type type) { this->state_type = type; };

    Transiction *get_transiction(char input, char history, char output);
    void add_transiction(Transiction *transiction) { this->transictions.push_back(transiction); };

    State(string name);
    ~State();
};

Transiction *State::get_transiction(char input, char history, char output)
{
    Transiction *match = nullptr;
    for (Transiction *transiction : transictions)
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

State::State(string name)
{
    this->state_type = Enum_type::Middle;
    this->name = name;
}
State::~State()
{
    this->transictions.clear();
}

#endif