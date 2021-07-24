#! /bin/zsh

source Setup.sh

bin/centrality pp "rootFiles/pp/gen_SoftQCD_ND_job*.root" rootFiles/pp/cent_hists.root > outputs/pp_centrality.out 2> errors/pp_centrality.err &
bin/centrality pPb "rootFiles/pPb/gen_SoftQCD_ND_job*.root" rootFiles/pPb/cent_hists.root > outputs/pPb_centrality.out 2> errors/pPb_centrality.err &

wait
