#ifndef __viral_dynamics_h__
#define __viral_dynamics_h__

/*
 * =====================================================================================
 *
 *       Filename:  viral_dynamics.h
 *    Description:  contains viral load and transmission dynamics
 *         Author:  David Kariuki
 *
 * =====================================================================================
 */


#include "utils.h"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  initialize_cd4
 *  Description:  initializes cd4 count for newly inserted susceptible person to baseline 
 *                TODO: perhaps use this in initialization (person.h)
 * =====================================================================================
 */
void initialize_cd4(person &p, StochasticLib1 &s)
{
    p.cd4_count = s.Normal(800.0, 150.0);
}





/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  update_cd4 
 *  Description:  adjusts change in cd4 count based on viral load (quarterly adjustment)
 *                source: Mellors et. al. Plasma Viral load and CD4+ Lymphocytes as 
                  Prognostic Markers of HIV-1 infection
 * =====================================================================================
 */
void update_cd4(person &p, CRandomMersenne &r)
{
    if(p.viral_load <= log10(500))
    {
        p.cd4_count -= (r.IRandomX(76, 106) / 10.0); 
    }
    else if(p.viral_load > log10(500) && p.viral_load <= log10(3000))
    {
        p.cd4_count -= (r.IRandomX(98, 126) / 10.0);
    }
    else if(p.viral_load > log10(3000) && p.viral_load <= log10(10000))
    {
        p.cd4_count -= (r.IRandomX(127, 149) / 10.0);
    }
    else if(p.viral_load > log10(10000) && p.viral_load <= log10(30000))
    {
        p.cd4_count -= (r.IRandomX(149, 175) / 10.0);
    }
    else
    {
        p.cd4_count -= (r.IRandomX(176, 207) / 10.0);
    }
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  update_cd4_count
 *  Description:  updates cd4 count for given (infected) population
 * =====================================================================================
 */

void update_cd4_count(list<person> &population, CRandomMersenne &r)
{
    list<person>::iterator itr;
    for(itr = population.begin(); itr != population.end(); ++itr)
        update_cd4(*itr, r);
}

#endif

