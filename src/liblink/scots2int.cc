#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

/* LibraryLink main header */
#include "WolframLibrary.h"

/* SCOTS header */
#include "scots.hh"

#include "input_output.hh"

using namespace std;
using namespace scots;
using namespace tud::ctrl::scots::optimal;

/*Define the global variables*/
static Cudd * pCuddMgr = NULL; //Stores the CUDD manager
static BDD * pBDD = NULL; //Stores the BDD of the controller
static SymbolicSet * pCtr = NULL; //Stores the Symbolic set of the controller

/*Data type for the state space and input values*/
using data_type = std::vector<double>;

/**
 * Allows to copy the data from a vector to the tensor
 * @param libData the library data object
 * @param data a vector of doubles/reals
 * @param Res the reference of the return argument to set the tesor into
 * @return error code
 */
static inline int set_result_tensor(WolframLibraryData & libData, data_type & data, MArgument & Res) {
    //The error code
    int err = LIBRARY_NO_ERROR;
    
    //Get the size for the tensor to be used
    const mint size = (mint) data.size();
    
    //Obtain the co-domain tenzor from the parameters
    MTensor tens;
    err = libData->MTensor_new(MType_Real, 1, &size, &tens);
    
    //Check if the tensor is created
    if(err == LIBRARY_NO_ERROR) {
        //Coy the elements one by one
        for(mint i = 1; (i <= size) && (err == LIBRARY_NO_ERROR); i++){
            err = libData->MTensor_setReal(tens, &i, data[i-1]);
        }
        
        //Set the result
        MArgument_setMTensor(Res, tens);
    } else {
        //Failed to allocate tensor
        libData->Message("tensor_alloc");
    }
    
    return err;
}

/**
 * Returns the library version
 */
DLLEXPORT mint WolframLibrary_getVersion( ) {
    return WolframLibraryVersion;
}

/**
 * Performs required library initialization
 */
DLLEXPORT int WolframLibrary_initialize(WolframLibraryData /* unused */) {
    return LIBRARY_NO_ERROR;
}

/**
 * Allows to load the SCOTS v2.0 BDD controller
 * @param file_name the C-string name of the controller file without ".scs" extention
 * @result 0 if everything went fine, otherwise an error
 */
EXTERN_C DLLEXPORT int load_controller_bdd(WolframLibraryData libData, mint Argc,
                                           MArgument *Args, MArgument Res)
{
    //The error code
    int err = LIBRARY_NO_ERROR;
    
    if( Argc == 1 ) {
        //Get the string from the arguments, there shall be just one
        char* s = MArgument_getUTF8String(Args[0]);
    
        //Get the file name as a string
        string file_name(s);
    
        /*Re-allocate the data structures*/
        if(pCtr) {
            delete pCtr;
        }
        pCtr = new SymbolicSet();
        if(pBDD) {
            delete pBDD;
        }
        pBDD = new BDD();
        if(pCuddMgr) {
            delete pCuddMgr;
        }
        pCuddMgr = new Cudd();
        //Disable automatic variable ordering
        pCuddMgr->AutodynDisable();

        //Releaser the string memory
        libData->UTF8String_disown(s);
    
        /* read controller from file */
        if(!read_from_file(*pCuddMgr, *pCtr, *pBDD, file_name.c_str())) {
            libData->Message("file_read_error");
            err = LIBRARY_FUNCTION_ERROR;
        } else {
            //Get the number of BDD nodes and set as a result
            MArgument_setInteger(Res, pBDD->nodeCount());
        }
    } else {
        libData->Message("inc_num_arg");
        err = LIBRARY_FUNCTION_ERROR;
    }
    
    //Return the result code
    return err;
}

/**
 * Allows to get the number of dimensions of the controller.
 * This is the sum of the state space dimensions and input
 * signal dimensions.
 * @param Res will store the integer value that is the length of the domain + co-domain
 * @return 0 if the controller is not loaded, otherwise the number of dimensions
 */
EXTERN_C DLLEXPORT int get_dim(WolframLibraryData libData, mint /* unused */,
                               MArgument * /* unused */, MArgument Res) {
    //The error code
    int err = LIBRARY_NO_ERROR;
    
    if(pCtr) {
        //Get the dimensions and set as a result
        MArgument_setInteger(Res, pCtr->get_dim());
    } else {
        //The controller is not loaded yet
        libData->Message("missing_ctrl");
        err = LIBRARY_FUNCTION_ERROR;
    }
    
    //Return the result code
    return err;
}

/**
 * Allows to get the list of discretization values per dimension
 * @return the list of discretization parameters
 */
EXTERN_C DLLEXPORT int get_eta(WolframLibraryData libData, mint /* unused */,
                               MArgument * /* unused */, MArgument Res) {
    //The error code
    int err = LIBRARY_NO_ERROR;
    
    if(pCtr) {
        //Get the eta vector and then return it to Mathematica
        data_type data = pCtr->get_eta();
        
        //Copy the data to the tensor and set as the result
        err = set_result_tensor(libData, data, Res);
    } else {
        //The controller is not loaded yet
        libData->Message("missing_ctrl");
        err = LIBRARY_FUNCTION_ERROR;
    }
    
    //Return the result code
    return err;
}

