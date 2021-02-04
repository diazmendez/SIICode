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



    dataThermal thermal(ISystem &system, double T,
                        int conf, // keep initial with -7  
                        int initial_steps, int steps,
                        int measures, std::string algorithm, 
                        int samples=1){

        dataThermal outcome;




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

        // keep featured initial configuration 
        ISystem sys0(system.l);
        if (conf == -7) {
            sys0.copy_conf(system);
        }

        int msteps = (int) steps/measures;

        for (int s=0; s<samples; s++){                


            if (conf!=-7) {
                system.set_conf(conf);
            } else {
                system.copy_conf(sys0); // senseless for s=0
            }


            // perform initial steps
            if (algorithm=="metro"){
                system.metro_flips(system.l2*initial_steps);  
            } else{

            }

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
    
    std::cout << "  -----------------------------  " << std::endl;

    protocols::dataThermal  mydatat;

    mydatat = protocols::thermal(thesystem, 1.8, // temperature
                                    0, // configuration (featured -7)
                                    5, // init_steps
                                    100, // steps
                                    10, // measures
                                    "metro", // algorithm
                                    100); // samples


    mydatat.print2file("datat.txt");

    
    std::cout << "en -> " << mydatat.energy[9] << std::endl;




    return 0;
}

