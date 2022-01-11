#ifndef __TRANSICTION_HPP__
#define __TRANSICTION_HPP__

#include <list>

typedef std::string Quadruple_t;

class Transiction
{
private:
    std::string curr_state, next_state;

    Quadruple_t fst_quad[6], snd_quad[6];
    Quadruple_t rev_fst_quad[6], rev_snd_quad[6];

    bool revert = false;

public:
    std::pair<Quadruple_t *, Quadruple_t *> get_quad();
    std::pair<std::string, std::string> get_states();

    Transiction(char curr_state, char symbol, char next_state, char next_symbol, int step);
    ~Transiction();
};

std::pair<Quadruple_t *, Quadruple_t *> Transiction::get_quad()
{
    return std::pair<Quadruple_t *, Quadruple_t *>(fst_quad, snd_quad);
}
std::pair<std::string, std::string> Transiction::get_states()
{
    return std::pair<std::string, std::string>(curr_state, next_state);
}

static void set_values(Quadruple_t *array, std::list<std::string> values, int size)
{
    auto it = values.begin();
    for (int i = 0; i < size; i++)
    {
        array[i] = *it;
        it++;
    }
}

Transiction::Transiction(char curr_state, char symbol, char next_state, char next_symbol, int step)
{
    std::string str_state(1, curr_state);
    std::string str_nxtstate(1, next_state);
    std::string str_symbol(1, symbol);
    std::string str_nxtsymbol(1, next_symbol);
    this->curr_state = str_state;
    this->next_state = str_nxtstate;
    set_values(fst_quad, {str_symbol, "\\", " ", str_nxtsymbol, "1", " "}, 6);
    set_values(snd_quad, {"\\", " ", "\\", std::to_string(step), str_symbol, std::to_string(step)}, 6);
    int rev_step = -1 * step;
    set_values(rev_fst_quad, {"\\", str_symbol, "\\", std::to_string(rev_step), " ", std::to_string(rev_step)}, 6);
    set_values(rev_snd_quad, {str_nxtsymbol, "\\", "\\", str_symbol, "-1", "0"}, 6);
}

Transiction::~Transiction() {}

#endif