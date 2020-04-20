
NRUNS=$1
i=1
while [ $i -le $NRUNS ]
  do
    logtime=`date +%m%d-%H%M`
    # python MGrunningJSON.py -r -delphes -s '{"BkgSigTag":"bkg"}' |& tee ./MGrunningJSON-$logtime.log
    # python MGrunningJSON.py -r -delphes -s '{}' -e 1500 |& tee ./MGrunningJSON-$logtime.log 
    python MGrunningJSON.py -r -delphes -s '{"BkgSigTag":"bkg"}' -e 1500 |& tee ./MGrunningJSON-$logtime.log
    i=$[$i+1]
  done
