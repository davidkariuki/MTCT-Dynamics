/*
 * =====================================================================================
 *
 *       Filename:  outputs.h
 *    Description:  contains all data output functions
           Author:  David Kariuki
 *
 * =====================================================================================
 *
 */

#ifndef __outputs_h__
#define __outputs_h__

#include <fstream>
#include <iterator>
#include "person.h"
#include <list>

using namespace std;


void output_age_specific_prevalence(StochasticLib1 &s, list<person> &susceptible, list<person> &infected)
{
    list<person>::iterator itr;
    ofstream prev_out;

    prev_out.open("outputs.txt", ios::out);
    copy(infected.begin(), infected.end(), ostream_iterator<person> (prev_out, "\n"));
    
    prev_out.close();
    //copy(susceptible.begin(), susceptible.end(), ostream_iterator<person> (cout, "\n"));

}


void output_population_statistics(list<person> &S, list<person> &I, list<person> &R, ofstream &output)
{
    static int count = 1;
    if(count == 1)
        output << "quarter\tsusceptibles\tinfected\tremoved\n";

    output << count << "\t" << S.size() <<"\t" << I.size() << "\t" << R.size() << "\n";
     ++count;
}

#endif
