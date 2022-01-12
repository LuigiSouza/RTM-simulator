#ifndef __TAPE_HPP__
#define __TAPE_HPP__

class Tape
{
private:
    int index = 0;
    std::string memory = " ";

public:
    void shift_r();
    void shift_l();
    const static char blank = 'B';

    char read();
    void write(char sym);

    void print_memory();
    int get_index() { return index; };

    Tape(/* args */);
    ~Tape();
};

void Tape::print_memory()
{
    std::cout << memory << std::endl;
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

Tape::Tape(/* args */)
{
    memory[0] = Tape::blank;
}

Tape::~Tape()
{
}

#endif
