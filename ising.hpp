#pragma once

#include <vector>
#include "rngparisi.hpp"

class ISite{
    public:
        int spin;
        int neighbour[4];
        double field;

        //methods
        ISite();
};
        

class ISystem{
    private:
        RNG rng;

        void set_neighbours();
        void set_EdeltaE();
        void set_magnet();
        void update(int);
        double ising_deltaE(int);

    public:
        int l, l2;
        double T;
        std::vector<ISite> site;
        std::vector<double> deltaE;
        double energy;
        int magnet;
        

        //methods
        ISystem(int);
        void metro_flips(int);
        void set_conf(int);
        double nenergy();
        double nmagnet();
        void print_sys();
        void copy_conf(ISystem &source);

        void ed_flips();
};

