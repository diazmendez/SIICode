
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "rngparisi.hpp"

//** Random Number Generator by Parisi & Rapuano 



RNG::RNG(){

  myrand = myrdtsc();
  
  myseed = myrand;

  ip = 128;
  ip1 = ip - 24;
  ip2 = ip - 55;
  ip3 = ip - 61;

  for (int i=ip3; i<ip; i++)
        ira[i] = rand4init();

}


RNG::RNG(unsigned seed){

  if (seed==0){ 
    myrand = myrdtsc();
  }
  else{ 
    myrand = seed;
  }

  myseed = myrand;

  ip = 128;
  ip1 = ip - 24;
  ip2 = ip - 55;
  ip3 = ip - 61;

  for (int i=ip3; i<ip; i++)
        ira[i] = rand4init();

}


float RNG::gauss_ran(){
  static int iset=0;
  static float gset;
  float fac, rsq, v1, v2;

  if (iset == 0) {
    do {
      v1 = 2.0 * frandom() - 1.0;
      v2 = 2.0 * frandom() - 1.0;
      rsq = v1 * v1 + v2 * v2;
    } while (rsq >= 1.0 || rsq == 0.0);
    fac = sqrt(-2.0 * log(rsq) / rsq);
    gset = v1 * fac;
    iset = 1;
    return v2 * fac;
  } else {
    iset = 0;
    return gset;
  }
}


double RNG::random(){
  return  ((ira[ip++] = ira[ip1++] + ira[ip2++]) ^ ira[ip3++]);
}


double RNG::frandom(){
  return (FNORM*random());
}


double RNG::pm1(){
  return ((frandom() > 0.5) ? 1 : -1); 
}


//*** private methods:

unsigned RNG::rand4init(){
  unsigned long long y;

  y = (myrand*16807LL);
  myrand = (y&0x7fffffff) + (y>>31);
  if (myrand&0x80000000)
    myrand = (myrand&0x7fffffff) + 1;
  return myrand;
}




// returns the last five digits of the total pseudo-cycles since the 
// processor was powered on
int RNG::myrdtsc(){
  unsigned long long res;
  unsigned int lo,hi;

  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  res = ((unsigned long long)hi << 32) | lo;

  //return (abs(res)%100000);
  return (res%100000);
}






