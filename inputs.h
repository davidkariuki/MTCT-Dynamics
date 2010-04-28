/*
 * =====================================================================================
 *
 *       Filename:  inputs.h
 *    Description:  contains files that perform input operations on data
 *        Created:  12/29/09 12:54:21
 *         Author: David Kariuki 
 *
 * =====================================================================================
 */

#ifndef __inputs_h__
#define __inputs_h__

#include <list>
#include <fstream> 
#include <sstream>
#include "person.h"
#include "utils.h"

#define POPULATION_DATA "data/population_data.csv" // contains age groups and fractions of men and women
#define PREVALENCE_DATA "data/prevalence_data.csv" // contains age groups and fractions of men and women
#define BIRTH_DATA "data/birth_data.csv"


/*-----------------------------------------------------------------------------
 *  stores prevalence data : TODO: templetize to use with other data if necessary
 *-----------------------------------------------------------------------------*/
struct prevalence_data
{
    public:
        explicit prevalence_data(int l, int h, float m, float f)
        {
            lower = l; higher = h; male_f = m; female_f = f;
        }

        int lower, higher; // age brackets
        float male_f, female_f; // percentages
};
 

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  import_population_data
 *  Description:  gets population distribution data from population_data.csv
 * =====================================================================================
 */
void import_population_data(list<person> &data, CRandomMersenne &r)
{
    ifstream input(POPULATION_DATA, ios::in);
    if(input.fail()) 
    {
        cout << "Input error: could not open" << POPULATION_DATA << endl;
        exit(-1);
    }
   
    int lower, higher; // age brackets
    float male_fraction, female_fraction; // percentages
    string line;

    while(getline(input, line))
    {
        stringstream input;
        input << line;
        input >> lower  >> higher >> male_fraction >> female_fraction;

        // assign males
        int num_males = num_from_fraction(male_fraction, 'M');
        for(int i = 0; i < num_males; ++i)
        {
            person m;
            m.sex = 'M';
            set_age(r, m, lower, higher);
            data.push_back(m);
        }

        // assign females
        int num_females = num_from_fraction(female_fraction, 'F');
        for(int i = 0; i < num_females; ++i)
        {
            person f;
            f.sex = 'F';
            set_age(r, f, lower, higher);
            data.push_back(f);
        }
    }

    input.close();
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  import_prevalence_data
 *  Description:  imports prevalence distribution data from prevalence_data.csv 
 * =====================================================================================
 */
void import_prevalence_data(list<prevalence_data> &data){
    ifstream input(PREVALENCE_DATA, ios::in);
    if(input.fail()) 
    {
        cout << "Input error: could not open" << PREVALENCE_DATA << endl;
        exit(-1);
    }
   
    int lower, higher; // age brackets
    float male_fraction, female_fraction; // percentages
    string line;

    while(getline(input, line))
    {
        stringstream input;
        input << line;
        input >> lower  >> higher >> male_fraction >> female_fraction;
        prevalence_data p(lower, higher, male_fraction, female_fraction);
        data.push_back(p); 
    }

    input.close();
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  import_birth_data
 *  Description:  imports birth distribution data from birth_data.csv 
 * =====================================================================================
 */
void import_birth_data(list<birth_data> &data){
    ifstream input(BIRTH_DATA, ios::in);
    if(input.fail()) 
    {
        cout << "Input error: could not open" << BIRTH_DATA << endl;
        exit(-1);
    }
   
    int lower, higher; // age brackets
    float fraction; // percentages
    string line;

    while(getline(input, line))
    {
        stringstream input;
        input << line;
        input >> lower  >> higher >> fraction;
        birth_data b(lower, higher, fraction);
        data.push_back(b); 
    }

    input.close();
}

#endif

