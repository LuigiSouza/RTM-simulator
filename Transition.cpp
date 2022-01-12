#include "Transiction.hpp"

pair<Quadruple_t *, Quadruple_t *> Transiction::get_quad()
{
    return pair<Quadruple_t *, Quadruple_t *>(fst_quad, snd_quad);
}
pair<string, string> Transiction::get_states()
{
    return pair<string, string>(curr_state, next_state);
}

// void Transiction::set_next_state(State *state)
// {
//     this->next_state_obj = state;
// }

Transiction::Transiction(char curr_state, char symbol, char next_state, char next_symbol, int step)
{
    string str_state(1, curr_state);
    string str_nxtstate(1, next_state);
    string str_symbol(1, symbol);
    string str_nxtsymbol(1, next_symbol);
    this->curr_state = str_state;
    this->next_state = str_nxtstate;
    set_values(fst_quad, {str_symbol, "\\", "\\", str_nxtsymbol, "1", "0"}, 6);
    set_values(snd_quad, {"\\", " ", "\\", to_string(step), str_state, to_string(step)}, 6);
    int rev_step = -1 * step;
    set_values(rev_fst_quad, {"\\", str_state, "\\", to_string(rev_step), " ", to_string(rev_step)}, 6);
    set_values(rev_snd_quad, {str_nxtsymbol, "\\", "\\", str_symbol, "-1", "0"}, 6);
}

Transiction::~Transiction() {}