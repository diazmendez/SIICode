#pragma once 

#include <vector>
#include <string>

#include "ising.hpp"

namespace protocols {



    struct dataThermal {
        std::vector<double> time, energy, magnet;
        std::vector<double> energy2, energy4, magnet2, magnet4;
        std::vector<std::vector<double>> configuration;

        void print2file(std::string fname);

    };



    dataThermal thermal(ISystem &system, double T,
                        int conf, // keep initial with -7  
                        int initial_steps, int steps,
                        int measures, std::string algorithm, 
                        int samples=1);



}