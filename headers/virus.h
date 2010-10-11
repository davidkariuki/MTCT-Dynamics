/*
 *  virus.h
 *  simulation
 *
 */

#ifndef __virus_h__
#define __virus_h__

#define DEFAULT_VIRUSTYPE "R5"

#include<string>

using namespace std;

class virus 
{
    public: //TODO: think about protected ?
		friend class patient;
		virus();
		virus(string);
		string type;
		bool mutaiton_present;
		string mutation_type;
		float viral_load;
};

// default constructor
virus::virus() 
{
	type = DEFAULT_VIRUSTYPE; // should work this out with probablities
}


// parameterized constructor
virus::virus(string new_type)
{
	type = new_type;
}

#endif
