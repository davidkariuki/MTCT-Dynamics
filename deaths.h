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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: person_removed 
 *  Description:  
 * =====================================================================================
 */
bool person_removed(person &p, StochasticLib1 &s)
{
    return s.Binomial(1, 0.009);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  calc_inf_death_risk
 *  Description:  calculates an infected's risk of death based on cd4 count (Badri et. al)
 *                TODO: NOTE: this is six-month risk of AIDS or death. Formulas used to 
 *                calculate death RATE only (by clinical stage / CD4 are below)
                  Death 1 or 2: exp(–1·72–0·08√[CD4–cell count])
                             3: exp(–0·895–0·079√[CD4–cell count])
                             4: exp(–0·26–0·065√[CD4–cell count])
 * 
 * =====================================================================================
 */
int calc_6month_death_risk(person &p, StochasticLib1 &s)
{
         if(p.cd4_count  >= 0 && p.cd4_count < 200)     return s.Binomial(1, 0.186);
    else if(p.cd4_count >= 200 && p.cd4_count < 350)    return s.Binomial(1, 0.067);
    else if(p.cd4_count >= 350)                         return s.Binomial(1, 0.023);
    else
    {
        cout << "Error: invalid CD4 count." << endl;
        exit(-2);

    }
}



#endif

