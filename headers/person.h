/*
 *  Header: person.h
 *  Created by David Kariuki.
 *
 */

#ifndef __person_h__
#define __person_h__

#include <iostream>
#include <cstdlib>
#include <set> 
#include <algorithm>
#include "virus.h"
#include "stocc.h"
#include "randomc.h"

using namespace std;

#define DEFAULT_CD4_COUNT 1500.0
#define DEFAULT_VIRAL_LOAD 0.0 
#define INITIAL_AGE 15
#define DEFAULT_VIRUS_TYPE "R5"

class person 
{
	public:
	// constructors
		explicit person();
	// public member functions
        void print_data();
    // public data members
        		
        friend ostream &operator << (ostream &cout, const person &elem)
        {
            cout <<  elem.sex << " " << elem.age << " " << elem.status;
            return cout;
        }

        unsigned int ID, infection_date, clinical_stage;
		float age, cd4_count, cholesterol;
		char sex;
		string risk_group; // IDU, MSM, blood etc
        float viral_load; // TODO: this has overlap in virus.h also, log
        string virus_type; // TODO: same as viral_load
		virus hiv; // TODO: is this needed?
        string status;		
	private:
			
};

/* DEFAULT CONSTRUCTOR */
person::person()
{
    status = "susceptible";
    cd4_count = DEFAULT_CD4_COUNT;
    viral_load = DEFAULT_VIRAL_LOAD;
    clinical_stage = 1;
}

// print stats
void person::print_data() 
{
    cout << ID << " age: " <<  age << "status: " << status;
	//printf("%-5d %-3c %-5.1f %-8.1f %5.3f", ID, sex, age, cd4_count, hiv.viral_load);
	//for_each(cur_regiment.begin(), cur_regiment.end(), print_arv());
	printf("\n");
}

#endif
