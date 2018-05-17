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

run_test dcm/1 2 blgdet bdd-mixed
run_test dcm/5 2 blgdet bdd-mixed
run_test dcm/10 2 blgdet bdd-mixed
run_test dcm/15 2 blgdet bdd-mixed
run_test dcm/20 2 blgdet bdd-mixed
run_test dcm/25 2 blgdet bdd-mixed

echo -e "\n*******************************\n"

run_test dcdc/1 2 blgdet bdd-mixed
run_test dcdc/40 2 blgdet bdd-mixed
run_test dcdc/80 2 blgdet bdd-mixed
run_test dcdc/120 2 blgdet bdd-mixed
run_test dcdc/160 2 blgdet bdd-mixed
run_test dcdc/200 2 blgdet bdd-mixed

echo -e "\n*******************************\n"

run_test dcdc_rec/1/1 2 blgdet bdd-mixed
run_test dcdc_rec/1/40 2 blgdet bdd-mixed
run_test dcdc_rec/1/80 2 blgdet bdd-mixed
run_test dcdc_rec/1/120 2 blgdet bdd-mixed
run_test dcdc_rec/1/160 2 blgdet bdd-mixed
run_test dcdc_rec/1/200 2 blgdet bdd-mixed

echo -e "\n*******************************\n"

run_test dcdc_rec/2/1 2 blgdet bdd-mixed
run_test dcdc_rec/2/40 2 blgdet bdd-mixed
run_test dcdc_rec/2/80 2 blgdet bdd-mixed
run_test dcdc_rec/2/120 2 blgdet bdd-mixed
run_test dcdc_rec/2/160 2 blgdet bdd-mixed
run_test dcdc_rec/2/200 2 blgdet bdd-mixed

echo -e "\n*******************************\n"

run_test vehicle/1 3 blgdet bdd-mixed
run_test vehicle/2 3 blgdet bdd-mixed
run_test vehicle/3 3 blgdet bdd-mixed
run_test vehicle/4 3 blgdet bdd-mixed
run_test vehicle/5 3 blgdet bdd-mixed

echo -e "\n*******************************\n"

run_test aircraft/1 3 blgdet bdd-mixed
run_test aircraft/2 3 blgdet bdd-mixed
run_test aircraft/3 3 blgdet bdd-mixed

echo -e "\n*******************************\n"

run_test dcm/1 2 bldet bdd-local
run_test dcm/5 2 bldet bdd-local
run_test dcm/10 2 bldet bdd-local
run_test dcm/15 2 bldet bdd-local
run_test dcm/20 2 bldet bdd-local
run_test dcm/25 2 bldet bdd-local

echo -e "\n*******************************\n"

run_test dcdc/1 2 bldet bdd-local
run_test dcdc/40 2 bldet bdd-local
run_test dcdc/80 2 bldet bdd-local
run_test dcdc/120 2 bldet bdd-local
run_test dcdc/160 2 bldet bdd-local
run_test dcdc/200 2 bldet bdd-local

echo -e "\n*******************************\n"

run_test dcdc_rec/1/1 2 bldet bdd-local
run_test dcdc_rec/1/40 2 bldet bdd-local
run_test dcdc_rec/1/80 2 bldet bdd-local
run_test dcdc_rec/1/120 2 bldet bdd-local
run_test dcdc_rec/1/160 2 bldet bdd-local
run_test dcdc_rec/1/200 2 bldet bdd-local

echo -e "\n*******************************\n"

run_test dcdc_rec/2/1 2 bldet bdd-local
run_test dcdc_rec/2/40 2 bldet bdd-local
run_test dcdc_rec/2/80 2 bldet bdd-local
run_test dcdc_rec/2/120 2 bldet bdd-local
run_test dcdc_rec/2/160 2 bldet bdd-local
run_test dcdc_rec/2/200 2 bldet bdd-local

echo -e "\n*******************************\n"

run_test vehicle/1 3 bldet bdd-local
run_test vehicle/2 3 bldet bdd-local
run_test vehicle/3 3 bldet bdd-local
run_test vehicle/4 3 bldet bdd-local
run_test vehicle/5 3 bldet bdd-local

echo -e "\n*******************************\n"

run_test aircraft/1 3 bldet bdd-local
run_test aircraft/2 3 bldet bdd-local
run_test aircraft/3 3 bldet bdd-local

echo -e "\n*******************************\n"

run_test dcm/1 2 lgdet mixed
run_test dcm/5 2 lgdet mixed
run_test dcm/10 2 lgdet mixed
run_test dcm/15 2 lgdet mixed
run_test dcm/20 2 lgdet mixed
run_test dcm/25 2 lgdet mixed

echo -e "\n*******************************\n"

