#include "Tape.hpp"

void Tape::print_memory()
{
    string print_memory = "";
    for (int i = 0; i < (int)memory.length(); i++)
    {
        string sym(1, memory[i]);
        print_memory += " " + sym;
    }
    print_memory += " ";
    print_memory[index << 1] = '|';
    print_memory[(index + 1) << 1] = '|';
    cout << print_memory << endl;
}

int Tape::get_index() { return index; }

void Tape::shift_l()
{
    if (index == 0)
        memory = blank + memory;
    else
        index -= 1;
}
void Tape::shift_r()
{
    index += 1;
    if (index == (int)memory.length())
        memory += blank;
}

char Tape::read()
{
    return memory[index];
}
void Tape::write(char sym)
{
    memory[index] = sym;
}

const string Tape::blank = "B";

Tape::Tape(/* args */)
{
    memory[0] = Tape::blank[0];
}

Tape::~Tape()
{
}