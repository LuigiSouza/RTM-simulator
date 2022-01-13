#ifndef __TAPE_HPP__
#define __TAPE_HPP__

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

class Tape
{
private:
    int index = 0;
    string memory = " ";

public:
    void shift_r();
    void shift_l();
    static const string blank;

    char read();
    void write(char sym);

    void print_memory();
    int get_index() { return index; };

    Tape(/* args */);
    ~Tape();
};

void Tape::print_memory()
{
    string print_memory = "";
    for (int i = 0; i < memory.length(); i++)
    {
        string sym(1, memory[i]);
        print_memory += " " + sym;
    }
    print_memory += " ";
    print_memory[index << 1] = '|';
    print_memory[(index + 1) << 1] = '|';
    cout << print_memory << endl;
}

void Tape::shift_l()
{
    if (index == 0)
        memory = blank + memory;
    else
        index -= 1;
}
void Tape::shift_r()
{
    if (index + 1 == memory.length())
        memory += blank;
    index += 1;
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

#endif
