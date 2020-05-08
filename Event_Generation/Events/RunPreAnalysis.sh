#!/bin/bash

topdir="/data/data068/ycwu/LamWZ/Event_Generation/Events"

for mode in 'wh' 'zh'
    do
      for sqrts in 1500 3000
         do
           python PreAnalysis.py -o $topdir/PreAna_"$mode"_"$sqrts"_SM -i ../processes_list.json -e $sqrts -m $mode
         done
    done

