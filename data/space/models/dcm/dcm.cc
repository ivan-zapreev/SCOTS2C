/*
 * dc.cc
 *
 *  created: Sep 2017
 *   author: Ivan Zapreev
 */

/* information about this example is given in the readme file */

#include <iostream>
#include <array>
#include <cmath>
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
const int state_dim=2;
/* input space dim */
const int input_dim=1;
/* sampling time */
const double tau = 0.05;

/*
 * data types for the elements of the state space
 * and input space used by the ODE solver
 */
using state_type = array<double,state_dim>;
using input_type = array<double,input_dim>;

/* abbrev of the type for abstract states and inputs */
using abs_type = abs_type;

/* parameters for system dynamics */
const double R = 500.0e-3;
const double L = 1500.0e-6;
const double J = 250.0e-6;
const double B = 100.0e-6;
const double k = 50.0e-3;
/* parameters for radius calculation */
const double mu=sqrt(2);
/* we integrate the dc ode by 0.05 sec (the result is stored in x)  */
auto system_post = [](state_type &x, const input_type &u) noexcept {
    /* the ode describing the dc converter */
    auto rhs =[](state_type& xx,  const state_type &x, const input_type &u) noexcept {
        xx[0]= -B/J * x[0] + k/J * x[1];
        xx[1]= -k/L * x[0] - R/L * x[1] + 1/L * u[0];
    };
    runge_kutta_fixed4(rhs,x,u,state_dim,tau,10);
};
/* we integrate the growth bound by 0.05 sec (the result is stored in r)  */
auto radius_post = [](state_type &r, const state_type&, const input_type & u) noexcept {
    /* the ode for the growth bound */
    auto rhs =[](state_type& rr,  const state_type &r, const input_type &/*unused u*/) noexcept {
        rr[0]= -B/J * r[0] + k/J * r[1];
        rr[1]= +k/L * r[0] - R/L * r[1];
    };
    runge_kutta_fixed4(rhs,r,u,state_dim,tau,10);
};

int main(int argc, char** argv) {
    cout << "Starting the DC:\n";
    
    /* to measure time */
    TicToc tt;
    /* BDD manager */
    Cudd manager;
    
    /*Disable variable reordering */
    manager.AutodynEnable();
    
    /*Get the eta divider*/
    uint32_t divider = 1;
    string suffix = "";
    if(argc >= 2){
        divider = max(1, abs(atoi(argv[1])));
    }
    suffix = "_";
    suffix += to_string(divider);
    
    cout << "---------------------\n";
    
    /* setup the workspace of the synthesis problem and the uniform grid */
    /* grid node distance diameter */
    state_type s_eta={{0.5/divider, 0.5/divider}};
    /* lower bounds of the hyper-rectangle */
    state_type lb={{-1.0,-10.0}};
    /* upper bounds of the hyper-rectangle */
    state_type ub={{30.0,10.0}};
    SymbolicSet ss_pre(manager,state_dim,lb,ub,s_eta);
    SymbolicSet ss_post(manager,state_dim,lb,ub,s_eta);
    cout << "Unform grid details:\n";
    ss_pre.print_info();
    
    /* construct grid for the input alphabet */
    auto i_mu = input_type{{0.01}}; //Original value 0.01
    SymbolicSet ss_input(manager,input_dim,input_type{{-10.0}},input_type{{10.0}},i_mu);
    ss_input.print_info();
    
    /* compute transition function of symbolic model */
    cout << "Computing the transition function:\n";
    /* SymbolicModel class to compute the BDD encoding the transition function */
    SymbolicModel<state_type,input_type> sym_model(ss_pre,ss_input,ss_post);
    
    tt.tic();
    size_t no_trans;
    BDD TF = sym_model.compute_gb(manager,system_post,radius_post,no_trans);
    tt.toc();
    
    cout << "No of Transitions " << no_trans  << "\n";
    if(!getrusage(RUSAGE_SELF, &usage)) {
        cout << "Memory pro Transition: " << usage.ru_maxrss/(double)no_trans<< "\n";
    }
    
    manager.DebugCheck();
    /* we continue with the controller synthesis for FG (target) */
    cout << "Synthesis: ";
    /* inner approximation of target set */
    auto target = [&ss_pre,&s_eta](const abs_type& idx) {
        double h[4] = {10.5,20.5,-10.0, 10.0};
        state_type x;
        ss_pre.itox(idx,x);
        double c1= s_eta[0]/2.0+1e-10;
        double c2= s_eta[1]/2.0+1e-10;
        if ((h[0]+c1) <= x[0] && x[0] <= (h[1]-c1) &&
            (h[2]+c2) <= x[1] && x[1] <= (h[3]-c2)) {
            return true;
        }
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
    const string file_name = string("dcm") + suffix;
    return check_and_safe_controller(manager, ss_pre, ss_input,
                                     safe_states, controller, file_name);
}