/**
 * Allows to get the position of the lower-left point of the grid
 * @param tens a rank one tensor that will store the resulting point
 *             of the length of domain+co-domain.
 * @return the lower-left point of the grid
 */
EXTERN_C DLLEXPORT int get_lower_left(WolframLibraryData libData, mint /* unused */,
                                      MArgument */* unused */, MArgument Res) {
    //The error code
    int err = LIBRARY_NO_ERROR;
    
    if(pCtr) {
        //Get the lower left vector and then return it to Mathematica
        data_type data = pCtr->get_lower_left();
        
        //Copy the data to the tensor and set as the result
        err = set_result_tensor(libData, data, Res);
    } else {
        //The controller is not loaded yet
        libData->Message("missing_ctrl");
        err = LIBRARY_FUNCTION_ERROR;
    }
    
    //Return the result code
    return err;
}

/**
 * Allows to get the position of the upper-right point of the grid
 * @param tens a rank one tensor that will store the resulting point
 *             of the length of domain+co-domain.
 * @return the upper-right point of the grid
 */
EXTERN_C DLLEXPORT int get_upper_right(WolframLibraryData libData, mint /* unused */,
                                       MArgument */* unused */, MArgument Res) {
    //The error code
    int err = LIBRARY_NO_ERROR;
    
    if(pCtr) {
        //Get the upper right vector and then return it to Mathematica
        data_type data = pCtr->get_upper_right();
        
        //Copy the data to the tensor and set as the result
        err = set_result_tensor(libData, data, Res);
    } else {
        //The controller is not loaded yet
        libData->Message("missing_ctrl");
        err = LIBRARY_FUNCTION_ERROR;
    }
    
    //Return the result code
    return err;
}

/** Allows to get a vector of grid points that are encoded in the BDD.
 *  The return vector is of size (number of grid points) x n where n is the dimension.
 *  The grid points are stacked on top of each other, i.e., the first n
 *  entries of the return vector represent the first grid point.
 *  @return the list of grid points
 **/
EXTERN_C DLLEXPORT int get_grid_points(WolframLibraryData libData, mint /* unused */,
                                       MArgument * /* unused */, MArgument Res) {
    //The error code
    int err = LIBRARY_NO_ERROR;
    
    if(pCtr) {
        //Obtaian the grid points data
        data_type data = pCtr->bdd_to_grid_points(*pCuddMgr, *pBDD);
        
        //Copy the data to the tensor and set as the result
        err = set_result_tensor(libData, data, Res);
    } else {
        //The controller is not loaded yet
        libData->Message("missing_ctrl");
        err = LIBRARY_FUNCTION_ERROR;
    }
    
    //Return the result code
    return err;
}

/**
 * Allows to get the available input signal values per state
 * NOTE-1: Must get the one perameter as specified, the latter
 * is not checked in the code for the sped purposes!
 * NOTE-2: Must have the controller loaded, the latter
 * is not checked in the code for the sped purposes!
 * @param dom_point a rank one tensor
 * @return the list of input signal values
 */

EXTERN_C DLLEXPORT int restriction(WolframLibraryData libData, mint /* unused */,
                                   MArgument *Args, MArgument Res) {
    //The error code
    int err = LIBRARY_NO_ERROR;
    
    //Obtain the domain tensor from the parameters
    MTensor tens_dom = MArgument_getMTensor(Args[0]);
    
    //Get the data and the data length
    double * point = libData->MTensor_getRealData(tens_dom);
    int len = libData->MTensor_getFlattenedLength(tens_dom);
    
    //Initialize a vector to use in restriction
    data_type point_x;
    point_x.assign(point, point + len);
    
    //Get the restriction
    data_type data = pCtr->restriction(*pCuddMgr, *pBDD, point_x);

    //Copy the data to the tensor and set as the result
    err = set_result_tensor(libData, data, Res);
    
    //Return the result code
    return err;
}


/**
 * Allows to store the controller in a bdd form with the inputs being the control functional ids.
 * Note that, the functionals corresponding to the controller ids are to be stored separately.
 * @param num_ctl the number of fontrol modes (controller ids) used in the controller.
 * @param data the tensor rank two storing the controller points in a form
 *             {{state_point, controller_id}, ... {state_point, controller_id}}.
 * @param file_name the C-string name of the controller file without any extention
 * @param optimize true if the variable ordering is to be optimized, otherwise false.
 *                 In the latter case we use the one of the original controller for
 *                 the state space and natural for the input space.
 * @result 0 if everything went fine, otherwise an error
 */
