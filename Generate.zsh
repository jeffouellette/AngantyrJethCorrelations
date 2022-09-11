#! /bin/zsh

declare -a configFiles=("HardQCD_pTHat50_allowRescatter" "HardQCD_pTHat50_allowRescatter_withNPDF")

for configFile in ${configFiles[@]}; do

  #for iJob in $(seq 1 40); do
  for iJob in $(seq 1 1); do
  
    # syntax:      job #    rndm phase  beam A     beam B  sqrt(s)   y*      nevt    config file               root output                                     text output                                     error output
    cmd="./bin/gen ${iJob}  0           2212       2212    5020      0       1000  aux/${configFile}.config  rootFiles/pp/gen_${configFile}_job${iJob}.root  > outputs/pp_gen_${configFile}_job${iJob}.out   2> errors/pp_gen_${configFile}_job${iJob}.err &"
    echo "Launching `echo ${cmd} | cut -c1-80`..."
    eval ${cmd}
  
    cmd="./bin/gen ${iJob}  0           1000822080 2212    5020      -0.465  1000  aux/${configFile}.config  rootFiles/pPb/gen_${configFile}_job${iJob}.root > outputs/pPb_gen_${configFile}_job${iJob}.out  2> errors/pPb_gen_${configFile}_job${iJob}.err &"
    echo "Launching `echo ${cmd} | cut -c1-80`..."
    eval ${cmd}

    (( iJob % 3 == 0 )) && wait
  
  done

done
