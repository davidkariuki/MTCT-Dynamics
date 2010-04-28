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
       if                               (p.viral_load < 1700) { return 0.0001; }
       else if(p.viral_load >= 1700 && p.viral_load <= 12499) { return 0.0020; }
       else if(p.viral_load >= 12500 && p.viral_load <=38500) { return 0.0019; }
       else                                                   { return 0.0032; } // > 38500
   }
   else if(p.age >= 25 && p.age < 30)
   {
       if                               (p.viral_load < 1700) { return 0.0001; }
       else if(p.viral_load >= 1700 && p.viral_load <= 12499) { return 0.0018; }
       else if(p.viral_load >= 12500 && p.viral_load <=38500) { return 0.0026; }
       else                                                   { return 0.0048; } 
   }
   else if(p.age >= 30 && p.age < 35)
   {
       if                               (p.viral_load < 1700) { return 0.00003; }
       else if(p.viral_load >= 1700 && p.viral_load <= 12499) { return 0.0005;  }
       else if(p.viral_load >= 12500 && p.viral_load <=38500) { return 0.0005;  }
       else                                                   { return 0.0014;  } 
   }
   else if(p.age >= 35 && p.age < 60)
   {
       if                               (p.viral_load < 1700) { return 0.00004; }
       else if(p.viral_load >= 1700 && p.viral_load <= 12499) { return 0.0007;  }
       else if(p.viral_load >= 12500 && p.viral_load <=38500) { return 0.0008;  }
       else                                                   { return 0.0020;  } 
   }
   else
   {
       // TODO: figure out what this would mean
       return 0;
   }

}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  person_infected 
 *  Description:  returns true if an individual is to be infected based on infection probability
 * =====================================================================================
 */
bool person_infected(person &p, StochasticLib1 &s)
{
    if(s.Binomial(INTERCOURSE_FREQUENCY, infection_probability(p)))
        return true;
    else
        return false;
}




#endif
