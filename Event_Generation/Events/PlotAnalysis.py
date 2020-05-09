#!/usr/bin/env python

from __future__ import print_function
import os
import glob
import stat
import sys
import shutil
import subprocess
import numpy as np
import time
import math
import shutil
import pickle
import argparse
import simplejson

Decays={'bbll':1,'tatall':2}
Lumi_map={'3000':4000,'1500':2000}
SIGCOLOR=["kBlue+4","kBlue+3","kBlue+2","kBlue+1","kBlue","kBlue-4","kBlue-7","kBlue-9","kBlue-10"]
#ProsList=['bkg_tt_bbll','bkg_VBF_wz_bbll','bkg_VBF_zz_bbll','bkg_VBF_zz_tatall','ee_VBF_wh_InterOnly','ee_VBF_wh_WOnly','ee_VBF_wh_ZOnly','ee_VBF_zh_InterOnly','ee_VBF_zh_WOnly','ee_VBF_zh_ZOnly']
#ChannelInfo=[]
# Following BR information is used for signal process CS calculation
BRZll=0.067316
BRZvv=0.20000
BRWlv=0.2172
BRhbb=0.5809
BRZbb=0.1512
BRhtata=0.06256

parser = argparse.ArgumentParser(prog='PreAnalysis',description='PreAnalysis of Lambda WZ events.')
parser.add_argument('-input_dir',dest='inputdir',default='/data/data068/ycwu/LamWZ/Event_Generation/Events/PreAna_wh_3000_Full')
parser.add_argument('-rm',dest='rmode',default='p') # running mode, 
# p: plot, ct: train cuts, ca: apply cuts, bt: train bdt, ba: apply bdt, br: BDT results
parser.add_argument('-am',dest='amode',default='wh') # analysis mode
parser.add_argument('-i',dest='processfile',default='processes_in_this_dict.json')
parser.add_argument('-ic',dest='cutfile',default='HARD_CUTS.json')
parser.add_argument('-e',dest='sqrts',default=3000,type=int)# in GeV
parser.add_argument('-t',dest='tag',default='wh_0508_1557')
parser.add_argument('-r',dest='renorm',default=1,type=int)
args = parser.parse_args()

cutfile=args.cutfile
ProcessesFile=args.inputdir + '/' + args.processfile
SRCDIR='/data/data068/ycwu/LamWZ/Event_Generation/Events/LamWZPlot'
PlotsNAME='LamWZPlot'
CutTrainNAME='LamWZCutTrain'
CutApplyNAME='LamWZCutApply'
BDTTrainNAME='LamWZBDTTrain'
BDTApplyNAME='LamWZBDTApply'
BDTResultNAME='LamWZBDTResults'
rmode=args.rmode
sqrts = args.sqrts
renorm=args.renorm
amode=args.amode
tag=amode+'_%d_'%(sqrts)+time.strftime("%m%d_%H%M")

if amode == 'wh':
    channelID = 1
elif amode == 'zh':
    channelID = 2

if rmode == 'p':
    EXENAME=PlotsNAME
elif rmode == 'ct':
    EXENAME=CutTrainNAME
elif rmode == 'ca':
    EXENAME=CutApplyNAME
    tag = args.tag
elif rmode == 'bt':
    EXENAME=BDTTrainNAME
elif rmode == 'ba':
    EXENAME=BDTApplyNAME
    tag = args.tag
elif rmode == 'br':
    EXENAME=BDTResultNAME
    tag = args.tag

# Link required signal and background processes information 
Lumi=Lumi_map['%d'%(sqrts)]
nsig=0
nbkg=0
sigid=[]
bkgid=[]
signame=[]
bkgname=[]
siglabel=[]
bkglabel=[]
signeve=[]
bkgneve=[]

# if amode == 'zh':
#     sigid = [0]
#     signame = ['']
#     siglabel = ['']
#     signeve = [1]
#     nsig+=1

with open(ProcessesFile,'r') as f:
    ProcessesList = (simplejson.load(f))
    for key in ProcessesList.keys():
        process=ProcessesList[key]
        print("Adding process: ",process['Name'])
        if process['SorB'] == 0:
            bkgid.append(process['ProcessID'])
            bkgname.append(process['Abbr'])
            bkglabel.append(process['Name'])
            bkgneve.append(str(process['NEvents']))
            nbkg+=1
        else:
            sigid.append(process['ProcessID'])
            signame.append(process['Abbr'])
            siglabel.append(process['Name'])
            signeve.append(str(process['NEvents']))
            nsig+=1

# print(bkgid,bkgname,bkglabel)

