#!/bin/bash

BINARY_HOME="../../build/src/optdet"
MODELS_HOME="./models"
BINARY="${BINARY_HOME}/scots_opt_det"

function run_test() {
    echo "========================================================================="
    
    #Prepare varible values
    SCOTS_CTRL=${MODELS_HOME}/$1/scots/c_reo
    mkdir -p ${MODELS_HOME}/${1}/${3}/
    DET_CTRL=${MODELS_HOME}/${1}/${3}/c
    CMD="${BINARY} -s ${SCOTS_CTRL} -t ${DET_CTRL} -l info -d ${2} -a ${4} -r -e -c -g -x -n"
    echo "${CMD}"
    LOG_FILE_NAME="${DET_CTRL}.log"
    echo "Logging into: ${LOG_FILE_NAME}"
    
    #Remove the old bdd models only for the sake
    #of better visibility of software crashes
    rm -f ${DET_CTRL}*.bdd
    
    #Execute the program
    ${CMD} > ${LOG_FILE_NAME}
    
    #Get the data to show on the screen
    grep "Loaded controller BDD with" ${LOG_FILE_NAME}
    grep "Distinct input ids count" ${LOG_FILE_NAME}
    grep "Definite input ids count" ${LOG_FILE_NAME}
    grep "Found determinization" ${LOG_FILE_NAME}
    echo ""
    grep "BDD determinization took" ${LOG_FILE_NAME}
    grep " and storing the controller took" ${LOG_FILE_NAME}
    echo ""
    grep "Const mode switches" ${LOG_FILE_NAME}
    grep "Line mode switches" ${LOG_FILE_NAME}
    echo ""
    #grep "USAGE: The resulting " ${LOG_FILE_NAME}
    #echo ""
    ls -al ${SCOTS_CTRL}.bdd | awk '{print "Original\t"$9"\t, bytes: "$5}'
    ls -al ${DET_CTRL}.bdd | awk '{print "Determinized\t"$9"\t, bytes: "$5}'
    ls -al ${DET_CTRL}_*.bdd | awk '{print "Compressed\t"$9"\t, bytes: "$5}'
}

echo -e "\n*******************************\n"

run_test dcm/1 2 blga bdd-mixed
run_test dcm/5 2 blga bdd-mixed
run_test dcm/10 2 blga bdd-mixed
run_test dcm/15 2 blga bdd-mixed
run_test dcm/20 2 blga bdd-mixed
run_test dcm/25 2 blga bdd-mixed

run_test dcm/1 2 lga mixed
run_test dcm/5 2 lga mixed
run_test dcm/10 2 lga mixed
run_test dcm/15 2 lga mixed
run_test dcm/20 2 lga mixed
run_test dcm/25 2 lga mixed

run_test dcm/1 2 la local
run_test dcm/5 2 la local
run_test dcm/10 2 la local
run_test dcm/15 2 la local
run_test dcm/20 2 la local
run_test dcm/25 2 la local

run_test dcm/1 2 ga global
run_test dcm/5 2 ga global
run_test dcm/10 2 ga global
run_test dcm/15 2 ga global
run_test dcm/20 2 ga global
run_test dcm/25 2 ga global

echo -e "\n*******************************\n"

run_test dcdc/1 2 blga bdd-mixed
run_test dcdc/5 2 blga bdd-mixed
run_test dcdc/10 2 blga bdd-mixed
run_test dcdc/15 2 blga bdd-mixed
run_test dcdc/20 2 blga bdd-mixed
run_test dcdc/25 2 blga bdd-mixed
run_test dcdc/30 2 blga bdd-mixed
run_test dcdc/35 2 blga bdd-mixed

run_test dcdc/1 2 lga mixed
run_test dcdc/5 2 lga mixed
run_test dcdc/10 2 lga mixed
run_test dcdc/15 2 lga mixed
run_test dcdc/20 2 lga mixed
run_test dcdc/25 2 lga mixed
run_test dcdc/30 2 lga mixed
run_test dcdc/35 2 lga mixed

run_test dcdc/1 2 la local
run_test dcdc/5 2 la local
run_test dcdc/10 2 la local
run_test dcdc/15 2 la local
run_test dcdc/20 2 la local
run_test dcdc/25 2 la local
run_test dcdc/30 2 la local
run_test dcdc/35 2 la local

run_test dcdc/1 2 ga global
run_test dcdc/5 2 ga global
run_test dcdc/10 2 ga global
run_test dcdc/15 2 ga global
run_test dcdc/20 2 ga global
run_test dcdc/25 2 ga global
run_test dcdc/30 2 ga global
run_test dcdc/35 2 ga global

echo -e "\n*******************************\n"

