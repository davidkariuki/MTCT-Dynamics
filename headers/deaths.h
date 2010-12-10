/*
 * =====================================================================================
 *
 *       Filename:  deaths.h
 *    Description:  implements deaths in population
 *         Author:  David Kariuki
 *
 *
 * =====================================================================================
 */


#ifndef __deaths_h__
#define __deaths_h__


#define DEATH_CD4 50
#include "utils.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  calc_inf_death_risk
 *  Description:  calculates an infected's risk of death based on cd4 count (Badri et. al)
 * =====================================================================================
 */
int calc_infected_death_risk(person &p, StochasticLib1 &s)
{
    return (p.cd4_count <= DEATH_CD4);
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  calc_susceptible_death_risk
 *  Description:  calculates a susceptible's risk of death 
 *         TODO:  cause of death? for reporting purposes.
 * =====================================================================================
 */
bool calc_susceptible_death_risk(person &p, list<death_data> &data, StochasticLib1 &s)
{
    float death_rate = 0.015;
    float death_prob = 1 - exp((-1) * death_rate * 1.0); // yearly probability

    if(s.Binomial(1, death_prob))
            return true;
    else
        return false;

    list<death_data>::iterator itr; 
    for(itr = data.begin(); itr != data.end(); ++itr)
    {
        if(verify_sex_age(p, 'M', itr->lower, itr->higher))
        {
            if(s.Binomial(1, itr->nqx_m))
                    return true;
        }
        else if(verify_sex_age(p, 'F', itr->lower, itr->higher))
        {
            if(s.Binomial(1, itr->nqx_f))
                return true;
        }
    }                
    return false;
}




/* 
 * ===  FUNCTION  ======================================================================
 *         Name: person_removed 
 *  Description:  
 * =====================================================================================
 */
bool death_risk(person &p, StochasticLib1 &s, list<death_data> &data)
{
    if(p.status == "susceptible")
       return calc_susceptible_death_risk(p, data, s);
   else
       return calc_infected_death_risk(p, s); 
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  compute_deaths
 *  Description:  removes from susceptible and infected popoulations based on mortality
 *                data TODO: finish this
 * =====================================================================================
 */
void compute_deaths(list<person> &S, list<person> &I, list<person> &R, list<death_data> &data, StochasticLib1 &s)
{
    list<person>::iterator itr;
    // run through the susceptible population 
    for(itr = S.begin(); itr != S.end(); )
    {
        if(death_risk(*itr, s, data) == true)
        {
            R.push_back(*itr);
            itr = S.erase(itr); 
        }
        else
        {
            ++itr;
        }
    }

    // infected population
    for(itr = I.begin(); itr != I.end(); )
    {
        if(death_risk(*itr, s, data) == true)
        {
            R.push_back(*itr);
            itr = I.erase(itr); 
        }
        else
        {
            ++itr;
        }
    }
}



#endif

