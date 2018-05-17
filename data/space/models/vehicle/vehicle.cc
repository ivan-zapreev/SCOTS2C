/*
 * vehicle.cc
 *
 *  created: Oct 2016
 *   author: Matthias Rungger
 */

/*
 * information about this example is given in
 * http://arxiv.org/abs/1503.03715
 * doi: 10.1109/TAC.2016.2593947
 */

#include <iostream>
#include <array>
#include <cstdlib>
#include <string>
#include <algorithm>

/* SCOTS header */
#include "scots.hh"
/* ode solver */
#include "RungeKutta4.hh"
/*The helper routines*/
#include "scots_examples_helper.hh"

/* time profiling */
#include "TicToc.hh"
/* memory profiling */
#include <sys/time.h>
#include <sys/resource.h>
struct rusage usage;

using namespace std;
using namespace scots;
using namespace tud::ctrl::scots::examples::helper;

/* state space dim */
const int state_dim=3;
/* input space dim */
const int input_dim=2;

/* sampling time */
const double tau = 0.3;

/*
 * data types for the state space elements and input space
 * elements used in uniform grid and ode solvers
 */
using state_type = array<double,state_dim>;
using input_type = array<double,input_dim>;

/* abbrev of the type for abstract states and inputs */
using abs_type = abs_type;

/* we integrate the vehicle ode by tau sec (the result is stored in x)  */
auto  vehicle_post = [](state_type &x, const input_type &u) {
    /* the ode describing the vehicle */
    auto rhs =[](state_type& xx,  const state_type &x, const input_type &u) {
        double alpha=atan(tan(u[1])/2.0);
        xx[0] = u[0]*cos(alpha+x[2])/cos(alpha);
        xx[1] = u[0]*sin(alpha+x[2])/cos(alpha);
        xx[2] = u[0]*tan(u[1]);
    };
    /* simulate (use 10 intermediate steps in the ode solver) */
    runge_kutta_fixed4(rhs,x,u,state_dim,tau,10);
};

/* we integrate the growth bound by 0.3 sec (the result is stored in r)  */
auto radius_post = [](state_type &r, const state_type &, const input_type &u) {
    double c = abs(u[0])*sqrt(tan(u[1])*tan(u[1])/4.0+1);
    r[0] = r[0]+c*r[2]*tau;
    r[1] = r[1]+c*r[2]*tau;
};

