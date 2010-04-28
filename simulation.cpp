/*
 * =====================================================================================
 *
 *       Filename:  simulation.cpp
 *    Description:  contains the main simulation code
 *        Created:  7/13/09 15:33:15
 *         Author:  David Kariuki
 *
 * =====================================================================================
 */

/*-----------------------------------------------------------------------------
 * TODO: Implement:
  	-Resistance
    -Arv dynamics / failure
	-prediction of mutations
    -how does viral load respond to treatment based on effectiveness
    -if viral load changes, how does cd4 respond
 *-----------------------------------------------------------------------------*/

#include <iterator>
#include "inputs.h"
#include "outputs.h"
#include "person.h"
#include "utils.h"
#include "transmission.h"
#include "deaths.h"

using namespace std;

/*-----------------------------------------------------------------------------
 *  Function Definitions 
 *-----------------------------------------------------------------------------*/
void set_age(CRandomMersenne &, person &, int, int);
void initialize_population(list<person> &,list<person> &, list<prevalence_data> &, StochasticLib1 &, CRandomMersenne &);
void update_population(list<person> &infected, list<person> &susceptible, StochasticLib1& sto);
void update_viral_load(person &, StochasticLib1 &);
void update_cd4_count(person &, StochasticLib1 &);
void process_virus_type(person &, StochasticLib1 &);
void population_stats(list<person> &, int &);
int process_flags(int argc, char* argv[]);
bool init_infect(person &, prevalence_data &, StochasticLib1 &); 
void update_births(list<person> &, list<person> &, list<birth_data> &, StochasticLib1 &);
int process_flags(int argc, char* argv[]);
void compute_infected(list<person> &susceptible, list<person> &infected, StochasticLib1 &s);
void compute_removed(list<person> &susceptible, list<person> &infected, list<person> &removed, StochasticLib1 &s);
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  main is called with number of runs to simulate TODO: other params
 * =====================================================================================
 */
