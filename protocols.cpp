#include "protocols.hpp"

#include <fstream>
#include "ising.hpp"





void protocols::dataThermal::print2file(std::string fname){
    std::ofstream f;

    f.open(fname + ".dat");
    for (int i=0; i<time.size(); i++){
            
        f << time[i] << "\t" << energy[i] << "\t" << energy2[i] 
        << "\t" << energy4[i] << "\t" << magnet[i] << "\t"
        << magnet2[i] << "\t" << magnet4[i] << std::endl;
    }
    f << "\n\n" << std::endl;
    f.close();
}




protocols::dataThermal protocols::thermal(ISystem &system, double T,
                    int conf, // keep initial with -7  
                    int initial_steps, int steps,
                    int measures, std::string algorithm, 
                    int samples /* =1 */){

    dataThermal outcome;
    double temp;



    // resizing vectors and setting to zero
    outcome.time.resize(measures);  
    outcome.energy.resize(measures);  
    outcome.energy2.resize(measures);  
    outcome.energy4.resize(measures);  
    outcome.magnet.resize(measures); 
    outcome.magnet2.resize(measures); 
    outcome.magnet4.resize(measures); 
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

        // run n measures
        for (int i=0; i<measures; i++){

            //perform msteps steps
            (algorithm=="metro") ? system.metro_flips(system.l2*msteps) :
                                    system.metro_flips(system.l2*msteps);

            // gathering data
            temp=system.nenergy();
            outcome.energy[i] += temp;
            outcome.energy2[i] += temp*temp;
            outcome.energy4[i] += temp*temp*temp*temp;
            temp=system.nmagnet();
            outcome.magnet[i] += temp;
            outcome.magnet2[i] += temp*temp;
            outcome.magnet4[i] += temp*temp*temp*temp;


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
        outcome.energy2[i] = outcome.energy2[i]/samples;
        outcome.energy4[i] = outcome.energy4[i]/samples;
        outcome.magnet[i] = outcome.magnet[i]/samples;
        outcome.magnet2[i] = outcome.magnet2[i]/samples;
        outcome.magnet4[i] = outcome.magnet4[i]/samples;
    }

    return outcome;
}





