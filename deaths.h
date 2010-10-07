/*
 * =====================================================================================
 *
 *       Filename:  deaths.h
 *    Description:  implements deaths in population
 *         Author:  David Kariuki
 *
 * =====================================================================================
 */


#ifndef __deaths_h__
#define __deaths_h__

#include "person.h"
#include <math.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  calc_inf_death_risk
 *  Description:  calculates an infected's risk of death based on cd4 count (Badri et. al)
 * =====================================================================================
 */
int calc_infected_death_risk(person &p, StochasticLib1 &s)
{
    float death_rate, death_probability;

    switch(p.clinical_stage)
    {
        case 1: case 2:
            {
                death_rate = exp(-1.72 - 0.08 * sqrt(p.cd4_count));
                break;
            }
        case 3:
            {
                death_rate = exp(-0.895 - 0.079 * sqrt(p.cd4_count));
                break;
            }
        case 4:
            {
                death_rate = exp(-0.26 - 0.065 * sqrt(p.cd4_count));
                break;
            }
        default:
            {
                cout << "invalid cd4 stage.\n";
                exit(-3);
            }

    }

    death_probability = (1 - exp((-1) * death_rate * .25)); // .25 for quarterly probability
    return s.Binomial(1, death_probability);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  calc_susceptible_death_risk
 *  Description:  calculates a susceptible's risk of death 
 * =====================================================================================
 */
int calc_susceptible_death_risk(person &p, StochasticLib1 &s)
{
        return s.Binomial(1, 0.0009);
}




/* 
 * ===  FUNCTION  ======================================================================
 *         Name: person_removed 
 *  Description:  
 * =====================================================================================
 */
bool person_removed(person &p, StochasticLib1 &s)
{
    if(p.status == "susceptible")
       return calc_susceptible_death_risk(p, s);
    else
       return calc_infected_death_risk(p, s); 
}




#endif

