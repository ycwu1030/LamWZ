#!/bin/bash

tag=$1
DataDir=/data/data068/ycwu/LamWZ/Event_Generation/Events

TrainApp=$DataDir/LamWZPlot/LamWZBDTTrain.x
ApplyApp=$DataDir/LamWZPlot/LamWZBDTApply.x

chan=(xx wh zh)

for chanID in 1 2 
    do
        for sqrts in 1500 3000
            do
                $TrainApp $DataDir/PreAna_"${chan[$chanID]}"_"$sqrts"_Full 1 ${chan[$chanID]}_"$sqrts"_"$tag" $chanID $sqrts >> test_"$tag".log
                $ApplyApp $DataDir/PreAna_"${chan[$chanID]}"_"$sqrts"_Full 1 ${chan[$chanID]}_"$sqrts"_"$tag" $chanID $sqrts >> test_"$tag".log
            done
    done

