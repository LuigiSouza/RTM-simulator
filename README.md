# RTM-Simulator

Reversible Turing Machine simulator based on Bennett's Reversibility, made in C++.

To compile:

`g++ -o main src/*.cpp`

The program wait the entrance by std::cin, so you can type each input by hand in console.

The first line has numbers, wich indicate: number of states, number of symbols in the input alphabet, number of symbols in the tape alphabet and number of transitions, respectively.

Next, we have the stattes, in the next line input alphabet and then tape alphabet. In the following lines it have the transition function (as explained [here](https://mathweb.ucsd.edu/~sbuss/CourseWeb/Math268_2013W/Bennett_Reversibiity.pdf)). After this one, the machine input it follows. Remebering that the acceptance state is the last one, in the case of the example, 6. The 'B' symbols is interpreted as Blank Symbol.

Example of transition function (quintuple):

`(1,0)=>(2,1,R)`

In the example above, the tape which is in the '1' state expects the '0' symbol. If there's a match, it will then write the '1' symbol in the tape, change to '2' state then move the reader to the Right (R).

An alternative to writing one input at a time is to use a file from the standard input, as shown in the following example in [entrada-quintupla.txt](entrada-quintupla.txt):

`./main < entrada-quintupla.txt`
