#include "ising.hpp"
#include "rngparisi.hpp"
#include <cstdlib>
#include <vector>
#include <math.h>
#include <iostream>


//** Site

// this constructor does not axecutes in an array definition
ISite::ISite(){ 
    spin = 1;
    for (int i=0; i < 4; i++) 
        neighbour[i]=0;
    field = 0;
}



//** System

ISystem::ISystem(int L){
    l=L;
    l2=l*l;
    T=1e-10;

    site.reserve(l2);
    deltaE.reserve(l2);
 
    set_neighbours();
    set_conf(1); 

}

// sets a spin configuration and calls set_deltaE
void ISystem::set_conf(int c){
    switch (c)
    {
    case 1: // ferro
        for (int i=0; i<l2; i++){ 
            site[i].spin=1;
            site[i].field=0;
        }
        break;

    case 0: // para
        for (int i=0; i<l2; i++){ 
            site[i].spin=rng.pm1();
            site[i].field=0;
        }
        break;
    
    default: // keeps the current configuration
        break;
    }

    set_EdeltaE();
    set_magnet();
}

void ISystem::set_neighbours(){
    int i;
	for (i = 0; i < l2; i++){	// safe double count
		// rigth               
		if (div(i+1, l).rem == 0) site[i].neighbour[0] = i - (l -1);
		else  site[i].neighbour[0] = i + 1;
		// left
		if (div(i, l).rem == 0) site[i].neighbour[1]  = i + (l -1);
		else  site[i].neighbour[1]  = i -1;
		// up
		if (i < l) site[i].neighbour[2]  = i + ((l -1) * l);
		else   site[i].neighbour[2] = i - l;
		// down
		if ((i + l) >= l2) site[i].neighbour[3]  = i -((l - 1) * l);
		else  site[i].neighbour[3]  = i + l;
    	}

}

// updates energy and deltaE from scratch
void ISystem::set_EdeltaE(){
    double dE;

    // reset energies    
    energy = 0;
    for (int i =0; i<l2; i++){
        deltaE[i]=0;
    }

    // loop for the sites
    for (int k = 0; k<l2; k++){

        // Ising contribution 
        dE = ising_deltaE(k);      
        deltaE[k] += dE;
        energy += -0.25 * dE; // energy += 0.5 * site[k].spin * sumj;

        // field contribution
        dE = 2 * site[k].field * site[k].spin;
        deltaE[k] += dE;
        energy += -0.5 * dE;

    }

}


double ISystem::nenergy(){
    return energy/l2;
}


// updates magnet from scratch
void ISystem::set_magnet(){
    magnet  = 0;
    
    //for (auto s : site){ // not working
    //    magnet += s.spin;
    //}

    for (int i=0; i<l2; i++){
        magnet += site[i].spin;
    }

}

double ISystem::nmagnet(){
    return (double) magnet/l2;   
}


void ISystem::metro_flips(int n){ // Metropolis flip attempts
    for (int i=0; i<n; i++){
        int k = (int) floor(rng.frandom()*l2);

        // short-circuit evaluation ||
        if ((deltaE[k]<=0) || (rng.frandom()<exp(-deltaE[k]/T))){
            update(k);
        } 
    }
}

void ISystem::update(int k){
    energy += deltaE[k];
    magnet += -2*site[k].spin;
    site[k].spin = -site[k].spin;

    deltaE[k] = 0;
    for (int i=0; i<4; i++){
        deltaE[site[k].neighbour[i]] = 0;
    }


    // Ising contribution 
    deltaE[k] += ising_deltaE(k); 
    for (int i=0; i<4; i++){
        deltaE[site[k].neighbour[i]] += ising_deltaE(site[k].neighbour[i]);
    }

    // field contribution
    deltaE[k] += -2 * site[k].field * site[k].spin;
    for (int i=0; i<4; i++){
        deltaE[site[k].neighbour[i]] += -2 * site[site[k].neighbour[i]].field * site[site[k].neighbour[i]].spin;
    }
}


double ISystem::ising_deltaE(int k){
    double sumj=0;

    for (int j=0; j<4; j++) {
        sumj += site[site[k].neighbour[j]].spin;
    }

    return(2.0 * site[k].spin * sumj);            
}


void ISystem::print_sys(){
    for (int i = 0; i < l; i++){
        for (int j = 0; j < l; j++){
            std::cout << site[(i*l)+j].field << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for (int i = 0; i < l; i++){
        for (int j = 0; j < l; j++){
            std::cout << site[(i*l)+j].spin << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;


    std::cout << "energy: " << energy << " " << (double)energy/l2 
    << "\t magnet: " << magnet << "  " << (double) magnet/l2  
    << "\n\n" << std::endl; 
}



void ISystem::copy_conf(ISystem &source){

    if (source.l!=l){
        l=source.l;
        l2=l*l;        

        site.reserve(l2);
        deltaE.reserve(l2);
    
        set_neighbours();

    }

    T=source.T;

    for (int i=0; i<l2; i++){
        site[i].spin = source.site[i].spin;
        site[i].field = source.site[i].field;
    }


    set_conf(-7); 


}



