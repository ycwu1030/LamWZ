#!/bin/bash

topdir="/data/data068/ycwu/LamWZ/Event_Generation/Events"

for mode in 'wh' 'zh'
    do
      for sqrts in 1500 3000
         do
           python PlotAnalysis.py -input_dir $topdir/PreAna_"$mode"_"$sqrts"_SM_Full -rm p -am $mode -r 0 -e $sqrts -ic 'VOID'
           python PlotAnalysis.py -input_dir $topdir/PreAna_"$mode"_"$sqrts"_SM_Full -rm p -am $mode -r 0 -e $sqrts
         done
    done

cd Plots
tar -czvf Cuts_Results_Plots.tar.gz Plots_*
GEINDEX
cd -

