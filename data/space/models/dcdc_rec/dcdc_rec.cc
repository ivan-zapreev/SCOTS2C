#include <iostream>
#include <array>
#include <cmath>
#include <cstdlib>
#include <string>
#include <algorithm>

/* CUDD header */
#include "cudd.h"
#include "cuddObj.hh"

/* SCOTS_BDD define */
#ifndef SCOTS_BDD
#define SCOTS_BDD
#endif

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

/***********************************************
 *
 * define state space and dynamic
 *
 * ********************************************/

/* state space dim */
const int state_dim=2;
/* input space dim */
const int input_dim=1;
/* sampling time */
const double tau = 0.5;

/*
 * data types for the elements of the state space
 * and input space used by the ODE solver
 */
using state_type = array<double,state_dim>;
using input_type = array<double,input_dim>;

/* parameters for system dynamics */
const double xc=70;
const double xl=3;
const double rc=0.005;
const double rl=0.05;
const double ro=1;
const double vs=1;

/* we integrate the dcdc ode by 0.5 sec (the result is stored in x)  */
auto system_post = [](state_type &x, const input_type &u) noexcept {
    /* the ode describing the dcdc converter */
    auto rhs =[](state_type& xx,  const state_type &x, const input_type &u) noexcept {
        if(u[0]==1) {
            xx[0]=-rl/xl*x[0]+vs/xl;
            xx[1]=-1/(xc*(ro+rc))*x[1];
        } else {
            xx[0]=-(1/xl)*(rl+ro*rc/(ro+rc))*x[0]-(1/xl)*ro/(5*(ro+rc))*x[1]+vs/xl;
            xx[1]=(1/xc)*5*ro/(ro+rc)*x[0]-(1/xc)*(1/(ro+rc))*x[1];
        }
    };
    runge_kutta_fixed4(rhs,x,u,state_dim,tau,5);
};
/* we integrate the growth bound by 0.5 sec (the result is stored in r)  */
auto radius_post = [](state_type &r, const state_type&, const input_type &u) noexcept {
    /* the ode for the growth bound */
    auto rhs =[](state_type& rr,  const state_type &r, const input_type &u) noexcept {
        if(u[0]==1) {
            rr[0]=-rl/xl*r[0];
            rr[1]=-1/(xc*(ro+rc))*r[1];
        } else {
            rr[0]=-(1/xl)*(rl+ro*rc/(ro+rc))*r[0]+(1/xl)*ro/(5*(ro+rc))*r[1];
            rr[1]=5*(1/xc)*ro/(ro+rc)*r[0]-(1/xc)*(1/(ro+rc))*r[1];
        }
    };
    runge_kutta_fixed4(rhs,r,u,state_dim,tau,5);
};

