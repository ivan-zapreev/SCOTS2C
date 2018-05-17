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
   CMD="${SPLIT_EXEC} -s ${CTRL_FILE_TEMPL} -t ${SPLIT_FILE_TEMPL} -d ${3} -l info -p -i"
   echo "Running: ${CMD}" > ${LOG_FILE_NAME}
   ${CMD} > ${LOG_FILE_NAME}
   
   #Get the statistics
   ls -al ${CTRL_FILE_TEMPL}.bdd | awk '{print "\tOriginal controller size: " $5 }'
   ls -al ${SPLIT_FILE_TEMPL}_dom.bdd | awk '{print "\tSupport controller size: " $5 }'
   ls -al ${SPLIT_FILE_TEMPL}_[0-9]*.bdd | awk 'BEGIN{sum=0;} {sum += $5;} END{print "\tSplit controllers size: "sum;}' 
}

ctrl_split_stats "dcm/1/det" "c_reo" 2
ctrl_split_stats "dcm/5/det" "c_reo" 2
ctrl_split_stats "dcm/10/det" "c_reo" 2
ctrl_split_stats "dcm/15/det" "c_reo" 2
ctrl_split_stats "dcm/20/det" "c_reo" 2
ctrl_split_stats "dcm/25/det" "c_reo" 2

ctrl_split_stats "dcdc/1/det" "c_reo" 2
ctrl_split_stats "dcdc/40/det" "c_reo" 2
ctrl_split_stats "dcdc/80/det" "c_reo" 2
ctrl_split_stats "dcdc/120/det" "c_reo" 2
ctrl_split_stats "dcdc/160/det" "c_reo" 2
ctrl_split_stats "dcdc/200/det" "c_reo" 2

ctrl_split_stats "dcdc/1/sym" "c_reo" 2
ctrl_split_stats "dcdc/40/sym" "c_reo" 2

ctrl_split_stats "dcdc_rec/1/1/det" "c_reo" 2
ctrl_split_stats "dcdc_rec/1/40/det" "c_reo" 2
ctrl_split_stats "dcdc_rec/1/80/det" "c_reo" 2
ctrl_split_stats "dcdc_rec/1/120/det" "c_reo" 2
ctrl_split_stats "dcdc_rec/1/160/det" "c_reo" 2
ctrl_split_stats "dcdc_rec/1/200/det" "c_reo" 2

ctrl_split_stats "dcdc_rec/1/1/sym" "c_reo" 2
ctrl_split_stats "dcdc_rec/1/40/sym" "c_reo" 2

ctrl_split_stats "dcdc_rec/2/1/det" "c_reo" 2
ctrl_split_stats "dcdc_rec/2/40/det" "c_reo" 2
ctrl_split_stats "dcdc_rec/2/80/det" "c_reo" 2
ctrl_split_stats "dcdc_rec/2/120/det" "c_reo" 2
ctrl_split_stats "dcdc_rec/2/160/det" "c_reo" 2
ctrl_split_stats "dcdc_rec/2/200/det" "c_reo" 2

ctrl_split_stats "dcdc_rec/2/1/sym" "c_reo" 2
ctrl_split_stats "dcdc_rec/2/40/sym" "c_reo" 2

ctrl_split_stats "vehicle/1/det" "c_reo" 3
ctrl_split_stats "vehicle/2/det" "c_reo" 3
ctrl_split_stats "vehicle/3/det" "c_reo" 3
ctrl_split_stats "vehicle/4/det" "c_reo" 3
ctrl_split_stats "vehicle/5/det" "c_reo" 3

ctrl_split_stats "vehicle/1/sym" "c_reo" 3
ctrl_split_stats "vehicle/2/sym" "c_reo" 3

ctrl_split_stats "aircraft/1/det" "c_reo" 3
ctrl_split_stats "aircraft/2/det" "c_reo" 3
ctrl_split_stats "aircraft/3/det" "c_reo" 3

ctrl_split_stats "aircraft/1/sym" "c_reo" 3
ctrl_split_stats "aircraft/2/sym" "c_reo" 3
