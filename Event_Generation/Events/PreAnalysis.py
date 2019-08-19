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

parser = argparse.ArgumentParser(prog='PreAnalysis',description='PreAnalysis of Lambda WZ events.')
parser.add_argument('-o',dest='outdir',default='/data/data068/ycwu/LamWZ/Event_Generation/Events/PreAna')
parser.add_argument('-m',dest='mode',default='wh')
parser.add_argument('-c',dest='case',default='Full')
parser.add_argument('-e',dest='sqrts',default=3000,type=int)# in GeV
args = parser.parse_args()

ProcessesFile="../processes_list.json"
decay="bbll"
Analysis='/data/data068/ycwu/LamWZ/Event_Generation/Events/PreAnalysis/LamWZPreAnalysis.x'
Outdir=args.outdir
mode_str=args.mode
if mode_str == 'wh':
    mode_int = 1
else:
    mode_int = 2
sqrts = args.sqrts
case_str=args.case
if case_str == 'Full':
    case_int = 1
else:
    case_int = 0

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
        finalname='%s_%d.root'%(nameprefix,fileid)
        if '%s/%s'%(rootdir,finalname) in rootfiles:
            fileid = fileid + 1
        else:
            break
    return finalname

ProcessesInfo={}
with open(ProcessesFile,'r') as f:
    ProcessesList = (simplejson.load(f))['Processes']
    for process in ProcessesList:
        print "Processing process: ",process['Name']
        process['NEvents']=0
        rootfiles = ListRootFiles(process['Name']+'/Delphes/' + '%d'%(sqrts), decay)
        processID = 100*Process[process['Abbr']]+10*Decays[decay]+BkgSigTag[process['BkgSigTag']]
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
        if process['BkgSigTag'] != 'bkg' and process['Abbr'] != mode_str:
            continue
        if case_str == 'Full':
            if process['BkgSigTag'] != 'bkg' and process['BkgSigTag'] != 'Full':
                continue
        for rootfile in rootfiles:
            filename=rootfile.split('/')[-1]
            RandomID=(filename.split('.')[0]).split('_')[-2:]
            OutfilePrefix = '_'.join([process['Abbr'],process['BkgSigTag'],decay,RandomID[0],RandomID[1]])
            outfilename=GenerateFileName(Outdir,OutfilePrefix)
            subprocess.call('%s %s %s %s %d %f %d'%(Analysis,rootfile,Outdir,outfilename,processID,CS,mode_int),shell=True)
            with open(Outdir+'/events_proc_%d.dat'%(processID),'r') as log:
                nevents = log.read()
            process['NEvents']+=int(nevents)
            try:
                os.remove(Outdir+'/events_proc_%d.dat'%(processID))
            except OSError:
                pass
        if process['NEvents'] != 0:
            ProcessesInfo['%d'%(processID)] = process
    with open(Outdir+'/processes_in_this_dict.json','w') as logall:
        simplejson.dump(ProcessesInfo,logall)
