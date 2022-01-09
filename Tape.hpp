#ifndef __TAPE_HPP__
#define __TAPE_HPP__

class Tape
{
private:
    int index = 0;
    char blank = ' ';
    std::string memory;

public:
    Tape(/* args */);
    ~Tape();
};

Tape::Tape(/* args */)
{
}

Tape::~Tape()
{
}

#endif
