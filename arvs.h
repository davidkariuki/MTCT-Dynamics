
/*
 *  arvs.h
 *  simulation
 *
 *  Created by David Kariuki on 7/13/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __arvs_h__
#define __arvs_h__

#include<string>
#include<iomanip>

using namespace std;
#define DEFAULT_ACTIVITY 1.0
#define DEFAULT_RESISTANCE 0.0


class arvs {
	public:
	//constructors
		arvs();
		arvs(string, float);
		void print_data() const;
	//variables
		string name;
		bool in_use;
		bool prev_use;
		bool prev_failure;
		bool prev_toxicity_stop;
		float resistance_level;
		float activity_level;
		
	// functions

	
	// patient class acesses and modifies all data members
		friend class patient;

};


/* DEFAULT CONSTRUCTOR */
arvs::arvs(){
	activity_level = DEFAULT_ACTIVITY;
	in_use = true;
	prev_use = false;
	prev_failure = false;
	prev_toxicity_stop = false;
	resistance_level = DEFAULT_RESISTANCE;
	name = "";
}

/* PARAMETERIZED CONSTRUCTOR */
arvs::arvs(string new_name, float new_activity_level){
	activity_level = new_activity_level;
	in_use = true;
	prev_use = false;
	prev_failure = false;
	prev_toxicity_stop = false;
	resistance_level = DEFAULT_RESISTANCE;
	name = new_name;
	
}
/* PRINT DATA */
void arvs::print_data() const {
	cout << "\t" << name << ": " << activity_level;
	
}


#endif
