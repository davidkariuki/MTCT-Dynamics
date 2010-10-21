/*
 * =====================================================================================
 *
 *       Filename:  transmission.h
 *    Description:  contains hard-coded per coital act transmission probabilities 
 *         Author:  David Kariuki
 *
 * =====================================================================================
 */

#ifndef __transmission_h__
#define __transmission_h__

#include "utils.h"

#define INTERCOURSE_FREQUENCY 26.7 // Average Quarterly number of sex acts from Gray et. al.


/*                     (viral load)
    (Age)  <1700    1700–12499 12500–38500 >38 500
    15–24  0·0001   0·0020     0·0019      0·0032
    25–29  0·0001   0·0018     0·0026      0·0048
    30–34  0·00003  0·0005     0·0005      0·0014
    35–59  0·00004  0·0007     0·0008      0·0020
*/



/* 
 * ===  FUNCTION  ======================================================================
 *         Name: infection_probability 
 *  Description: returns the infection probability per coital act based on age and viral load
 * =====================================================================================
 */
float infection_probability(person &p)
{
   if(p.age >= 15 && p.age < 25) // TODO: sync age ranges throughout source
   {
       if                                 (p.viral_load < 3.230449)  { return 0.0001; }
       else if(p.viral_load >= 3.230449 && p.viral_load < 4.096910)  { return 0.0020; }
       else if(p.viral_load >= 4.096910 && p.viral_load <= 4.585461) { return 0.0019; }
       else                                                          { return 0.0032; }
   }
   else if(p.age >= 25 && p.age < 30)
   {
       if                                 (p.viral_load < 3.230449)  { return 0.0001; }
       else if(p.viral_load >= 3.230449 && p.viral_load < 4.096910)  { return 0.0018; }
       else if(p.viral_load >= 4.096910 && p.viral_load <= 4.585461) { return 0.0026; }
       else                                                          { return 0.0048; } 
   }
   else if(p.age >= 30 && p.age < 35)
   {
       if                                 (p.viral_load < 3.230449) { return 0.00003; }
       else if(p.viral_load >= 3.230449 && p.viral_load < 4.096910) { return 0.0005;  }
       else if(p.viral_load >= 4.096910 && p.viral_load <=4.585461) { return 0.0005;  }
       else                                                         { return 0.0014;  } 
   }
   else if(p.age >= 35 && p.age < 50) // changed to 40 to match prevalence data
   {
       if                                 (p.viral_load < 3.230449) { return 0.00004; }
       else if(p.viral_load >= 3.230449 && p.viral_load < 4.096910) { return 0.0007;  }
       else if(p.viral_load >= 4.096910 && p.viral_load <=4.585461) { return 0.0008;  }
       else                                                         { return 0.0020;  } 
   }
   else
   {
       // age over 49!
       cout << "warning, transmission.h edge case: " <<  p.viral_load;
       return 0;
   }

}






/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  will_infect
 *  Description:  returns true if an infected will infect a susceptible 
 * =====================================================================================
 */
bool will_infect(person &p, StochasticLib1 &s)
{
    if(s.Binomial(INTERCOURSE_FREQUENCY, infection_probability(p)))
        return true;
    else
        return false;
}




/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  compute_infected
 *  Description:  goes over the infected population and infects a random susceptible 
 *                based on infection probability by number of coital acts
 * =====================================================================================
 */
void compute_infected(list<person> &susceptible, list<person> &infected, StochasticLib1 &s)
{
    list<person>::iterator itr, sus_itr;

    for(itr = infected.begin(); itr != infected.end(); ++itr)
    {
        if(within_age(*itr, 15, 49) && will_infect(*itr, s))
        {
            sus_itr = find_if(susceptible.begin(), susceptible.end(), within_age_range(15, 49));
            infected.push_back(*sus_itr);
            sus_itr = susceptible.erase(sus_itr); 
        }
    }
}




#endif
