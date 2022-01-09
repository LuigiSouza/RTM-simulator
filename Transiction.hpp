#ifndef __TRANSICTION_HPP__
#define __TRANSICTION_HPP__

#include <list>

class Transiction
{
private:
    std::string curr_state, next_state;

    std::list<std::string> fst_quad, snd_quad;
    std::list<std::string> rev_fst_quad, rev_snd_quad;

public:
    Transiction(char curr_state, char symbol, char next_state, char next_symbol, int step);
    ~Transiction();
};

Transiction::Transiction(char curr_state, char symbol, char next_state, char next_symbol, int step)
{
    std::string str_state(1, curr_state);
    std::string str_nxtstate(1, next_state);
    std::string str_symbol(1, symbol);
    std::string str_nxtsymbol(1, next_symbol);
    this->curr_state = str_state;
    this->next_state = str_nxtstate;
    this->fst_quad = {str_symbol, "\\", " ", str_nxtsymbol, "1", " "};
    this->snd_quad = {"\\", " ", "\\", std::to_string(step), str_symbol, std::to_string(step)};
    int rev_step = -1 * step;
    this->rev_fst_quad = {"\\", str_symbol, "\\", std::to_string(rev_step), " ", std::to_string(rev_step)};
    this->rev_snd_quad = {str_nxtsymbol, "\\", "\\", str_symbol, "-1", "0"};
}

Transiction::~Transiction() {}

#endif