int main(int argc, char** argv) {
    /* to measure time */
    TicToc tt;
    /* cudd manager */
    Cudd manager;
    manager.AutodynEnable();
    
    /*Get the eta divider*/
    uint32_t divider = 1;
    string suffix = "";
    if(argc >= 2){
        divider = max(1, abs(atoi(argv[1])));
    }
    suffix = "_";
    suffix += to_string(divider);
    
    /* try to read data from files */
    SymbolicSet ss_pre;
    SymbolicSet ss_post;
    //if(!read_from_file(manager,ss_pre,"state_pre") ||
    //   !read_from_file(manager,ss_post,"state_post")) {
        /* lower bounds of the hyper rectangle */
        state_type s_lb={{0,0,-3.5}};
        /* upper bounds of the hyper rectangle */
        state_type s_ub={{10,10,3.5}};
        /* grid node distance diameter */
        state_type s_eta={{.2 / divider,.2 / divider,.2 / divider}};
        /* construct SymbolicSet with the UniformGrid information for the state space
         * and BDD variable IDs for the pre */
        ss_pre = SymbolicSet(manager, state_dim,s_lb,s_ub,s_eta);
        /* construct SymbolicSet with the UniformGrid information for the state space
         * and BDD variable IDs for the post */
        ss_post = SymbolicSet(manager, state_dim,s_lb,s_ub,s_eta);
        
        //write_to_file(ss_pre,"state_pre");
        //write_to_file(ss_post,"state_post");
    //}
    cout << "Unfiorm grid details:" << endl;
    ss_pre.print_info(1);
    
    /* try to read data from files */
    SymbolicSet ss_input;
    //if(!read_from_file(manager,ss_input,"input_alphabet")) {
    /* construct grid for the input space */
    /* lower bounds of the hyper rectangle */
    input_type i_lb={{-1,-1}};
    /* upper bounds of the hyper rectangle */
    input_type i_ub={{ 1, 1}};
    /* grid node distance diameter */
    input_type i_mu={{.3,.3}}; //Original value {{.3,.3}}
    ss_input = SymbolicSet(manager, input_dim,i_lb,i_ub,i_mu);
    //write_to_file(ss_input,"input_alphabet");
    //}
    ss_input.print_info(1);
    
    BDD TF;
    /* initialize SymbolicModel class with the abstract state and input alphabet */
    SymbolicModel<state_type,input_type> sym_model(ss_pre,ss_input,ss_post);
    /* does there exist the transition function file ?*/
    //SymbolicSet set;
    //if(!read_from_file(manager,set,TF,"tf")) {
    /* set up constraint functions with obtacles */
    double H[15][4] = {
        { 1  , 1.2, 0  ,   9 },
        { 2.2, 2.4, 0  ,   5 },
        { 2.2, 2.4, 6  ,  10 },
        { 3.4, 3.6, 0  ,   9 },
        { 4.6, 4.8, 1  ,  10 },
        { 5.8, 6  , 0  ,   6 },
        { 5.8, 6  , 7  ,  10 },
        { 7  , 7.2, 1  ,  10 },
        { 8.2, 8.4, 0  ,  8.5},
        { 8.4, 9.3, 8.3,  8.5},
        { 9.3, 10 , 7.1,  7.3},
        { 8.4, 9.3, 5.9,  6.1},
        { 9.3, 10 , 4.7,  4.9},
        { 8.4, 9.3, 3.5,  3.7},
        { 9.3, 10 , 2.3,  2.5}
    };
    /* avoid function returns 1 if x is in avoid set  */
    auto avoid = [&H,&ss_pre](const abs_type& idx) {
        state_type x;
        ss_pre.itox(idx,x);
        double c1= ss_pre.get_eta()[0]/2.0+1e-10;
        double c2= ss_pre.get_eta()[1]/2.0+1e-10;
        for(size_t i=0; i<15; i++) {
            if ((H[i][0]-c1) <= x[0] && x[0] <= (H[i][1]+c1) &&
                (H[i][2]-c2) <= x[1] && x[1] <= (H[i][3]+c2))
            return true;
        }
        return false;
    };
    /* compute BDD for the avoid set (returns the number of elements) */
    //BDD A = ss_pre.ap_to_bdd(manager,avoid);
    /* write ap to files avoid.scs/avoid.bdd */
    //write_to_file(manager,ss_pre,A,"obstacles");
    
    //set = SymbolicSet(SymbolicSet(ss_pre,ss_input),ss_post);
    
    cout << "Computing the transition function: " << endl;
    tt.tic();
    size_t no_trans;
    TF = sym_model.compute_gb(manager,vehicle_post,radius_post,avoid,no_trans);
    tt.toc();
    cout << "Number of transitions: " << no_trans << endl;
    if(!getrusage(RUSAGE_SELF, &usage)) {
      cout << "Memory per transition: " << usage.ru_maxrss/(double)no_trans << endl;
    }
    //write_to_file(manager,set,TF,"tf");
    //}
    
    /* define target set */
    auto target = [&ss_pre](const abs_type& idx) {
        state_type x;
        ss_pre.itox(idx,x);
        double r0 = ss_pre.get_eta()[0]/2.0;
        double r1 = ss_pre.get_eta()[1]/2.0;
        /* function returns 1 if cell associated with x is in target set  */
        if (9 <= (x[0]-r0) && (x[0]+r0) <= 9.5 &&
            0 <= (x[1]-r1) && (x[1]+r1) <= 0.5)
        return true;
        return false;
    };
  
    BDD target_states = ss_pre.ap_to_bdd(manager,target);
   
    //Check on the number of safe set points:
    cout << "The number of target states: "
    << ss_pre.get_size(manager, target_states)
    << endl << flush;
   
    //Read the unfit points to be expcluded from the safe set
    BDD safe_states = manager.bddOne();
    if(argc >= 3){
        //Read the unfit points if any
        const string unsafe_file_name(argv[2]);
        if(load_unsafe_states(manager, ss_pre,
                              safe_states, unsafe_file_name)){
            return 1;
        }
    }
    
    //The BDD to store the controller
    BDD controller = manager.bddZero();
   
    //Compute the controller for the reach and stay specification
    tt.tic();
    reach(manager, TF, sym_model, ss_pre, ss_input,
          safe_states, target_states, controller);
    tt.toc();
   
    //Make controller, check for safety and write into file
    const string file_name = string("vehicle") + suffix;
    return check_and_safe_controller(manager, ss_pre, ss_input,
                                     safe_states, controller, file_name);
}
