#! /bin/zsh

source Setup.sh

bin/analyze pp rootFiles/pp/gen_all.root rootFiles/pp/hists.root > outputs/pp_analyze.out 2> errors/pp_analyze.err &
bin/analyze pp_bkg rootFiles/pp/gen_bkg_all.root rootFiles/pp/hists_bkg.root > outputs/pp_bkg_analyze.out 2> errors/pp_bkg_analyze.err &

bin/analyze pPb rootFiles/pPb/gen_all.root rootFiles/pPb/hists.root > outputs/pPb_analyze.out 2> errors/pPb_analyze.err &
bin/analyze pPb_bkg rootFiles/pPb/gen_bkg_all.root rootFiles/pPb/hists_bkg.root > outputs/pPb_bkg_analyze.out 2> errors/pPb_bkg_analyze.err &

wait