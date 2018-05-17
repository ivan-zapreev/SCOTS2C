#!/bin/bash

WORK_DIR="$PWD"
echo "Being run in: ${WORK_DIR}"

function build() {
    cd $1
    make $2
    cd ${WORK_DIR}
}

build ./models/aircraft aircraft
build ./models/vehicle vehicle
build ./models/dcm dcm
build ./models/dcdc dcdc
build ./models/dcdc_rec dcdc_rec