p = np.array(bkgid).argsort()
bkgname=np.array(bkgname)[p]
bkglabel=np.array(bkglabel)[p]
bkgneve=np.array(bkgneve)[p]

# print(bkgname,bkglabel,bkgneve)

p = np.array(sigid)
signame=np.array(signame)[p]
siglabel=np.array(siglabel)[p]
signeve=np.array(signeve)[p]


signame_str='\",\"'.join(signame)
bkgname_str='\",\"'.join(bkgname)
siglabel_str='\",\"'.join(siglabel)
bkglabel_str='\",\"'.join(bkglabel)
signeve_str=','.join(signeve)
bkgneve_str=','.join(bkgneve)


def ReadJsonValue(entry,mode,s):
    if type(entry) != dict:
        return entry
    else:
        if type(entry[mode]) != dict:
            return entry[mode]
        else:
            return entry[mode][s]

cutslist=['1==1']
if os.path.exists(cutfile):
    with open(cutfile,'r') as fcut:
        cutsinfo=(simplejson.load(fcut))['HARD_CUTS']
        for key in cutsinfo.keys():
            var=cutsinfo[key]
            varname=var['Name']
            varstr=''
            relationtype = ReadJsonValue(var['Relation'],amode,'%d'%(sqrts))
            linksym = '\&\&' if relationtype == 'AND' else '||'
            varmin = ReadJsonValue(var['Min'],amode,'%d'%(sqrts))
            varmax = ReadJsonValue(var['Max'],amode,'%d'%(sqrts))
            if varmin is None and varmax is None:
                continue
            elif varmin is None:
                varstr='(%s<=%f)'%(varname,varmax)
            elif varmax is None:
                varstr='(%f<=%s)'%(varmin,varname)
            else:
                varstr='(%f<=%s %s %s<=%f)'%(varmin,varname,linksym,varname,varmax)
            cutslist.append(varstr)

cuts_str='\&\&'.join(cutslist)
colorlist=[]
if nsig == 2:
    colorlist=[SIGCOLOR[0],SIGCOLOR[4]]
elif nsig == 3:
    colorlist=[SIGCOLOR[0],SIGCOLOR[4],SIGCOLOR[-1]]
else:
    colorlist=SIGCOLOR

color_str=','.join(colorlist)


subprocess.call("sed -e 's/__LUMI__/%d/g' -e 's/__NSIG__/%d/g' -e 's/__NBKG__/%d/g' -e 's/__SIGNAME__/\"%s\"/g' -e 's/__BKGNAME__/\"%s\"/g' -e 's/__SIGLABEL__/\"%s\"/g' -e 's/__BKGLABEL__/\"%s\"/g' -e 's/__SIGNEVE__/%s/g' -e 's/__BKGNEVE__/%s/g' -e 's/__CUTS__/%s/g' -e 's/__SIGCOLOR__/%s/g' %s/%s_tmp.cpp > %s/%s.cpp "%(Lumi,nsig,nbkg,signame_str,bkgname_str,siglabel_str,bkglabel_str,signeve_str,bkgneve_str,cuts_str,color_str,SRCDIR,EXENAME,SRCDIR,EXENAME),shell=True)
subprocess.call("cd %s;make %s.x;cd -"%(SRCDIR,EXENAME),shell=True)

GEINDEX="tree -H . -h -D -v -I index.html --noreport --charset utf-8 -L 1 > index.html"

if rmode == 'p':
    subprocess.call("%s/%s.x %s %s %d"%(SRCDIR,EXENAME,args.inputdir,tag,renorm),shell=True)
    subprocess.call("cd Plots; %s; cd -; cd Plots/Plots_LamWZ_%s; %s; cd -;"%(GEINDEX,tag,GEINDEX),shell=True)
elif rmode == 'ct' or rmode == 'ca':
    subprocess.call("%s/%s.x %s %s %d"%(SRCDIR,EXENAME,args.inputdir,tag,channelID),shell=True)
    subprocess.call("cd Plots; %s; cd -; cd Plots/CUT_Train_LamWZ_%s; %s; cd -;"%(GEINDEX,tag,GEINDEX),shell=True)
elif rmode == 'bt' or rmode == 'ba' or rmode == 'br':
    subprocess.call("%s/%s.x %s %s %d"%(SRCDIR,EXENAME,args.inputdir,tag,channelID),shell=True)
    subprocess.call("cd Plots; %s; cd -; cd Plots/BDT_Train_LamWZ_%s; %s; cd -;"%(GEINDEX,tag,GEINDEX),shell=True)

subprocess.call('echo "Tag is: %s"'%(tag),shell=True)