int main(int argc, char** argv)
{

    cout << "dcdc example for the use of scots with bdd: " << endl
              << "to solve a recurrence problem for the dcdc converter, " << endl
              << "we we implement the nested fixed point algorithm: " << endl
              << "nu Z.intersection_{i=1-N}(mu Y[i].((T[i] & pre(Z)) | pre(Y[i]))" << endl;

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

     /***********************************************
     *
     * setup of the uniform grids
     *
     * ********************************************/

    /* construct grid for the state alphabet */
    /* grid node distance diameter */
    state_type s_eta={{20.0/4e3 / divider,20.0/4e3 / divider}};
    /* lower bounds of the hyper-rectangle */
    state_type lb={{0.649,4.949}};
    /* upper bounds of the hyper-rectangle */
    state_type ub={{1.65,5.95}};
    SymbolicSet ss_pre(manager,state_dim,lb,ub,s_eta);
    SymbolicSet ss_post(manager,state_dim,lb,ub,s_eta);
    cout << endl << "state alphabet details:" << endl;
    ss_pre.print_info();

    /* construct grid for the input alphabet */
    /* hyper-rectangle [1,2] with grid node distance 1 */
    auto i_eta = input_type{{1.0}}; //Original value 1
    SymbolicSet ss_input(manager,input_dim,input_type{{.99}},input_type{{2.1}},i_eta);
    cout << endl <<"input alphabet details:" << endl;
    ss_input.print_info();

    /***********************************************
     *
     * Definition of the Targets:
     * Targets are represented as rectangles in the
     * state space and defined by lambda functions
     *
     * we create N=2 targets
     *
     * ********************************************/

    /* Vectors for the Targets and their individual controller */
    vector<BDD> targets;
    vector<BDD> safe_states;
    vector<BDD> controller;

    /* lambda for target 1*/
    auto target_1 = [&ss_pre,&s_eta](const abs_type& idx) {
        /* bounding rectangle for T_1*/
        double h[4] = {1.3,1.6,5.7,5.9};
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
    /* lambda for target 2*/
    auto target_2 = [&ss_pre,&s_eta](const abs_type& idx) {
        /* bounding rectangle for T_2*/
        double h[4] = {0.8,1.1,5.0,5.2};
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
    /* fill the target set with T_1 - T_N */
    targets.push_back(ss_pre.ap_to_bdd(manager,target_1));
    targets.push_back(ss_pre.ap_to_bdd(manager,target_2));
  
    /*Read the safe sets which will be based on unsafe/unfit states*/
    BDD safe_states_1 = manager.bddOne();
    BDD safe_states_2 = manager.bddOne();
    if(argc >= 4) {
        //Read the unfit points if any
        const string unsafe_file_name_1(argv[2]);
        const string unsafe_file_name_2(argv[3]);
        if(load_unsafe_states(manager, ss_pre, safe_states_1, unsafe_file_name_1)
           || load_unsafe_states(manager, ss_pre, safe_states_2, unsafe_file_name_2)){
            return 1;
        }
    }
    safe_states.push_back(safe_states_1);
    safe_states.push_back(safe_states_2);

    /* initiate N=2 controller as empty */
    controller.push_back(manager.bddZero());
    controller.push_back(manager.bddZero());

    cout << endl << "Targets:" << endl
         << "Target1: [1.3,1.6];[5.7,5.9]" << endl
         << "Target2: [0.8,1.1];[5.0,5.2]" << endl;

    /***********************************************
     *
     * Compute transition function of symbolic model
     *
     * ********************************************/
    cout << "Computing the transition function:" << endl;

    tt.tic();
    /* SymbolicModel class to compute the BDD encoding the transition function */
    SymbolicModel<state_type,input_type> sym_model(ss_pre,ss_input,ss_post);
    /* flag for the number of transitions*/
    size_t no_trans;
    /* computing the growth bound*/
    BDD TF = sym_model.compute_gb(manager,system_post,radius_post,no_trans);
    tt.toc();

    cout << "Number of transitions " << no_trans << endl;
    if(!getrusage(RUSAGE_SELF, &usage)) {
        cout << "Memory per transition: " << usage.ru_maxrss/(double)no_trans << endl;
    }

    manager.DebugCheck();

    /***********************************************
     *
     * Synthesis
     *
     * ********************************************/
    cout << "Synthesizing the controllers:" << endl;
  
  
    //take into account the safe states, we use
    //backwards reachability so excluding them
    //from the source states of the transition
    //relation and the goal states shall be
    //enough to ensure Globally<sate_states>
    for(int i = 0; i < 2; i++) {
        targets[i] &= safe_states[i];
        TF &= safe_states[i];
    }

    /* class for extracting the pre lists*/
    EnfPre enf_pre(manager, TF, sym_model);

    /* outer fp*/
    BDD Z   = manager.bddZero();
    BDD ZZ  = manager.bddOne();

    /* inner fps*/
    vector<BDD> Y;
    vector<BDD> YY;
    for(int i = 0; i < 2; i++) {
        Y.push_back(manager.bddOne());
        YY.push_back(manager.bddZero());
    }

    /* helper */
    BDD U=ss_input.get_cube(manager);

    /***********************************************
     *
     * Fixed Point Computation:
     *
     * nu Z.intersection_{i=1-N}(mu Y[i].((T[i] & pre(Z)) | pre(Y[i]))
     *
     * ********************************************/
    tt.tic();
    while(Z != ZZ) {
        Z = ZZ;
        BDD preZ = enf_pre(Z);
        ZZ = manager.bddOne();
        for(int i = 0; i < 2; i++) {
            YY[i] = manager.bddZero();
            controller[i] = manager.bddZero();
            while(Y[i] != YY[i]) {
                Y[i]   = YY[i];
                YY[i]  = (targets[i] & preZ) | enf_pre(Y[i]);

                BDD N = YY[i] & (!(controller[i].ExistAbstract(U)));
                controller[i] = controller[i] | N;
            }
            ZZ = ZZ & YY[i];
        }
        //Eventually ZZ = YY[0] & YY[1]
    }
    tt.toc();

    /*checking on the sizes of the winning domains*/
    for(int i = 0; i < 2; i++) {
        cout << "Winning domain size of Target" + to_string(i+1) + " : "
                  << ss_pre.get_size(manager,controller[i]) << endl;
    }

    /***********************************************
     *
     * Saving of the controller
     *
     * ********************************************/
     cout << "Storing the controllers:\n";

    SymbolicSet symbSet(ss_pre,ss_input);
    for(int i = 0; i < 2; i++) {
        cout << "Controller size before re-ordering: " << controller[i].nodeCount() << endl;
        
        string file_name = string("dcdc_rec_") + to_string(i+1) + suffix;
        
        cout << "write " << file_name << ".scs to file : ";
        if(!write_to_file(manager,symbSet,controller[i], file_name.c_str())) {
            cout << "failed." << endl;
        } else {
            cout << "done." << endl;
        }
    }

    return 0;
}


