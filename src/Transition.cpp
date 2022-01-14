#include "Transition.hpp"
#include "State.hpp"

char Transition::index = '0';

bool Transition::get_single()
{
    return this->single_quad;
}

void Transition::set_next_state(State *state) { this->next_state_obj = state; }
State *Transition::get_next_state() { return this->next_state_obj; }

static void set_values(Quadruple_t *array, list<string> values, int size)
{
    auto it = values.begin();
    for (int i = 0; i < size; i++, ++it)
    {
        array[i] = *it;
    }
}

pair<Quadruple_t *, Quadruple_t *> Transition::get_quad()
{
    return pair<Quadruple_t *, Quadruple_t *>(fst_quad, snd_quad);
}
pair<Quadruple_t *, Quadruple_t *> Transition::get_reverse_quad()
{
    Quadruple_t *rev_fst_quad = new Quadruple_t[6];
    Quadruple_t *rev_snd_quad = new Quadruple_t[6];
    Quadruple_t *aux[2] = {fst_quad, snd_quad};
    Quadruple_t *aux_ptr[2] = {rev_snd_quad, rev_fst_quad};
    for (int j = 0; j < 2; j++)
        for (int i = 0; i < 3; i++)
        {
            if (aux[j][i] == "\\")
            {
                int step = stoi(aux[j][i + 3]);
                aux_ptr[j][i] = "\\";
                aux_ptr[j][i + 3] = to_string(-1 * step);
            }
            else
            {
                aux_ptr[j][i] = aux[j][i + 3];
                aux_ptr[j][i + 3] = aux[j][i];
            }
        }
    return pair<Quadruple_t *, Quadruple_t *>(rev_fst_quad, rev_snd_quad);
}
pair<string, string> Transition::get_states()
{
    return pair<string, string>(curr_state, next_state);
}

Transition::Transition(char curr_state, char symbol, char next_state, char next_symbol, int step)
{
    string str_state(1, curr_state), str_nxtstate(1, next_state);
    string str_symbol(1, symbol), str_nxtsymbol(1, next_symbol);
    string str_index(1, Transition::index);
    this->curr_state = str_state;
    this->next_state = str_nxtstate;
    set_values(fst_quad, {str_symbol, "\\", "\\", str_nxtsymbol, "1", "0"}, 6);
    set_values(snd_quad, {"\\", Tape::blank, "\\", to_string(step), str_index, "0"}, 6);
    Transition::index += Transition::index == Tape::blank[0] ? 2 : 1;
}

Transition::Transition(string curr_state, string next_state, list<string> quad)
{
    this->curr_state = curr_state;
    this->next_state = next_state;
    this->single_quad = true;
    set_values(fst_quad, quad, 6);
    set_values(snd_quad, quad, 6);
}
Transition::Transition(string curr_state, string next_state, Quadruple_t *fst_quad, Quadruple_t *snd_quad)
{
    this->curr_state = curr_state;
    this->next_state = next_state;
    for (int i = 0; i < 6; i++)
    {
        this->fst_quad[i] = fst_quad[i];
        this->snd_quad[i] = snd_quad[i];
    }
}

Transition::~Transition() {}