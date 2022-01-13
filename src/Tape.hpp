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
    static const string blank;

    void shift_r();
    void shift_l();

    char read();
    void write(char sym);

    void print_memory();
    int get_index();

    Tape(/* args */);
    ~Tape();
};

#endif
