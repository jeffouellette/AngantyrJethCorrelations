#! /bin/zsh

source Setup.sh

bin/centrality pp 60 "rootFiles/pp/gen_job*.root" rootFiles/pp/cent_hists.root > outputs/pp_centrality.out 2> errors/pp_centrality.err &
bin/centrality pp_bkg 0 "rootFiles/pp/gen_bkg_job*.root" rootFiles/pp/cent_hists_bkg.root > outputs/pp_bkg_centrality.out 2> errors/pp_bkg_centrality.err &

bin/centrality pPb 60 "rootFiles/pPb/gen_job*.root" rootFiles/pPb/cent_hists.root > outputs/pPb_centrality.out 2> errors/pPb_centrality.err &
bin/centrality pPb_bkg 0 "rootFiles/pPb/gen_bkg_job*.root" rootFiles/pPb/cent_hists_bkg.root > outputs/pPb_bkg_centrality.out 2> errors/pPb_bkg_centrality.err &

wait
