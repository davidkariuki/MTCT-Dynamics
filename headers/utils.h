/*
 * =====================================================================================
 *
 *       Filename:  utils.h
 *    Description:  contains utility functions
 *        Created:  12/28/09 15:33:15
 *         Author:  David Kariuki
 *
 * =====================================================================================
 */

#ifndef __utils_h__
#define __utils_h__


#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <iterator>
#include <math.h>
#include <vector>
#include <list>
#include <string>
#include <iomanip>
#include "stocc.h"
#include "randomc.h"
#include "person.h"

using namespace std;

/*-----------------------------------------------------------------------------
 *  #defines 
 *-----------------------------------------------------------------------------*/
#define EPSILON 0.0001 // TODO: Figure out if rounding check needed
#define time_step 0.25 // model updates quarterly
#define FEMALE_FRACTION 0.5122270149 // from DHS Malawi 2004
#define MALE_FRACTION (1.0 - FEMALE_FRACTION)
#define NUM_ARVS_IN_REGIMENT 3
#define V_INIT 4.0 + sto.Normal(0, 0.5) // initial viral load
#define CD4_INIT 39 - (2 * viral_load) + sto.Normal(0, 2) // initial CD4 count (sqrt)
#define CD4_MAX sto.Normal(800, 150) // CD4 limit on ART
#define TRANS_PROB 0.0011 // transmission probability per act from Gray et. al.
const string ARVS[3] = {"D4T", "3TC", "NVP"}; // TODO: other ARVs used for model?


/*-----------------------------------------------------------------------------
 *  flag parameters
 *-----------------------------------------------------------------------------*/
int POPULATION_SIZE = 50000;
int NUM_ITERATIONS = 40;  // number of iterations for simulation in 3-month intervals


/*-----------------------------------------------------------------------------
 *  stores birth data
 *-----------------------------------------------------------------------------*/
struct birth_data
{
    public:
        explicit birth_data(int l, int h, float f)
        {
            lower = l; higher = h; fraction = f; 
        }

        int lower, higher; // age brackets
        float fraction; // percentages
};


/*-----------------------------------------------------------------------------
 *  stores death data
 *-----------------------------------------------------------------------------*/
struct death_data 
{
    public:
        explicit death_data(int l, int h, float m, float f)
        {
            lower = l; higher = h; nqx_m = m; nqx_f = f; 
        }

        int lower, higher; // age brackets
        float nqx_m; // male death probability
        float nqx_f;
};




/* 
 *
 *
 * ===  FUNCTION  ======================================================================
 *         Name: verify_sex_age 
 *  Description:  checks sex and age within specified limits 
 * =====================================================================================
 */
bool verify_sex_age(person &p, char sex, float lower, float higher)
{
    return (p.sex == sex  && p.age >= lower && p.age < higher + 1);

}		


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  validate_flags
 *  Description:  Checks validity of provided flags. TODO: FLAGS
 * =====================================================================================
 */
void validate_flags(string flags[], const char* values[], int num_flags)
{
	for(int i = 0; i < num_flags; ++i)
	{
        if(flags[i] == "-p" || flags[i] == "-n") 
        {
            continue;
        }
        else
        {
			cout << "invalid flag \"" << flags[i];
			cout << "\" provided. Valid flags are:\n\t-n (#years) \n\t-p (population size) \n";
			exit(-1);
		}
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  initialize_variables
 *  Description:  initializes the values of global constants to specified flag values. 
 * =====================================================================================
 */
void initialize_variables(string flags[], const char* values[], int num_flags)
{
	//initialize variables
	for(int i = 0; i < num_flags; ++i){
		if(flags[i] == "-p"){
            POPULATION_SIZE = atoi(values[i]);
		}
        else if(flags[i] == "-n"){
			NUM_ITERATIONS = 4 * atoi(values[i]);
		}
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  num_from_fraction
 *  Description:  helper for import_population. returns an integer number given
 *                a fraction of the population
 * =====================================================================================
 */
int inline  num_from_fraction(float fraction, char sex)
{
    if(sex == 'F')
        return fraction * FEMALE_FRACTION * POPULATION_SIZE + 0.5;
    else
        return fraction * MALE_FRACTION * POPULATION_SIZE + 0.5;
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  set_age
 *  Description:  sets age using a uniform distribution between age brackets, inclusive.
 * =====================================================================================
 */
void inline set_age(CRandomMersenne &r, person &p, int lower, int higher)
{
    p.age =r.IRandomX(lower, higher);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  set_sex
 *  Description:  sets sex according to female and male proportions.
 * =====================================================================================
 */
void inline set_sex(person &p, StochasticLib1 &s)
{
    p.sex = s.Binomial(1, FEMALE_FRACTION) ? 'F' : 'M';
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  within_age
 *  Description:  function version of the age checker
 * =====================================================================================
 */
 bool within_age(person& elem, int lower, int higher) 
 { 
     return (elem.age >= lower && elem.age <= higher);
 }



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  within_age_range 
 *  Description:  returns true if within age range 
 * =====================================================================================
 */
struct within_age_range 
{
	public: 
        explicit within_age_range(float l, float h)
        {
            lower = l;
            higher = h;
        }
        
        bool operator()(person &elem) 
        { 
            return (elem.age >= lower && elem.age <= higher);
        }

        float lower, higher;
};


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  update_age
 *  Description:  each run of the simulation increases the age by 3 months
 * =====================================================================================
 */
void inline update_age(person &p) 
{
	p.age = p.age + 0.25;
}




/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  check_bounds
 *  Description:  used for float comparison TODO: finish/this may not be necessary
 * =====================================================================================
 */
void inline check_bounds(float number, float lower, float higher)
{
  ; // if(abs(number -lower));;
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  print
 *  Description:  print helper
 * =====================================================================================
 */
void inline print(person &one)
{
    one.print_data(); //TODO: Figure out if copy method is better
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  print_population_stats
 *  Description:  prints out population statistics
 * =====================================================================================
 */
void print_population_stats(list<person> &population, int &year) 
{
	cout << setw(25) << setfill ('-') << "Year " << year / 4 << setw(25) << "\n";
	for_each(population.begin(), population.end(), print);
	cout << endl;
}

#endif
