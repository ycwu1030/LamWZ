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
parser.add_argument('-p',dest='Plots',action='store_true')
parser.add_argument('-am',dest='amode',default='wh') # analysis mode
parser.add_argument('-i',dest='processfile',default='processes_in_this_dict.json')
parser.add_argument('-e',dest='sqrts',default=3000,type=int)# in GeV
args = parser.parse_args()

ProcessesFile=args.inputdir + '/' + args.processfile
SRCDIR='/data/data068/ycwu/LamWZ/Event_Generation/Events/LamWZPlot'
PlotsNAME='LamWZPlot'
sqrts = args.sqrts


# Link required signal and background processes information 
Lumi=Lumi_map['%d'%(sqrts)]
nsig=0
nbkg=0
signame=[]
bkgname=[]
siglabel=[]
bkglabel=[]
signeve=[]
bkgneve=[]


with open(ProcessesFile,'r') as f:
    ProcessesList = (simplejson.load(f))
    for key in ProcessesList.keys():
        process=ProcessesList[key]
        print("Adding process: ",process['Name'])
        if process['SorB'] == 0:
            bkgname.append(process['Abbr'])
            bkglabel.append(process['Name'])
            bkgneve.append(str(process['NEvents']))
            nbkg+=1
        else:
            signame.append(process['Abbr'])
            siglabel.append(process['Name'])
            signeve.append(str(process['NEvents']))
            nsig+=1


signame_str='\",\"'.join(signame)
bkgname_str='\",\"'.join(bkgname)
siglabel_str='\",\"'.join(siglabel)
bkglabel_str='\",\"'.join(bkglabel)
signeve_str=','.join(signeve)
bkgneve_str=','.join(bkgneve)

subprocess.call("sed -e 's/__LUMI__/%d/g' -e 's/__NSIG__/%d/g' -e 's/__NBKG__/%d/g' -e 's/__SIGNAME__/\"%s\"/g' -e 's/__BKGNAME__/\"%s\"/g' -e 's/__SIGLABEL__/\"%s\"/g' -e 's/__BKGLABEL__/\"%s\"/g' -e 's/__SIGNEVE__/%s/g' -e 's/__BKGNEVE__/%s/g' %s/%s_tmp.cpp > %s/%s.cpp "%(Lumi,nsig,nbkg,signame_str,bkgname_str,siglabel_str,bkglabel_str,signeve_str,bkgneve_str,SRCDIR,PlotsNAME,SRCDIR,PlotsNAME),shell=True)
