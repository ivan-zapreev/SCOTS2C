#!/bin/bash

SPLIT_EXEC=../../build/src/optdet/scots_split_det

function ctrl_split_stats() {
   #Prepare variables
   CTRL_FILE_TEMPL="models/${1}/${2}"
   SPLIT_DIR="models/${1}/split"
   mkdir -p ${SPLIT_DIR}
   SPLIT_FILE_TEMPL="${SPLIT_DIR}/${2}"
   LOG_FILE_NAME="${SPLIT_FILE_TEMPL}.log"

    echo "**** ${LOG_FILE_NAME} ****"
   
   #Remove any opld files
   rm -rf ${SPLIT_FILE_TEMPL}*
   
   #Run the splitter
   CMD="${SPLIT_EXEC} -s ${CTRL_FILE_TEMPL} -t ${SPLIT_FILE_TEMPL} -d ${3} -l info -p"
   echo "Running: ${CMD}" > ${LOG_FILE_NAME}
   ${CMD} > ${LOG_FILE_NAME}
   
   #Get the statistics
   ls -al ${CTRL_FILE_TEMPL}.bdd | awk '{print "\tOriginal controller size: " $5 }'
   ls -al ${SPLIT_FILE_TEMPL}_dom.bdd | awk '{print "\tSupport controller size: " $5 }'
}

ctrl_split_stats "dcm/1/scots" "c_reo" 2
ctrl_split_stats "dcm/5/scots" "c_reo" 2
ctrl_split_stats "dcm/10/scots" "c_reo" 2
ctrl_split_stats "dcm/15/scots" "c_reo" 2
ctrl_split_stats "dcm/20/scots" "c_reo" 2
ctrl_split_stats "dcm/25/scots" "c_reo" 2

ctrl_split_stats "dcdc/1/scots" "c_reo" 2
ctrl_split_stats "dcdc/40/scots" "c_reo" 2
ctrl_split_stats "dcdc/80/scots" "c_reo" 2
ctrl_split_stats "dcdc/120/scots" "c_reo" 2
ctrl_split_stats "dcdc/160/scots" "c_reo" 2
ctrl_split_stats "dcdc/200/scots" "c_reo" 2

ctrl_split_stats "dcdc_rec/1/1/scots" "c_reo" 2
ctrl_split_stats "dcdc_rec/1/40/scots" "c_reo" 2
ctrl_split_stats "dcdc_rec/1/80/scots" "c_reo" 2
ctrl_split_stats "dcdc_rec/1/120/scots" "c_reo" 2
ctrl_split_stats "dcdc_rec/1/160/scots" "c_reo" 2
ctrl_split_stats "dcdc_rec/1/200/scots" "c_reo" 2

ctrl_split_stats "dcdc_rec/2/1/scots" "c_reo" 2
ctrl_split_stats "dcdc_rec/2/40/scots" "c_reo" 2
ctrl_split_stats "dcdc_rec/2/80/scots" "c_reo" 2
ctrl_split_stats "dcdc_rec/2/120/scots" "c_reo" 2
ctrl_split_stats "dcdc_rec/2/160/scots" "c_reo" 2
ctrl_split_stats "dcdc_rec/2/200/scots" "c_reo" 2

ctrl_split_stats "vehicle/1/scots" "c_reo" 3
ctrl_split_stats "vehicle/2/scots" "c_reo" 3
ctrl_split_stats "vehicle/3/scots" "c_reo" 3
ctrl_split_stats "vehicle/4/scots" "c_reo" 3
ctrl_split_stats "vehicle/5/scots" "c_reo" 3

ctrl_split_stats "aircraft/1/scots" "c_reo" 3
ctrl_split_stats "aircraft/2/scots" "c_reo" 3
ctrl_split_stats "aircraft/3/scots" "c_reo" 3
