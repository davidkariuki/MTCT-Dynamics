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
  	-Resistance mutation tracking
    -Arv dynamics / failure
	-prediction of mutations
    -how does viral load respond to treatment based on effectiveness
    -if viral load changes, how does cd4 respond
 *-----------------------------------------------------------------------------*/

#include "headers/utils.h"
#include "headers/inputs.h"
#include "headers/outputs.h"
#include "headers/person.h"
#include "headers/transmission.h"
#include "headers/deaths.h"
#include "headers/births.h"
#include "headers/viral_dynamics.h"


/*-----------------------------------------------------------------------------
 *  Function Definitions 
 *-----------------------------------------------------------------------------*/
void set_age(CRandomMersenne &, person &, int, int);
void update_age(list<person> &S, list<person> &I);
void age_updater(person &p);
void set_initial_inf_duration(person &p, CRandomMersenne &r);
void initialize_population(list<person> &,list<person> &, list<prevalence_data> &, StochasticLib1 &, CRandomMersenne &);
void update_population(list<person> &infected, list<person> &susceptible, StochasticLib1& sto);
void update_viral_load(person &, StochasticLib1 &);
void population_stats(list<person> &, int &);
int process_flags(int argc, char* argv[]);
bool init_infect(person &, prevalence_data &, StochasticLib1 &); 
void compute_infected(list<person> &susceptible, list<person> &infected, StochasticLib1 &s, CRandomMersenne &r);
void shuffle_population(list<person> &population);



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  main is called with number of runs to simulate TODO: other params
 *
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
    list<death_data> death_data;
    list<float> birth_rate_data;
    ofstream prev_out("outputs/prevalence.txt", ios::out), out("outputs/population.txt", ios::out);
    //set up data
    import_prevalence_data(pr_data);
    import_birth_rates(birth_rate_data);
    import_death_data(death_data);
	initialize_population(susceptible, infected, pr_data, s, r);

    float b_rate;
    list<float>::iterator birthrate_itr = birth_rate_data.begin();
    //print prleminary (day 0) stats
    //
    cout << "time 0: ";
    print_prevalence(susceptible, infected, prev_out);

    int year = 0;
    // run simulation
    

    int total = 0, prev_total = susceptible.size() + infected.size();

    for(int i = 0; i < NUM_ITERATIONS; ++i) // 3 month intervals
    {
        update_births(susceptible, infected, b_rate, s);
        shuffle_population(susceptible);
        compute_infected(susceptible, infected, s, r);
        update_cd4_count(infected, r);
        update_age(susceptible, infected); // quarterly update
        output_population_statistics(susceptible, infected, removed, out);
        
        //update_cd4_count(
        if(i % 20 == 0) // every 5 years
        {
            if(birthrate_itr != birth_rate_data.end())
            {
                b_rate = *birthrate_itr;
                ++birthrate_itr;
            }
        }

        if(i  % 4 == 0) // every year
        {
            compute_deaths(susceptible, infected, removed, death_data, s); // life tables assume annual
            map<string, int> S, I, R;
            ++year;
            cout << "Year: " << year << "  \t";
            total = susceptible.size() + infected.size();
            float d_p = 100.0 * (total - prev_total) / prev_total;
            prev_total = total;
            cout << "change (TOTAL population): " << d_p <<  "% (" << total << ")" << "\t";
            // TODO: this outputs/couts
            print_prevalence(susceptible, infected, prev_out);
            //cout << "susceptibles: " << susceptible.size() << " infected: " 
             //   << infected.size() << " removed: " << removed.size() << endl << endl;
            print_population_changes(susceptible, infected, removed, S, I, R);
        }
	}


    out.close();
    prev_out.close();
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
 *         Name:  shuffle_population
 *  Description:  simulates random mixing for susceptible population
 * =====================================================================================
 */
void shuffle_population(list<person> & population)
{
    vector<person> temp_container(population.begin(), population.end());
    random_shuffle(temp_container.begin(), temp_container.end());
    population.assign(temp_container.begin(), temp_container.end());
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
    import_population_data(susceptible, r, s);

    // infect males and females based on prevalence data
    bool infctd;
    for(itr = susceptible.begin(); itr != susceptible.end(); )
    {
        
        for(pd_itr = pr_data.begin(); pd_itr != pr_data.end(); ++pd_itr)
        {
           infctd = init_infect(*itr, *pd_itr, s);
           
           if(infctd)
           {
               set_initial_inf_duration(*itr, r);
               infected.push_back(*itr);
               itr = susceptible.erase(itr); 
               break;
           }               
       }

        if(!infctd)
            ++itr;

   }
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  set_initial_inf_duration
 *  Description:  randomizes duration of infection for initial population (years)
 * =====================================================================================
 */
void set_initial_inf_duration(person &p, CRandomMersenne &r)
{
    float duration = r.IRandomX(10, 100) / 10.0;
    p.infection_duration = duration;
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
               p.viral_load = s.Normal(5, 1);
               return true;
           }
           else return false;
        }
        else // sex == 'F'
        {
           if(s.Binomial(1, pr.female_f))
            {
               p.status = "infected";
               p.viral_load = s.Normal(5,1); 
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
 *         Name:  age_updater
 *  Description:  helper function for update_age (0.25 for quarterly age updates)
 * =====================================================================================
 */
void age_updater(person &p)
{
   p.age += 0.25;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  update_age
 *  Description:  updates population ages every quarter
 * =====================================================================================
 */
void update_age(list<person> &S, list<person> &I)
{
    for_each(S.begin(), S.end(), age_updater);
    for_each(I.begin(), I.end(), age_updater);
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
*/