int main(int argc, char* argv[])
{
	int32_t seed = (int32_t)time(0);
	StochasticLib1 s(seed);
	CRandomMersenne r((int)seed);
	process_flags(argc, argv);
	list<person> infected, removed;	
    list<person> susceptible;
    list<prevalence_data> pr_data;	
    list<birth_data> br_data;

    //set up data
    import_prevalence_data(pr_data);
    import_birth_data(br_data);
	initialize_population(susceptible, infected, pr_data, s, r);
   
    print_prevalence(susceptible, infected);
    cout << "susceptibles: " << susceptible.size() << " infected: " << infected.size() << endl << endl;
    cout << "removed: " << removed.size() << endl;

    int year = 0;
    // run simulation
    
    for(int i = 0; i < NUM_ITERATIONS; ++i) // 3 month intervals
    {
        update_births(susceptible, infected, br_data, s);
        compute_infected(susceptible, infected, s);
        compute_removed(susceptible, infected, removed, s);

        if(i  % 4 == 0)
        {
            ++year;
            cout << "Year: " << year << endl;
            output_age_specific_prevalence(s, susceptible, infected); // writes to output
            print_prevalence(susceptible, infected);
            cout << "susceptibles: " << susceptible.size() << " infected: " 
                << infected.size() << " removed: " << removed.size() << endl << endl;
        }
	}
    return 0;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  process_flags
 *  Description:  processes input flags  
 * =====================================================================================
 */
int process_flags(int argc, char* argv[])
{
    if((argc % 2) == 0) 
    {
        cout << "invalid number of arguments specified.\n";
        cout << "usage: " << argv[0] << " <flag> <value> ";
        exit(-1);
    }
    
    int num_flags = (argc-1)/2;
    string* flags = new string[num_flags];
    const char** values = new const char*[num_flags];
    
    int j = 0;
    for(int i = 1; i+1 < argc; ++i)
    {
        flags[j] = string(argv[i]);
        values[j] = argv[i+1];
        ++i;
        ++j;
    }
    
    // validate flags 
    validate_flags(flags, values, num_flags);
    // if no flags provided, globals will use default values
    initialize_variables(flags, values, num_flags);
    
    delete [] flags;
    delete [] values;

	return 0;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  initialize_population
 *  Description:  infects an initial population of size defined in utils.h based on the
 *                population and prevalence data provided.
 * =====================================================================================
 */
void initialize_population(list<person> &susceptible, list<person> &infected, list<prevalence_data> &pr_data, StochasticLib1 &s, CRandomMersenne &r)
{
    list<person>::iterator itr;
    list<prevalence_data>::iterator pd_itr;

    // get age data from file & generate numbers of males and females
    import_population_data(susceptible, r);

    // infect males and females based on prevalence datai
    bool moved = false;
    for(itr = susceptible.begin(); itr != susceptible.end(); ++itr)
    {
         if(moved)
        {
            --itr;
            moved = false;
        }
        
        for(pd_itr = pr_data.begin(); pd_itr != pr_data.end(); ++pd_itr)
        {
           if(init_infect(*itr, *pd_itr, s))
           {
               infected.push_back(*itr);
               itr = susceptible.erase(itr); 
               moved = true;
               continue;
           }               
       }
   }
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_infect
 *  Description:  uses prevalence and age data to infect a person  (initial population)
 * =====================================================================================
 */
bool init_infect(person &p, prevalence_data &pr, StochasticLib1 &s) 
{
   if(p.age >= pr.lower && p.age < pr.higher + 1)
    {
        if(p.sex == 'M') 
        {
           if(s.Binomial(1, pr.male_f))
           {
               p.status = "infected";
               return true;
           }
           else return false;
        }
        else // sex == 'F'
        {
           if(s.Binomial(1, pr.female_f))
            {
               p.status = "infected";
               return true;
           }
           else return false;
        }
    }
    return false; // not within age range
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  update_population
 *  Description:  updates the population for a single time step TODO: needs fixing
 * =====================================================================================
 */
void update_population(list<person> &infected, list<person> &susceptible, StochasticLib1& sto)
{
    // update age	
    // update births
    // update infecteds / susceptibles
     
}

   
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  update_births
 *  Description:  adds children based on birth_probability defined in utils.h
 *                TODO: implement infecteds MTC
 * =====================================================================================
 */
void update_births(list<person> &susceptible, list<person> &infected, list<birth_data> &data, StochasticLib1& s)
{
    list<person>::iterator itr;

    for(itr = susceptible.begin(); itr != susceptible.end(); ++itr)
{
        if(evaluate_birth_probability(*itr, data, s)) 
        {
           person p;
           set_sex(p, s);
           p.age = 0;
           susceptible.push_front(p); // TODO: ID? Other things to set?
           
        }
    }
    
    for(itr = infected.begin(); itr != infected.end(); ++itr)
    {
        if(evaluate_birth_probability(*itr, data, s)) 
        {
           person p;
           set_sex(p, s);
           p.age = 0;
           susceptible.push_front(p); // TODO: ID? Other things to set?
        }
    }
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  compute_infected
 *  Description:  goes over the susceptible population and infects based on infection
 *                probability by number of coital acts
 * =====================================================================================
 */
void compute_infected(list<person> &susceptible, list<person> &infected, StochasticLib1 &s)
{
    list<person>::iterator itr;
    bool moved = false;

    for(itr = susceptible.begin(); itr != susceptible.end(); ++itr)
    {
        if(moved)
        {
            --itr;
            moved = false;
        }
       
        if(person_infected(*itr, s))
        {
            infected.push_back(*itr);
            itr = susceptible.erase(itr); 
            moved = true;
        }
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  compute_removed
 *  Description:  removes from susceptible and infected popoulations based on mortality
 *                data TODO: finish this
 * =====================================================================================
 */
void compute_removed(list<person> &susceptible, list<person> &infected, list<person> &removed, StochasticLib1 &s)
{
    list<person>::iterator itr;
    bool  iterator_moved = false;

    for(itr = susceptible.begin(); itr != susceptible.end(); ++itr)
    {
        if(iterator_moved) // deletion moves iterator to next element, prevent skipping
        {
            --itr;
            iterator_moved = false;
        }
       
        if(person_removed(*itr, s) == true)
        {
            removed.push_back(*itr);
            itr = susceptible.erase(itr); 
            iterator_moved = true;
        }
    }

    for(itr = infected.begin(); itr != infected.end(); ++itr)
    {
        if(iterator_moved) // deletion moves iterator to next element, prevent skipping
        {
            --itr;
            iterator_moved = false;
        }
       
        if(person_removed(*itr, s) == true)
        {
            removed.push_back(*itr);
            itr = infected.erase(itr); 
            iterator_moved = true;
        }
    }

}


/* UPDATE VIRAL LOAD
	- v(t) = v(t-1) + delta_v(t-1)
	- maximum allowed viral load is 6.5 (log copies/mL)
*/
void update_viral_load(person &p, StochasticLib1 &sto)
{
	float vl_prev = p.viral_load;
	float delta_v = 0.0;
	
	if(vl_prev < 3) { delta_v = 0.11/4 + sto.Normal(0, 0.3); }
	else if(vl_prev >= 3 && vl_prev < 3.5) { delta_v = 0.13/4 + sto.Normal(0, 0.3); }
	else if(vl_prev >= 3.5 && vl_prev < 4) { delta_v = 0.14/4 + sto.Normal(0, 0.3); }
	else if(vl_prev >= 4 && vl_prev < 4.5) { delta_v = 0.15/4 + sto.Normal(0, 0.3); }
	else if(vl_prev >= 4.5 && vl_prev < 5) { delta_v = 0.17/4 + sto.Normal(0, 0.3); }
 	else if(vl_prev >= 5 && vl_prev < 5.5) { delta_v = 0.18/4 + sto.Normal(0, 0.3); }
	else if(vl_prev >= 5.5 && vl_prev < 6) { delta_v = 0.20/4 + sto.Normal(0, 0.3); }
	else if(vl_prev >= 6 && vl_prev < 6.5) { delta_v = 0.21/4 + sto.Normal(0, 0.3); }
	
	float vl_new = vl_prev + delta_v;
	p.viral_load = vl_new < 6.5 ? vl_new : 6.5; 
}


/* UPDATE CD4 COUNT 
	- CD4(t) = CD4(t-1) + delta_CD4(t-1)
	- updates a single person's cd4 count based on viral load, age and presence of the x4 
	  virus. 
	- vl_prev = virus load at time t-1
	- cd4_sq_prev = square root of cd4 count at time t-1
	- delta_cd4_sq = change in square root of cd4 count
*/
void update_cd4_count(person &p, StochasticLib1 &sto)
{	
	float vl_prev = p.viral_load;
	float cd4_sq_prev = p.cd4_count;
	float delta_cd4_sq = 0.0; 
	float age = p.age;

	// update cd4 count based on viral load
	if     (vl_prev < 3)		           { delta_cd4_sq = -0.000 + sto.Normal(0, 0.8); }
	else if(vl_prev >= 3 && vl_prev < 3.5) { delta_cd4_sq = -0.016 + sto.Normal(0, 0.8); }
	else if(vl_prev >= 3.5 && vl_prev < 4) { delta_cd4_sq = -0.040 + sto.Normal(0, 0.8); }
	else if(vl_prev >= 4 && vl_prev < 4.5) { delta_cd4_sq = -0.120 + sto.Normal(0, 0.8); }
	else if(vl_prev >= 4.5 && vl_prev < 5) { delta_cd4_sq = -0.400 + sto.Normal(0, 0.8); }
	else if(vl_prev >= 5 && vl_prev < 5.5) { delta_cd4_sq = -0.800 + sto.Normal(0, 0.8); }
	else if(vl_prev >= 5.5 && vl_prev < 6) { delta_cd4_sq = -1.600 + sto.Normal(0, 0.8); }
	else if(vl_prev >= 6)				   { delta_cd4_sq = -2.000 + sto.Normal(0, 0.8); }

	// update cd4 count based on age
	if     (age < 20)			   { delta_cd4_sq +=  0.15; }
	else if(age >= 20 && age < 25) { delta_cd4_sq +=  0.09; }
	else if(age >= 25 && age < 30) { delta_cd4_sq += -0.06; }
	else if(age >= 30 && age < 35) { delta_cd4_sq += -0.00; }
	else if(age >= 35 && age < 40) { delta_cd4_sq += -0.00; }
	else if(age >= 40 && age < 45) { delta_cd4_sq += -0.06; }
	else if(age >= 45 && age < 50) { delta_cd4_sq += -0.09; }
	else if(age >= 50 && age < 60) { delta_cd4_sq += -0.15; }
	else if(age >= 60)			   { delta_cd4_sq += -0.20; }	
	if(p.virus_type == "X4") { delta_cd4_sq += -0.25; } // update for presence of x4 virus
	
	p.cd4_count = delta_cd4_sq + cd4_sq_prev;
}




/* PROCESS VIRUS TYPE */ //TODO: mutation type maybe?
void process_virus_type(person &p, StochasticLib1 &sto) 
{
	float v_prev = p.viral_load;
	float p_x4 = pow(10, v_prev) * 0.0000004; 
	if(sto.Binomial(1, p_x4 > 1 ? 1 : p_x4)){
		p.virus_type = "x4";
	}	
}

/* DETERMINE RESISTANCE */ // TODO: incomplete
void determine_resistance(person &p) {
	int inf_date = p.infection_date;
	
	if(inf_date > 1997)
		;
}





