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




float birth_probability(float birth_rate)
{
    return (1 - exp((-1) * birth_rate * 0.25)); // quarterly probability (fleurence & hollenbeak 2007)
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  evaluate_birth_probability
 *  Description:  calculates the probability of birth, returns true or false
 *                NOTE: infected pregnancy rate reduced by 17% (Desgrees du Lou et al. 1999). 
 * =====================================================================================
 */
bool evaluate_birth_probability(person &p, float birth_rate, StochasticLib1 &s)
{
        if(p.status == "susceptible")
        {
            if(s.Binomial(1, birth_probability(birth_rate)))
                return true;
        }
        else // person is infected
        {
            if(s.Binomial(1, 0.83 * birth_probability(birth_rate))) // see note 
                return true;
        }

    return false;
}


   
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  update_births
 *  Description:  adds children based on birth_probability defined in  
 *                TODO: implement infecteds MTC
 * =====================================================================================
 */
void update_births(list<person> &susceptible, list<person> &infected, float birth_rate, StochasticLib1& s)
{
    list<person>::iterator itr;

    for(itr = susceptible.begin(); itr != susceptible.end(); ++itr)
{
        if(evaluate_birth_probability(*itr, birth_rate, s)) 
        {
           person p;
           set_sex(p, s);
           if(s.Binomial(1,0.4)) p.will_get_treatment = true; // set treatment prefs
           p.age = 0;
           susceptible.push_front(p); // TODO: ID? Other things to set?
           
        }
    }
    
    for(itr = infected.begin(); itr != infected.end(); ++itr)
    {
        if(evaluate_birth_probability(*itr, birth_rate, s)) 
        {
           person p;
           set_sex(p, s);
           if(s.Binomial(1,0.4))  p.will_get_treatment = true; // set treatment prefs
           p.age = 0;

           if(s.Binomial(1, 0.15)) // TODO: source this - Malawi MTCT Rates
               infected.push_back(p); 
           else
               susceptible.push_front(p);
        }
    }
}




#endif
