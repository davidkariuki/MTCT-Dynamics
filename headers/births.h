#ifndef __births_h__
#define __births_h__

/*
 * =====================================================================================
 *
 *       Filename:  births.h
 *
 *    Description:  birth rate calculations here
 *
 *        Version:  1.0
 *        Created:  10/19/2010 17:52:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "utils.h"
#define birth_rate 0.0424 // malawi dhs 2004 crude yearly birth rate per woman 
#define BIRTH_PROBABILITY(birth_rate) (1 - exp((-1) * birth_rate * time_step)) // quarterly probability (fleurence & hollenbeak 2007)

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  evaluate_birth_probability
 *  Description:  calculates the probability of birth, returns true or false
 *                NOTE: infected pregnancy rate reduced by 17% (Desgrees du Lou et al. 1999). 
 * =====================================================================================
 */
bool evaluate_birth_probability(person &p, list<birth_data> &data, StochasticLib1 &s)
{
    list<birth_data>::iterator itr; 
    for(itr = data.begin(); itr != data.end(); ++itr)
    {
        if(verify_sex_age(p, 'F', itr->lower, itr->higher))
        {
            if(p.status == "susceptible")
            {
                if(s.Binomial(1, BIRTH_PROBABILITY(itr->fraction)))
                    return true;
                else
                    continue;
            }
            else // person is infected
            {
                if(s.Binomial(1, 0.83 * BIRTH_PROBABILITY(itr->fraction))) // see note 
                    return true;
                else
                    continue;
            }
        }
    }                

    return false;
}


   
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  update_births
 *  Description:  adds children based on birth_probability defined in utils.h
 *                TODO: implement infecteds MTC
 * =====================================================================================
 */
void update_births(list<person> &susceptible, list<person> &infected, list<birth_data> &data, StochasticLib1& s)
{
    list<person>::iterator itr;

    for(itr = susceptible.begin(); itr != susceptible.end(); ++itr)
{
        if(evaluate_birth_probability(*itr, data, s)) 
        {
           person p;
           set_sex(p, s);
           p.age = 0;
           susceptible.push_front(p); // TODO: ID? Other things to set?
           
        }
    }
    
    for(itr = infected.begin(); itr != infected.end(); ++itr)
    {
        if(evaluate_birth_probability(*itr, data, s)) 
        {
           person p;
           set_sex(p, s);
           p.age = 0;

           if(s.Binomial(1, 0.15)) // TODO: source this - Malawi MTCT Rates
               infected.push_back(p); 
           else
               susceptible.push_front(p);
        }
    }
}




#endif
