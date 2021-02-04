#include <iostream>
#include "ising.hpp"
#include "rngparisi.hpp"
#include <math.h>

#include <vector>
#include <string>
#include <fstream>


namespace protocols {

    typedef struct dataThermal {
        std::vector<double> time, energy, magnet;
        std::vector<std::vector<double>> configuration;

        void print2file(std::string fname){
            std::ofstream f;
            f.open(fname);
            for (int i=0; i<time.size(); i++){
                
                f << time[i] << "\t" << energy[i] 
                << "\t" << magnet[i] << std::endl;
            }
            f << "\n\n" << std::endl;
            f.close();
        }

        //~dataThermal(){
        //    time.clear();
        //    energy.clear();
        //    magnet.clear();
        //    configuration.clear();
        //}
    } dataThermal;



    dataThermal thermal(ISystem &system, double T, int initial_steps, int steps,
                        int measures, std::string algorithm, int samples=1){

        dataThermal outcome;

        ISystem sys0(system.l);
        sys0.copy_conf(system);

        //sys0->print_sys();
        sys0.print_sys();
        std::cout << "-- || --" << std::endl;
        system.print_sys();



        // resizing vectors and setting to zero
        outcome.time.resize(measures);  
        outcome.energy.resize(measures);  
        outcome.magnet.resize(measures); 
        outcome.configuration.resize(measures);
        for (int i=0; i<measures; i++){
            outcome.configuration[i].resize(system.l2);
        } 

        // set temperature                    
        system.T=T;

        // perform initial steps
        if (algorithm=="metro"){
            system.metro_flips(system.l2*initial_steps);  
        } else{

        }


        int msteps = (int) steps/measures;
        for (int s=0; s<samples; s++){                

            //auto system=sys0;

            //system.print_sys();

            // perform the n measures
            for (int i=0; i<measures; i++){

                (algorithm=="metro") ? system.metro_flips(system.l2*msteps) :
                                       system.metro_flips(system.l2*msteps);

                // gathering data
                outcome.energy[i] += system.nenergy();
                outcome.magnet[i] += system.nmagnet();


                if (s==0){ // time & snapshots only from the first sample
                    outcome.time[i] += ((algorithm=="metro") ? i+1 : i+1);
                    for (int j=0; j<system.l2; j++){
                        outcome.configuration[i][j]=system.site[j].spin;    
                    }
                }

            }



        } 

        // taking averages
        for (int i=0;i<measures;i++){
            outcome.energy[i] = outcome.energy[i]/samples;
            outcome.magnet[i] = outcome.magnet[i]/samples;
        }



        
        std::cout << "*******************************" << std::endl;
        sys0.print_sys();
        std::cout << "-- || --" << std::endl;
        system.print_sys();
        


        return outcome;
    }


}



int main(){

    int l = 20;
    int l2=l*l;

    std::cout << "hello world" << std::endl;

    ISystem thesystem(l);

    thesystem.set_conf(0);
/*
    for (int i = 0; i < l2; i++){
        std::cout << "site: " << i 
        << "\t spi n: " << thesystem.site[i].spin 
        << "\t neigh 0: " << thesystem.site[i].neighbour[0] 
        << "\t field: " << thesystem.site[i].field << std::endl;
    }

    std::cout << "--> nenergy: " << thesystem.nenergy() << std::endl;
    std::cout << "--> nmagnet: " << thesystem.nmagnet() << std::endl;


    thesystem.print_sys();

    thesystem.metro_flips(20);

    thesystem.print_sys();


    std::cout << "nenergy: " << thesystem.nenergy() 
    << " \t temperature: " << thesystem.T <<std::endl;

    thesystem.set_conf(1);
    std::cout << "----> nenergy: " << thesystem.nenergy() << std::endl;

*/    
    std::cout << "  -----------------------------  " << std::endl;

    protocols::dataThermal  mydatat;

    thesystem.set_conf(0);
    mydatat = protocols::thermal(thesystem,1.8,
                                    5, // init_steps
                                    100, // steps
                                    10, // measures
                                    "metro",
                                    100); // samples


    mydatat.print2file("datat.txt");

    
    std::cout << "en -> " << mydatat.energy[9] << std::endl;




    return 0;
}

