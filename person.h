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
#include "arvs.h"
#include "virus.h"
#include "rand/stocc.h"
#include "rand/randomc.h"

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
		void start_arv(arvs &);	
		void stop_arv(string, string);
    // public data members
        struct cmp 
		{
			bool operator () (const arvs& left, const arvs& right) const 
			{
				return left.name.compare(right.name) == -1;
			}
		};
		
        friend ostream &operator << (ostream &cout, const person &elem)
        {
            cout <<  elem.sex << " " << elem.age << " " << elem.status;
            return cout;
        }

        unsigned int ID, infection_date, clinical_stage;
		float age, cd4_count, cholesterol;
		char sex;
		string risk_group; // IDU, MSM, blood etc
		multiset<arvs, cmp> cur_regiment; 
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
    clinical_stage = 1;
}

// print stats helper
struct print_arv 
{
	public: void operator()(arvs x) { x.print_data(); }
};

// print stats
void person::print_data() 
{
    cout << ID << " age: " <<  age << "status: " << status;
	//printf("%-5d %-3c %-5.1f %-8.1f %5.3f", ID, sex, age, cd4_count, hiv.viral_load);
	//for_each(cur_regiment.begin(), cur_regiment.end(), print_arv());
	printf("\n");
}

// start person on new arv
void person::start_arv(arvs &new_arv)
{
	cur_regiment.insert(new_arv);
}

// stop arv
void person::stop_arv(string arv, string reason)
{
	if(reason == "toxicity")
		cout << 1;
}

#endif
