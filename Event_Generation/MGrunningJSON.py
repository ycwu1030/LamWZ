#!/usr/bin/env python

# All Parton level process should be decayed 
#(Higgs can be decayed by madspin, and others should be decayed directly at ME level to preserve the spin correlation)
import os
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

Nevents = 50000
currentDir = os.getcwd()
mg5Dir = '/home/ycwu/Workings/MG5_aMC_v2_6_5'
ExROOT = mg5Dir+'/ExRootAnalysis/ExRootLHEFConverter'
modelName = 'SM_kWkZ_UFO'


work_dir = '/data/data068/ycwu/LamWZ/Event_Generation'
if not os.path.exists(work_dir):
    os.makedirs(work_dir)
Event_dir = work_dir +'/Events'
if not os.path.exists(Event_dir):
    os.makedirs(Event_dir)

parser = argparse.ArgumentParser(prog='MGrunning',description='Running MadGraph for process generation and/or event generation.')
parser.add_argument('-g',dest='flag_g',action='store_true') #Generate the MG5 directories
parser.add_argument('-r',dest='flag_r',action='store_true') #Running the MG5
parser.add_argument('-delphes',dest='flag_delphes',action='store_true')
parser.add_argument('-channel',dest='decays',default='bbll')
parser.add_argument('-i',dest='processfile',default='processes_list.json')
args = parser.parse_args()
# print(args)

flag_g = args.flag_g
flag_r = args.flag_r
flag_delphes = args.flag_delphes
decays = args.decays
processfile = args.processfile

with open(processfile,'r') as f:
    ProcessesList=(simplejson.load(f))['Processes']

if flag_g:
    # if -g, then generate the output directory using MG5
    for i in range(len(ProcessesList)):
        with open('tmp_mg5.dat','w') as OutGen:
            OutGen.write('import model ' + modelName + '\n')
            ChannelProcesses=ProcessesList[i]['Processes']
            OutGen.write('generate ' + ChannelProcesses[0] + '\n')
            for j in range(1,len(ChannelProcesses)):
                OutGen.write('add process ' + ChannelProcesses[j] + '\n')
            OutGen.write('output ' + work_dir + '/' + ProcessesList[i]['Name'] + ' -f \n')
            OutGen.write('exit\n')
        subprocess.call(mg5Dir + '/bin/mg5_aMC tmp_mg5.dat', shell=True)
        os.remove('tmp_mg5.dat')

if flag_r:
    for i in range(len(ProcessesList)):
        tag=time.strftime("%m%d_%H")
        Process=ProcessesList[i]
        if Process['Decay'] == 'None':
            Process['Decay'] = decays
        with open('tmp_madevent.dat','w') as MECOM:
            if Process['Abbr'] == 'zh':
                MECOM.write('generate_events run_' + tag + '\n')
                MECOM.write('done\n')
                MECOM.write('set mmll 80.0\n')
                MECOM.write('set mmllmax 100.0\n')
                MECOM.write('done\n')
            else:
                MECOM.write('generate_events run_' + tag + ' -f \n')
        shutil.copyfile(work_dir + '/Cards_tmp/run_card.dat', work_dir + '/' + Process['Name'] + '/Cards/run_card.dat')
        shutil.copyfile(work_dir + '/Cards_tmp/param_card.dat', work_dir + '/' + Process['Name'] + '/Cards/param_card.dat')
        shutil.copyfile(work_dir + '/Cards_tmp/me5_configuration.txt', work_dir + '/' + Process['Name'] + '/Cards/me5_configuration.txt')
	#print Process['Name'],Process['BkgSigTag']
        if Process['BkgSigTag'] != 'bkg':
            #print 'Signal'
            gene_dir = work_dir + '/' + Process['Name'] + '/Events/run_' + tag + "_decayed_1"
            shutil.copyfile(work_dir + '/Cards_tmp/madspin_card_' + decays + '.dat', work_dir + '/' + Process['Name'] + '/Cards/madspin_card.dat')
        else:
            #print 'Bkg'
            gene_dir = work_dir + '/' + Process['Name'] + '/Events/run_' + tag
            try:
                os.remove(work_dir + '/' + Process['Name'] + '/Cards/madspin_card.dat')
            except OSError:
                pass
        if flag_delphes: 
            store_dir = Event_dir + '/' + Process['Name'] + '/Delphes'
            filein = 'tag_1_delphes_events.root'
            fileout = 'delphes_events_' + Process['Decay'] + '_' + tag + '.root'
            shutil.copyfile(work_dir + '/Cards_tmp/delphes_card_CEPC.dat', work_dir + '/' + Process['Name'] + '/Cards/delphes_card.dat')
            shutil.copyfile(work_dir + '/Cards_tmp/pythia8_card.dat', work_dir + '/' + Process['Name'] + '/Cards/pythia8_card.dat')
        else:
            store_dir = Event_dir + '/' + Process['Name'] + '/Parton'
            filein = 'unweighted_events_lhe.root'
            fileout = 'parton_events_' + Process['Decay'] + '_' + tag + '.root'
            try:
                os.remove(work_dir + '/' + Process['Name'] + '/Cards/delphes_card.dat')
            except OSError:
                pass
            try:
                os.remove(work_dir + '/' + Process['Name'] + '/Cards/pythia8_card.dat')
            except OSError:
                pass
        subprocess.call(work_dir + '/' + Process['Name'] + '/bin/madevent ' + currentDir + '/tmp_madevent.dat', cwd = work_dir + '/' + Process['Name'], shell=True)
        if not os.path.exists(store_dir):
            os.makedirs(store_dir)
        if flag_delphes:
            shutil.copyfile(gene_dir + '/' + filein, store_dir + '/' + fileout)
            os.remove(gene_dir + '/' + filein)
            os.remove(gene_dir + '/tag_1_pythia8_events.hepmc.gz')
        else:
            subprocess.call('gunzip unweighted_events.lhe.gz',cwd=gene_dir,shell=True) # gunzip the lhe.gz file 
            subprocess.call(ExROOT + ' unweighted_events.lhe  unweighted_events_lhe.root',cwd=gene_dir,shell=True) # run ExRootAnalysis to convert lhe file to root file
            shutil.copyfile(gene_dir + '/' + filein, store_dir + '/' + fileout)
        os.remove('tmp_madevent.dat')






