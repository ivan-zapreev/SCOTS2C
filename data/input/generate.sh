#!/bin/bash

WORK_DIR="$PWD"
echo "Being run in: ${WORK_DIR}"

function move_ctrl_if_present() {
    GEN_FILE_NAME=${1}
    SCOTS_CTRL_DIR=${2}
    if [ -f ${GEN_FILE_NAME}.bdd ]; then
        #Delete the old files, if any
        rm -rf ${SCOTS_CTRL_DIR}/c_reo.scs
        rm -rf ${SCOTS_CTRL_DIR}/c_reo.bdd

        #Move the generated controllers to the appropriate locations and rename
        mkdir -p ${SCOTS_CTRL_DIR}

        #Move the generated controllers to the directory
        mv ${GEN_FILE_NAME}.scs ${SCOTS_CTRL_DIR}/c_reo.scs
        mv ${GEN_FILE_NAME}.bdd ${SCOTS_CTRL_DIR}/c_reo.bdd
    fi
}

function run() {
    for i in $(seq 0 $4 $3); do
        #Work around the zero case
        if [ "${i}" == "0" ]; then
            i=1
        fi

        echo "================================="
        echo "> Generating controller ${1}/${2}_${i}"
        echo "================================="

        #Move to the directory
        cd ${1}

        #Run the generation of the controller(s)
        ./${2} ${i}
        
        #A trivial work-around for the dcdc_rec case study
        move_ctrl_if_present "${2}_${i}" "./${i}/scots"
        move_ctrl_if_present "${2}_1_${i}" "./1/${i}/scots"
        move_ctrl_if_present "${2}_2_${i}" "./2/${i}/scots"
        
        cd ${WORK_DIR}
    done
}

run ./models/dcm dcm 25 5
run ./models/dcdc dcdc 200 40
run ./models/dcdc_rec dcdc_rec 200 40
run ./models/vehicle vehicle 5 1
run ./models/aircraft aircraft 3 1
