#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>

#include "rngparisi.hpp"
#include "ising.hpp"
#include "protocols.hpp"



int main(){

    int l = 64;
    int l2=l*l;

    ISystem thesystem(l);

    thesystem.set_conf(0);
    
    protocols::dataThermal  mydatat;

    mydatat = protocols::thermal(thesystem, 1.8, // temperature
                                    0, // configuration (featured -7)
                                    0, // init_steps
                                    100, // steps
                                    20, // measures
                                    "metro", // algorithm
                                    100); // samples


    mydatat.print2file("scratch/datat");



    return 0;
}

