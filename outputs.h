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

void output_age_specific_prevalence(StochasticLib1 &s, list<person> &susceptible, list<person> &infected)
{
    list<person>::iterator itr;
    ofstream prev_out;

    prev_out.open("outputs.txt", ios::out);
    copy(infected.begin(), infected.end(), ostream_iterator<person> (prev_out, "\n"));
    
    prev_out.close();
    //copy(susceptible.begin(), susceptible.end(), ostream_iterator<person> (cout, "\n"));

}


#endif
