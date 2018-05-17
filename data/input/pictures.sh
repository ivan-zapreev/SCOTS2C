#!/bin/bash

BINARY_HOME="../../build/src/optdet"
MODELS_HOME="./models"
BINARY="${BINARY_HOME}/scots_to_svg"
INKSCAPE_BIN="/Applications/Inkscape.app/Contents/Resources/bin/Inkscape"

function bdd2svg() {
    CMD="${BINARY} -s ${1} -t ${2} -l info -d ${3} ${5}"
    echo "bdd2svg: ${CMD}"
    ${CMD} >> ${4}
}

function svg2pdf() {
    SVG_SIZE=`ls -al ${PWD}/${1}.svg | awk '{print $5}'`
    #If the image size is greater than 10 Mb - shrink
    if [ "${SVG_SIZE}" -gt "10485760" ]; then
        head -n 100000 ${PWD}/${1}.svg | grep -v "stroke=\"green\"" > ${PWD}/${1}.small.svg
        echo "</svg>" >> ${PWD}/${1}.small.svg
        CMD="${INKSCAPE_BIN} ${PWD}/${1}.small.svg -A=${PWD}/${1}.small.pdf --without-gui"
    else
        CMD="${INKSCAPE_BIN} ${PWD}/${1}.svg -A=${PWD}/${1}.pdf --without-gui"
    fi
    echo "svg2pdf: ${CMD}"
    ${CMD} >> ${2}
}

function gen_imgs() {
    echo "========================================================================="
    
    #Define the array of model extentions
    exts=( reo ext lin con blin bcon );

    #Define the images dir and create it if not present
    IMGS_DIR=${MODELS_HOME}/${1}/${2}/imgs
    mkdir -p ${IMGS_DIR}
    
    #Prepare the log file
    LOG_FILE_NAME=${IMGS_DIR}/c.log
    rm -f ${LOG_FILE_NAME}
    echo "Logging into: ${LOG_FILE_NAME}"

    #Iterate over extensions and generate svg images
    for ext in "${exts[@]}"; do
        #Prepare varible values
        SOURCE_BDD_NAME=${MODELS_HOME}/$1/${2}/c_${ext}
        TARGET_IMG_NAME_TMPL=${IMGS_DIR}/c_${ext}
        
        #Generate the scots indexes svg
        TARGET_IMG_NAME=${TARGET_IMG_NAME_TMPL}_s
        bdd2svg ${SOURCE_BDD_NAME} ${TARGET_IMG_NAME} ${3} ${LOG_FILE_NAME}
        svg2pdf ${TARGET_IMG_NAME} ${LOG_FILE_NAME}
    
        #Generate the bdd indexes svg
        TARGET_IMG_NAME=${TARGET_IMG_NAME_TMPL}_b
        bdd2svg ${SOURCE_BDD_NAME} ${TARGET_IMG_NAME} ${3} ${LOG_FILE_NAME} "-b"
        svg2pdf ${TARGET_IMG_NAME} ${LOG_FILE_NAME}
    done
}

echo -e "\n*******************************\n"
gen_imgs dcm/1 blgdet 2
gen_imgs dcm/1 lgdet 2

echo -e "\n*******************************\n"
gen_imgs dcdc/1 blgdet 2
gen_imgs dcdc/1 lgdet 2

echo -e "\n*******************************\n"
gen_imgs dcdc_rec/1/1 blgdet 2
gen_imgs dcdc_rec/1/1 lgdet 2

echo -e "\n*******************************\n"
gen_imgs dcdc_rec/2/1 blgdet 2
gen_imgs dcdc_rec/2/1 lgdet 2

echo -e "\n*******************************\n"
gen_imgs vehicle/1 blgdet 3
gen_imgs vehicle/1 lgdet 3

echo -e "\n*******************************\n"
gen_imgs aircraft/1 blgdet 3
gen_imgs aircraft/1 lgdet 3
