/*
 * aircraft.cc
 *
 * created: Jan 2017
 *  author: Matthias Rungger
 *
 */

/*
 * information about this example is given in
 * http://arxiv.org/abs/1503.03715
 * doi: 10.1109/TAC.2016.2593947
 * doi: 10.1109/CDC.2015.7403185
 *
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
const double tau = 0.25;

/* data types of the state space elements and input
 * space elements used in uniform grid and ode solver */
using state_type = array<double,state_dim>;
using input_type = array<double,input_dim>;

/* we integrate the aircraft ode by 0.25 sec (the result is stored in x)  */
auto aircraft_post = [] (state_type &x, const input_type &u) {
    /* the ode describing the aircraft */
    auto rhs =[] (state_type& xx,  const state_type &x, const input_type &u) {
        double mg = 60000.0*9.81;
        double mi = 1.0/60000;
        double c=(1.25+4.2*u[1]);
        xx[0] = mi*(u[0]*cos(u[1])-(2.7+3.08*c*c)*x[0]*x[0]-mg*sin(x[1]));
        xx[1] = (1.0/(60000*x[0]))*(u[0]*sin(u[1])+68.6*c*x[0]*x[0]-mg*cos(x[1]));
        xx[2] = x[0]*sin(x[1]);
    };
    /* use 10 intermediate steps */
    runge_kutta_fixed4(rhs,x,u,state_dim,tau,5);
};

/* we integrate the growth bound by 0.25 sec (the result is stored in r)  */
auto radius_post = [] (state_type &r, const state_type &, const input_type &u) {
    /* the ode for the growth bound */
    auto rhs =[] (state_type& rr,  const state_type &r, const input_type &u) {
        /* lipschitz matrix computed with mupad/mathematica check the ./helper directory */
        double L[3][2];
        L[0][0]=-0.00191867*(2.7+3.08*(1.25+4.2*u[1])*(1.25+4.2*u[1]));
        L[0][1]=9.81;
        L[1][0]=0.002933+0.004802*u[1];
        L[1][1]=0.003623;
        L[2][0]=0.07483;
        L[2][1]=83.22;
        /* to account for input disturbances */
        const state_type w={{.108,0.002,0}};
        rr[0] = L[0][0]*r[0]+L[0][1]*r[1]+w[0]; /* L[0][2]=0 */
        rr[1] = L[1][0]*r[0]+L[1][1]*r[1]+w[1]; /* L[1][2]=0 */
        rr[2] = L[2][0]*r[0]+L[2][1]*r[1]+w[2]; /* L[2][2]=0 */
    };
    /* use 10 intermediate steps */
    runge_kutta_fixed4(rhs,r,u,state_dim,tau,5);
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
    
    /* construct grid for the state space */
    /* setup the workspace of the synthesis problem and the uniform grid */
    /* grid node distance diameter */
    /* optimized values computed according to doi: 10.1109/CDC.2015.7403185 */
    state_type s_eta={{25.0/362 / divider,3*M_PI/180/66 / divider,56.0/334 / divider}};
    /* lower bounds of the hyper rectangle */
    state_type s_lb={{58,-3*M_PI/180,0}};
    /* upper bounds of the hyper rectangle */
    state_type s_ub={{83,0,56}};
    /* construct SymbolicSet with the UniformGrid information for the state space
     * and BDD variable IDs for the pre */
    SymbolicSet ss_pre(manager, state_dim,s_lb,s_ub,s_eta);
    /* construct SymbolicSet with the UniformGrid information for the state space
     * and BDD variable IDs for the post */
    SymbolicSet ss_post(manager, state_dim,s_lb,s_ub,s_eta);
    cout << "Unfiorm grid details:" << endl;
    ss_pre.print_info();
    
    /* construct grid for the input space */
    /* lower bounds of the hyper rectangle */
    input_type i_lb={{0,0}};
    /* upper bounds of the hyper rectangle */
    input_type i_ub={{32000,8*M_PI/180}};
    /* grid node distance diameter */
    input_type i_mu={{32000.0, (8.0/9.0*M_PI/180)}}; // Original value {{32000,8.0/9.0*M_PI/180}}
    SymbolicSet ss_input(manager,input_dim,i_lb,i_ub,i_mu);
    ss_input.print_info();
    
    /* transition function of symbolic model */
    BDD TF;
    
    /* setup object to compute the transition function */
    SymbolicModel<state_type,input_type> sym_model(ss_pre,ss_input,ss_post);
    /* measurement disturbances  */
    state_type z={{0.0125,0.0025/180*M_PI,0.05}};
    sym_model.set_measurement_error_bound(z);
    
    cout << "Computing the transition function: " << endl;
    tt.tic();
    size_t no_trans;
    TF = sym_model.compute_gb(manager,aircraft_post,radius_post,no_trans);
    tt.toc();
    cout << "Number of transitions: " << no_trans << endl;
    if(!getrusage(RUSAGE_SELF, &usage)) {
        cout << "Memory per transition: " << usage.ru_maxrss/(double)no_trans << endl;
    }
    
    /* define target set */
    auto target = [&s_eta, &z, &ss_pre](const abs_type& abs_state) {
        state_type t_lb = {{63,-3*M_PI/180,0}};
        state_type t_ub = {{75,0,2.5}};
        state_type c_lb;
        state_type c_ub;
        /* center of cell associated with abs_state is stored in x */
        state_type x;
        ss_pre.itox(abs_state,x);
        /* hyper-interval of the quantizer symbol with perturbation */
        for(int i=0; i<state_dim; i++) {
            c_lb[i] = x[i]-s_eta[i]/2.0-z[i];
            c_ub[i] = x[i]+s_eta[i]/2.0+z[i];
        }
        if( t_lb[0]<=c_lb[0] && c_ub[0]<=t_ub[0] &&
           t_lb[1]<=c_lb[1] && c_ub[1]<=t_ub[1] &&
           t_lb[2]<=c_lb[2] && c_ub[2]<=t_ub[2]) {
            if(-0.91<=(x[0]*sin(x[1])-s_eta[0]/2.0-z[0]-(c_ub[0])*(s_eta[1]/2.0-z[1]))) {
                return true;
            }
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
    const string file_name = string("aircraft") + suffix;
    return check_and_safe_controller(manager, ss_pre, ss_input,
                                     safe_states, controller, file_name);
}
