#! /bin/zsh

source Setup.sh

bin/gaps pp "rootFiles/pp/gen_SoftQCD_Inelastic_job*.root" rootFiles/pp/gaps_hists.root > outputs/pp_gaps.out 2> errors/pp_gaps.err &
bin/gaps pPb "rootFiles/pPb/gen_SoftQCD_Inelastic_job*.root" rootFiles/pPb/gaps_hists.root > outputs/pPb_gaps.out 2> errors/pPb_gaps.err &

wait