run_test dcdc_rec/1/1 2 blga bdd-mixed
run_test dcdc_rec/1/5 2 blga bdd-mixed
run_test dcdc_rec/1/10 2 blga bdd-mixed
run_test dcdc_rec/1/15 2 blga bdd-mixed
run_test dcdc_rec/1/20 2 blga bdd-mixed
run_test dcdc_rec/1/25 2 blga bdd-mixed
run_test dcdc_rec/1/30 2 blga bdd-mixed
run_test dcdc_rec/1/35 2 blga bdd-mixed

run_test dcdc_rec/1/1 2 lga mixed
run_test dcdc_rec/1/5 2 lga mixed
run_test dcdc_rec/1/10 2 lga mixed
run_test dcdc_rec/1/15 2 lga mixed
run_test dcdc_rec/1/20 2 lga mixed
run_test dcdc_rec/1/25 2 lga mixed
run_test dcdc_rec/1/30 2 lga mixed
run_test dcdc_rec/1/35 2 lga mixed

run_test dcdc_rec/1/1 2 la local
run_test dcdc_rec/1/5 2 la local
run_test dcdc_rec/1/10 2 la local
run_test dcdc_rec/1/15 2 la local
run_test dcdc_rec/1/20 2 la local
run_test dcdc_rec/1/25 2 la local
run_test dcdc_rec/1/30 2 la local
run_test dcdc_rec/1/35 2 la local

run_test dcdc_rec/1/1 2 ga global
run_test dcdc_rec/1/5 2 ga global
run_test dcdc_rec/1/10 2 ga global
run_test dcdc_rec/1/15 2 ga global
run_test dcdc_rec/1/20 2 ga global
run_test dcdc_rec/1/25 2 ga global
run_test dcdc_rec/1/30 2 ga global
run_test dcdc_rec/1/35 2 ga global

echo -e "\n*******************************\n"

run_test dcdc_rec/2/1 2 blga bdd-mixed
run_test dcdc_rec/2/5 2 blga bdd-mixed
run_test dcdc_rec/2/10 2 blga bdd-mixed
run_test dcdc_rec/2/15 2 blga bdd-mixed
run_test dcdc_rec/2/20 2 blga bdd-mixed
run_test dcdc_rec/2/25 2 blga bdd-mixed
run_test dcdc_rec/2/30 2 blga bdd-mixed
run_test dcdc_rec/2/35 2 blga bdd-mixed

run_test dcdc_rec/2/1 2 lga mixed
run_test dcdc_rec/2/5 2 lga mixed
run_test dcdc_rec/2/10 2 lga mixed
run_test dcdc_rec/2/15 2 lga mixed
run_test dcdc_rec/2/20 2 lga mixed
run_test dcdc_rec/2/25 2 lga mixed
run_test dcdc_rec/2/30 2 lga mixed
run_test dcdc_rec/2/35 2 lga mixed

run_test dcdc_rec/2/1 2 la local
run_test dcdc_rec/2/5 2 la local
run_test dcdc_rec/2/10 2 la local
run_test dcdc_rec/2/15 2 la local
run_test dcdc_rec/2/20 2 la local
run_test dcdc_rec/2/25 2 la local
run_test dcdc_rec/2/30 2 la local
run_test dcdc_rec/2/35 2 la local

run_test dcdc_rec/2/1 2 ga global
run_test dcdc_rec/2/5 2 ga global
run_test dcdc_rec/2/10 2 ga global
run_test dcdc_rec/2/15 2 ga global
run_test dcdc_rec/2/20 2 ga global
run_test dcdc_rec/2/25 2 ga global
run_test dcdc_rec/2/30 2 ga global
run_test dcdc_rec/2/35 2 ga global

echo -e "\n*******************************\n"

run_test vehicle/1 3 blga bdd-mixed
run_test vehicle/2 3 blga bdd-mixed
run_test vehicle/3 3 blga bdd-mixed
run_test vehicle/4 3 blga bdd-mixed
run_test vehicle/5 3 blga bdd-mixed

run_test vehicle/1 3 lga mixed
run_test vehicle/2 3 lga mixed
run_test vehicle/3 3 lga mixed
run_test vehicle/4 3 lga mixed
run_test vehicle/5 3 lga mixed

run_test vehicle/1 3 la local
run_test vehicle/2 3 la local
run_test vehicle/3 3 la local
run_test vehicle/4 3 la local
run_test vehicle/5 3 la local

run_test vehicle/1 3 ga global
run_test vehicle/2 3 ga global
run_test vehicle/3 3 ga global
run_test vehicle/4 3 ga global
run_test vehicle/5 3 ga global

echo -e "\n*******************************\n"

run_test aircraft/1 3 blga bdd-mixed
run_test aircraft/2 3 blga bdd-mixed
run_test aircraft/3 3 blga bdd-mixed

run_test aircraft/1 3 lga mixed
run_test aircraft/2 3 lga mixed
run_test aircraft/3 3 lga mixed

run_test aircraft/1 3 la local
run_test aircraft/2 3 la local
run_test aircraft/3 3 la local

run_test aircraft/1 3 ga global
run_test aircraft/2 3 ga global
run_test aircraft/3 3 ga global

