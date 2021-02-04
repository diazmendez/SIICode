#pragma once

//*** Implementation of the random number generator by Parisi & Rapuano   


class RNG{

  private:

    const float FNORM =  2.3283064365e-10;

    unsigned myrand, ira[256];
    unsigned char ip, ip1, ip2, ip3;

    int myrdtsc();

    unsigned rand4init();

  public:

    unsigned myseed;

    RNG();
    RNG(unsigned seed);
    float gauss_ran(void);
    double random();
    double frandom();
    double pm1();

};


//** Unit test

//RNG rng(0);
//std::cout << "random: " << rng.random() 
//<< "\t frandom: " << rng.frandom() 
//<< "\t pm1: " << rng.pm1() 
//<< "\t gauss_ran: " << rng.gauss_ran() 
//<< "\t seed: " << rng.myseed << std::endl;


