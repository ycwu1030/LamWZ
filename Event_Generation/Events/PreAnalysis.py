#!/usr/bin/env python

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
BkgSigTag={'bkg':0,'hWW':1,'hZZ':2,'Inter':3}
Process={'wh':1,'zh':2,'tt':3,'wz':4,'zz':5}
#ProsList=['bkg_tt_bbll','bkg_VBF_wz_bbll','bkg_VBF_zz_bbll','bkg_VBF_zz_tatall','ee_VBF_wh_InterOnly','ee_VBF_wh_WOnly','ee_VBF_wh_ZOnly','ee_VBF_zh_InterOnly','ee_VBF_zh_WOnly','ee_VBF_zh_ZOnly']
#ChannelInfo=[]
# Following BR information is used for signal process CS calculation
BRZll=0.067316
BRWlv=0.2172
BRhbb=0.5809
BRhtata=0.06256

ProcessesFile="../processes_list.json"
decay="bbll"
Analysis='/data/data068/ycwu/LamWZ/Event_Generation/Events/PreAnalysis/LamWZPreAnalysis.x'
Outdir='/data/data068/ycwu/LamWZ/Event_Generation/Events/PreAna'
if not os.path.exists(Outdir):
    os.makedirs(Outdir)

def ListRootFiles(rootdir,decaychannel='bbll'):
    return glob.glob(rootdir+'/*_'+decaychannel+'_*.root')

def GenerateFileName(rootdir,nameprefix):
    good=False
    rootfiles=glob.glob(rootdir+'/*.root')
    fileid=0
    finalname=''
    while not good:
        finalname='%s/%s_%d.root'%(rootdir,nameprefix,fileid)
        if finalname in rootfiles:
            fileid = fileid + 1
        else:
            break
    return finalname

with open(ProcessesFile,'r') as f:
    ProcessesList = (simplejson.load(f))['Processes']
    for process in ProcessesList:
        print "Processing process: ",process['Name']
        rootfiles = ListRootFiles(process['Name']+'/Delphes',decay)
        ChannelID = 100*Process[process['Abbr']]+10*Decays[decay]+BkgSigTag[process['BkgSigTag']]
        if process['BkgSigTag'] == 'bkg':
            CS=process['CS']
        else:
            if decay == 'bbll':
                BRall = BRhbb
            else:
                BRall = BRhtata
            if process['Abbr'] == 'wh':
                BRall = BRall*BRWlv
            else:
                BRall = BRall*BRZll
            CS=process['CS']*BRall
        for rootfile in rootfiles:
            filename=rootfile.split('/')[-1]
            RandomID=(filename.split('.')[0]).split('_')[-2:]
            OutfilePrefix = '_'.join([process['Abbr'],process['BkgSigTag'],decay,RandomID[0],RandomID[1]])
            outfilename=GenerateFileName(Outdir,OutfilePrefix)
            subprocess.call('%s %s %s %d %f'%(Analysis,rootfile,outfilename,ChannelID,CS),shell=True)