EXTERN_C DLLEXPORT int store_controller_bdd(WolframLibraryData libData, mint Argc,
                                           MArgument *Args, MArgument /* unused */) {
    //The error code
    int err = LIBRARY_NO_ERROR;
    
    if( Argc == 4 ) {
        //Get the number of controllers
        const mint num_ctl = MArgument_getInteger(Args[0]);
        
        //Get the tensor data storing the controllers
        MTensor data = MArgument_getMTensor(Args[1]);
        const mint rank = libData->MTensor_getRank(data);

        //Get the string storing the controller name
        char* file_name = MArgument_getUTF8String(Args[2]);
        
        //Make the log file
        string log_file_name(file_name);
        log_file_name += ".log";
        ofstream logfile;
        logfile.open(log_file_name);
        logfile << "# control signals: " << num_ctl
            << ", tensor rank: " << rank << "\n" << std::flush;
        
        //The rank of the data is to be two
        if(rank == 2) {
            //Get the number of state dimensions
            mint const* dims = libData->MTensor_getDimensions(data);
            //The number of state-space dimension is the second tenzor
            //dimension size minus one, minus one is due to that the
            //controller id is stored as a last element of the sub-array
            const mint state_dim = dims[1] - 1;
            
            logfile << "tensor dimensions dims[0]: " << dims[0]
                << ", dims[1]: " << dims[1] << "\n" << std::flush;
            
            //The state space dimensionality is to be positive
            if(state_dim > 0) {
                //Check if the scots controller is loaded
                if(pCtr) {
                    //Check if one needs optimization of variable orders
                    const mbool is_opt = MArgument_getBoolean(Args[3]);
                    //Create symbolic set for the state space using new BDD varibales
                    //scots::SymbolicSet ss_state(*pCuddMgr, state_dim,
                    //                            pCtr->get_lower_left(),
                    //                            pCtr->get_upper_right(),
                    //                            pCtr->get_eta());
                    
                    //Create symbolic set for the state space re-using exisintg BDD varibales
                    UniformGrid ss_grid(state_dim,
                                        pCtr->get_lower_left(),
                                        pCtr->get_upper_right(),
                                        pCtr->get_eta());
                    std::vector<IntegerInterval<abs_type>> ints = pCtr->get_bdd_intervals();
                    auto first = ints.begin();
                    auto last = first + state_dim;
                    std::vector<IntegerInterval<abs_type>> sub_ints(first, last);
                    scots::SymbolicSet ss_state(ss_grid, sub_ints);
                    
                    //Create symbolic set for the input signals
                    data_type ill = {1}, iur = {(double)num_ctl}, ieta = {1.0};
                    scots::SymbolicSet ss_input(*pCuddMgr, 1, ill, iur, ieta);
                    
                    //Create the common symbolic set for the state and inpout spaces
                    scots::SymbolicSet ctrl_info(ss_state, ss_input);
                    
                    //Initialize the controller BDD
                    BDD ctrl_bdd = pCuddMgr->bddZero();
                    
                    //Create the controller bdd from data
                    double * points = libData->MTensor_getRealData(data);
                    mint base_idx = 0;
                    for(uint i = 0; i < dims[0]; ++i)
                    {
                        //logfile << "Processing element " << i << "/" << dims[0] << "\n" << std::flush;
                        
                        //Compute the input index
                        mint indim_idx = base_idx + state_dim;
                        
                        //Get the state point piece
                        data_type point_s;
                        point_s.assign(&points[base_idx], &points[indim_idx]);
                        //Get the state point bdd
                        BDD s_bdd = ss_state.id_to_bdd(ss_state.xtoi(point_s));

                        //Get the input point piece
                        data_type point_i = {points[indim_idx]};
                        //Get the input point bdd
                        BDD i_bdd = ss_input.id_to_bdd(ss_input.xtoi(point_i));
                        
                        //Add the state/input pair to the controller
                        ctrl_bdd |= s_bdd & i_bdd;
                        
                        //Update the base index move to the next sub-array
                        base_idx += dims[1];
                    }
                    
                    logfile << "The created controller size : "
                        << ctrl_bdd.nodeCount() << "\n" << std::flush;
                    
                    //Check if variable reordering is needed
                    try{
                        if(is_opt) {
                            //Perform optimization and store the minimized bdd
                            store_min_controller(*pCuddMgr, ctrl_info, ctrl_bdd,
                                                 file_name, store_type_enum::reorder);
                        } else {
                            //Store the original controller
                            store_controller(*pCuddMgr, ctrl_info, ctrl_bdd, file_name);
                        }
                    }catch(...){
                        //The controller could not be written
                        libData->Message("failed_to_write");
                        err = LIBRARY_FUNCTION_ERROR;
                    }
                    
                    logfile << "Work is finished!\n" << std::flush;
                } else {
                    //The controller is not loaded yet
                    libData->Message("missing_ctrl");
                    err = LIBRARY_FUNCTION_ERROR;
                }
            } else {
                libData->Message("inc_state_size");
                err = LIBRARY_DIMENSION_ERROR;
            }
        } else {
            libData->Message("inc_data_rank");
            err = LIBRARY_RANK_ERROR;
        }
        
        //Close the log file
        logfile.close();
    } else {
        libData->Message("inc_num_arg");
        err = LIBRARY_FUNCTION_ERROR;
    }
    
    //Return the result code
    return err;
}
