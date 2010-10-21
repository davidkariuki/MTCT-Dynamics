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
        output << "quarter\tsusceptibles\tinfected\tremoved\n";

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
        if(within_age(*itr, 0, 5))          M["0-5"] += 1;
        else if(within_age(*itr, 6, 10))    M["6-10"] += 1;
        else if(within_age(*itr, 11, 15))   M["11-15"] += 1;
        else if(within_age(*itr, 16, 20))   M["16-20"] += 1;
        else if(within_age(*itr, 21, 25))   M["21-25"] += 1;
        else if(within_age(*itr, 26, 30))   M["26-30"] += 1;
        else if(within_age(*itr, 31, 35))   M["31-35"] += 1;
        else if(within_age(*itr, 36, 40))   M["36-40"] += 1;
        else if(within_age(*itr, 41, 45))   M["41-45"] += 1;
        else if(within_age(*itr, 46, 50))   M["46-50"] += 1;
        else if(within_age(*itr, 51, 55))   M["51-55"] += 1;
        else if(within_age(*itr, 56, 60))   M["56-60"] += 1;
        else if(within_age(*itr, 61, 65))   M["61-65"] += 1;
        else if(within_age(*itr, 66, 70))   M["65-70"] += 1;
        else if(within_age(*itr, 71, 75))   M["71-75"] += 1;
        else if(within_age(*itr, 76, 80))   M["76-80"] += 1;
        else if(within_age(*itr, 81, 85))   M["81-85"] += 1;
        else if(within_age(*itr, 86, 90))   M["86-90"] += 1;
        else if(within_age(*itr, 91, 95))   M["91-95"] += 1;
        else if(within_age(*itr, 96, 100))  M["96-100"] += 1;
        else if(within_age(*itr, 101, 105)) M["101-105"] += 1;
        else if(within_age(*itr, 106, 110)) M["106-110"] += 1;
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

    cout << "========================== Population Changes (S I R) ==========================\n";
    print_line("0-5", S, I, R, old_S, old_I, old_R);
    print_line("6-10", S, I, R, old_S, old_I, old_R);
    print_line("11-15", S, I, R, old_S, old_I, old_R);
    print_line("16-20", S, I, R, old_S, old_I, old_R);
    print_line("21-25", S, I, R, old_S, old_I, old_R);
    print_line("26-30", S, I, R, old_S, old_I, old_R);
    print_line("31-35", S, I, R, old_S, old_I, old_R);
    print_line("36-40", S, I, R, old_S, old_I, old_R);
    print_line("41-45", S, I, R, old_S, old_I, old_R);
    print_line("46-50", S, I, R, old_S, old_I, old_R);
    print_line("51-55", S, I, R, old_S, old_I, old_R);
    print_line("56-60", S, I, R, old_S, old_I, old_R);
    print_line("61-65", S, I, R, old_S, old_I, old_R);
    print_line("66-70", S, I, R, old_S, old_I, old_R);
    print_line("71-75", S, I, R, old_S, old_I, old_R);
    print_line("76-80", S, I, R, old_S, old_I, old_R);
    print_line("81-85", S, I, R, old_S, old_I, old_R);
    print_line("86-90", S, I, R, old_S, old_I, old_R);
    print_line("91-95", S, I, R, old_S, old_I, old_R);
    print_line("96-100", S, I, R, old_S, old_I, old_R);
    print_line("101-105", S, I, R, old_S, old_I, old_R);
    print_line("106-110", S, I, R, old_S, old_I, old_R);
    cout <<  "==================================================================================\n";
}

#endif


