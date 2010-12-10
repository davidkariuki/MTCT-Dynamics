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

#include "utils.h"


void output_age_specific_prevalence(StochasticLib1 &s, list<person> &susceptible, list<person> &infected)
{
    list<person>::iterator itr;
    ofstream prev_out;

    prev_out.open("ouputs/prevalence.txt", ios::out);
    copy(infected.begin(), infected.end(), ostream_iterator<person> (prev_out, "\n"));
    
    prev_out.close();
    //copy(susceptible.begin(), susceptible.end(), ostream_iterator<person> (cout, "\n"));

}


void output_population_statistics(list<person> &S, list<person> &I, list<person> &R, ofstream &output)
{
    static int count = 1;
    if(count == 1)
        output << "quarter\tS\tI\tR\n";

    output << count << "\t" << S.size() <<"\t" << I.size() << "\t" << R.size() << "\n";
     ++count;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  print_prevalence
 *  Description:  prints prevalence (ages 15 - 49 from DHS Malawi 2004)
 * =====================================================================================
 */
void print_prevalence(list<person> &susceptible, list<person> &infected, ofstream &prev_out)
{
    int num_infected = count_if(infected.begin(), infected.end(), within_age_range(15, 49));
    int num_susceptible = count_if(susceptible.begin(), susceptible.end(), within_age_range(15, 49));
    //cout << "infected 15-49: " << num_infected;
    //cout << "\tsusceptible 15-49: " << num_susceptible;
    float prevalence = 100.0 * num_infected / (num_susceptible + num_infected); // percentage
    cout << "prevalence: " << prevalence << "%" << endl;
    prev_out << prevalence << "\n"; 

}



void populate_changes_map(list<person> &L, map<string, int> &M)
{
    list<person>::iterator itr;
    for(itr = L.begin(); itr != L.end(); ++itr)
    {
        int upper = 4, lower = 0; 
        for(lower = 0; lower < 106; ) {
            stringstream stream;
            stream << lower << "-" << upper;
            string key = stream.str(); 
            if(within_age(*itr, lower, upper)) 
            {
                M[key] += 1;
                break; // exit inner for loop
            }

            upper+=5;
            lower+=5;
        }
    }
}

void print_line(string range, map<string, int> &S, map<string, int> &I, map<string, int> &R,
                              map<string, int> &old_S, map<string, int> &old_I, map<string, int> &old_R)
{
/* 
    double S_val = (S.find(range) == S.end()) ? 0 : (100.0 * (S[range] - old_S[range]) / S[range]) ;
    double I_val = (I.find(range) == I.end()) ? 0 : (100.0 * (I[range] - old_I[range]) / I[range]) ;
    double R_val = (R.find(range) == R.end()) ? 0 : (100.0 * (R[range] - old_R[range]) / R[range]) ;

    cout << setw(10) << range << ": "
         << fixed << showpoint << setprecision(1) << setw(10) << S_val << "%" 
         << fixed << showpoint << setprecision(1) << setw(10) << I_val << "%" 
         << fixed << showpoint << setprecision(1) << setw(10) << R_val << "%\n" ;
*/
    int S_val = (S.find(range) == S.end()) ? 0 : S[range] ;
    int I_val = (I.find(range) == I.end()) ? 0 : I[range] ;
    int R_val = (R.find(range) == R.end()) ? 0 : R[range] ;
    
    
cout << setw(10) << range << ": " 
     << setw(10) << S_val  
     << setw(10) << I_val 
     << setw(10) << R_val << "\n";
 }





void print_population_changes(list<person> &s, list<person> &i, list<person> &r, 
        map<string, int> &S, map<string, int> &I, map<string, int> &R)
{
    map<string, int>::iterator itr;
    map<string, int> old_S = S;
    map<string, int> old_R = R;
    map<string, int> old_I = I;

    populate_changes_map(s, S);
    populate_changes_map(i, I);
    populate_changes_map(r, R);
    int lower = 0, upper = 4;
    cout << "========================== Population Changes (S I R) ==========================\n";
    for(lower = 0; lower < 106; ) {
        stringstream stream;
        stream << lower << "-" << upper;
        string key = stream.str(); 
        print_line(key, S, I, R, old_S, old_I, old_R);
        lower += 5;
        upper += 5;
    }
    cout <<  "==================================================================================\n";
}

#endif