run_test dcdc/1 2 lgdet mixed
run_test dcdc/40 2 lgdet mixed
run_test dcdc/80 2 lgdet mixed
run_test dcdc/120 2 lgdet mixed
run_test dcdc/160 2 lgdet mixed
run_test dcdc/200 2 lgdet mixed

echo -e "\n*******************************\n"

run_test dcdc_rec/1/1 2 lgdet mixed
run_test dcdc_rec/1/40 2 lgdet mixed
run_test dcdc_rec/1/80 2 lgdet mixed
run_test dcdc_rec/1/120 2 lgdet mixed
run_test dcdc_rec/1/160 2 lgdet mixed
run_test dcdc_rec/1/200 2 lgdet mixed

echo -e "\n*******************************\n"

run_test dcdc_rec/2/1 2 lgdet mixed
run_test dcdc_rec/2/40 2 lgdet mixed
run_test dcdc_rec/2/80 2 lgdet mixed
run_test dcdc_rec/2/120 2 lgdet mixed
run_test dcdc_rec/2/160 2 lgdet mixed
run_test dcdc_rec/2/200 2 lgdet mixed

echo -e "\n*******************************\n"

run_test vehicle/1 3 lgdet mixed
run_test vehicle/2 3 lgdet mixed
run_test vehicle/3 3 lgdet mixed
run_test vehicle/4 3 lgdet mixed
run_test vehicle/5 3 lgdet mixed

echo -e "\n*******************************\n"

run_test aircraft/1 3 lgdet mixed
run_test aircraft/2 3 lgdet mixed
run_test aircraft/3 3 lgdet mixed

echo -e "\n*******************************\n"

run_test dcm/1 2 ldet local
run_test dcm/5 2 ldet local
run_test dcm/10 2 ldet local
run_test dcm/15 2 ldet local
run_test dcm/20 2 ldet local
run_test dcm/25 2 ldet local

echo -e "\n*******************************\n"

run_test dcdc/1 2 ldet local
run_test dcdc/40 2 ldet local
run_test dcdc/80 2 ldet local
run_test dcdc/120 2 ldet local
run_test dcdc/160 2 ldet local
run_test dcdc/200 2 ldet local

echo -e "\n*******************************\n"

run_test dcdc_rec/1/1 2 ldet local
run_test dcdc_rec/1/40 2 ldet local
run_test dcdc_rec/1/80 2 ldet local
run_test dcdc_rec/1/120 2 ldet local
run_test dcdc_rec/1/160 2 ldet local
run_test dcdc_rec/1/200 2 ldet local

echo -e "\n*******************************\n"

run_test dcdc_rec/2/1 2 ldet local
run_test dcdc_rec/2/40 2 ldet local
run_test dcdc_rec/2/80 2 ldet local
run_test dcdc_rec/2/120 2 ldet local
run_test dcdc_rec/2/160 2 ldet local
run_test dcdc_rec/2/200 2 ldet local

echo -e "\n*******************************\n"

run_test vehicle/1 3 ldet local
run_test vehicle/2 3 ldet local
run_test vehicle/3 3 ldet local
run_test vehicle/4 3 ldet local
run_test vehicle/5 3 ldet local

echo -e "\n*******************************\n"

run_test aircraft/1 3 ldet local
run_test aircraft/2 3 ldet local
run_test aircraft/3 3 ldet local

echo -e "\n*******************************\n"

run_test dcm/1 2 det global
run_test dcm/5 2 det global
run_test dcm/10 2 det global
run_test dcm/15 2 det global
run_test dcm/20 2 det global
run_test dcm/25 2 det global

echo -e "\n*******************************\n"

run_test dcdc/1 2 det global
run_test dcdc/40 2 det global
run_test dcdc/80 2 det global
run_test dcdc/120 2 det global
run_test dcdc/160 2 det global
run_test dcdc/200 2 det global

echo -e "\n*******************************\n"

run_test dcdc_rec/1/1 2 det global
run_test dcdc_rec/1/40 2 det global
run_test dcdc_rec/1/80 2 det global
run_test dcdc_rec/1/120 2 det global
run_test dcdc_rec/1/160 2 det global
run_test dcdc_rec/1/200 2 det global

echo -e "\n*******************************\n"

run_test dcdc_rec/2/1 2 det global
run_test dcdc_rec/2/40 2 det global
run_test dcdc_rec/2/80 2 det global
run_test dcdc_rec/2/120 2 det global
run_test dcdc_rec/2/160 2 det global
run_test dcdc_rec/2/200 2 det global

echo -e "\n*******************************\n"

run_test vehicle/1 3 det global
run_test vehicle/2 3 det global
run_test vehicle/3 3 det global
run_test vehicle/4 3 det global
run_test vehicle/5 3 det global

echo -e "\n*******************************\n"

run_test aircraft/1 3 det global
run_test aircraft/2 3 det global
run_test aircraft/3 3 det global
