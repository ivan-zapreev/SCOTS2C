/*
 * File:   scots2ext.cc
 * Author: Dr. Ivan S. Zapreev
 *
 * Visit my Linked-in profile:
 *      <https://nl.linkedin.com/in/zapreevis>
 * Visit my GitHub:
 *      <https://github.com/ivan-zapreev>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.#
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Created on February 15, 2018, 14:43 PM
 */


#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

/* SSTP header */
#include "wstp.h"

/* SCOTS header */
#include "scots.hh"

using namespace std;
using namespace scots;

/*Define the global variables*/
static Cudd * pCuddMgr = NULL;
static BDD * pBDD = NULL;
static SymbolicSet * pCtr = NULL;

/**
 * Allows to load the SCOTS v2.0 BDD controller
 * @param s the C-string name of the controller file without ".scs" extention
 * @result 0 if everything went fine, otherwise an error
 */
extern "C" int32_t load_controller_bdd(const char* s)
{
    //Get the file name as a string
    string file_name(s);
    
    std::cout << "Loading the BDD controller: " << file_name << std::endl;

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

    /* read controller from file */
    if(!read_from_file(*pCuddMgr, *pCtr, *pBDD, file_name.c_str())) {
        std::cerr << "Could not read controller from: " << file_name << std::endl;
        return 1;
    } else {
        std::cout << "The BDD controller is successfully loaded!" << std::endl;
        return 0;
    }
}

/**
 * Allows to get the number of dimensions of the controller.
 * This is the sum of the state space dimensions and input
 * signal dimensions.
 * @return 0 if the controller is not loaded, otherwise the number of dimensions
 */
extern "C" int32_t get_dim() {
    if(pCtr) {
        return pCtr->get_dim();
    } else {
        return 0;
    }
}

/**
 * Allows to get the list of discretization values per dimension
 * @return the list of discretization values
 */
extern "C" void get_eta() {
    if(pCtr) {
        //Get the eta vector and then return it to Mathematica
        auto data = pCtr->get_eta();
        if(!WSPutReal64List(stdlink, &data[0], pCtr->get_dim())) {
            //Unable to put the list to WSTP link
            std::cerr << "Unable to put the eta values list to WSTP link" << std::endl;
        }
    } else {
        std::cerr << "The BDD controller is not loaded" << std::endl;
    }
}

/**
 * Allows to get the position of the lower-left point of the grid
 * @return the lower-left point of the grid
 */
extern "C" void get_lower_left() {
    if(pCtr) {
        //Get the lower left vector and then return it to Mathematica
        auto data = pCtr->get_lower_left();
        if(!WSPutReal64List(stdlink, &data[0], pCtr->get_dim())) {
            //Unable to put the list to WSTP link
            std::cerr << "Unable to put the eta values list to WSTP link" << std::endl;
        }
    } else {
        std::cerr << "The BDD controller is not loaded" << std::endl;
    }
}

/**
 * Allows to get the position of the upper-right point of the grid
 * @return the upper-right point of the grid
 */
extern "C" void get_upper_right() {
    if(pCtr) {
        //Get the upper right vector and then return it to Mathematica
        auto data = pCtr->get_upper_right();
        if(!WSPutReal64List(stdlink, &data[0], pCtr->get_dim())) {
            //Unable to put the list to WSTP link
            std::cerr << "Unable to put the eta values list to WSTP link" << std::endl;
        }
    } else {
        std::cerr << "The BDD controller is not loaded" << std::endl;
    }
}

/**
 * Allows to get the available input signal values per state
 * @param point an array of point coordinates
 * @param len the number of dimensions of the point
 * @return the list of input signal values
 */

extern "C" void restriction(double * point, const long len) {
    if(pCtr) {
        //Initialize a vector to use in restriction
        vector<double> point_x;
        point_x.assign(point, point + len);
        
        std::cout << "Got the point, size: " << point_x.size() << std::endl;
        
        auto data = pCtr->restriction(*pCuddMgr, *pBDD, point_x);
        
        std::cout << "Got the control inputs, size: " << data.size() << std::endl;

        if(!WSPutReal64List(stdlink, &data[0], data.size())) {
            //Unable to put the list to WSTP link
            std::cerr << "Unable to put the eta values list to WSTP link" << std::endl;
        }
    } else {
        std::cerr << "The BDD controller is not loaded" << std::endl;
    }
}
