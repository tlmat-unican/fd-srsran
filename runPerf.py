#!/bin/python3

import time
import os
import argparse
from subprocess import check_output
import subprocess as sp

logTime = 60

def WaitFunc (): 
  print('Waiting in a new thread')
  time.sleep(logTime)
  print('Killing them all!!!')
  # os.system('sudo killall perf')
  # os.system('sudo killall iperf3 > /dev/null 2>&1')
  # os.system('sudo killall srsenb > /dev/null 2>&1')
  # os.system('sudo killall srsepc > /dev/null 2>&1')

parser = argparse.ArgumentParser()

parser.add_argument('-so', '--split_option', help='Set the split option by name', type=str)
parser.add_argument('-br', '--bit_rate', help='Set sending bit rate', type=int)
parser.add_argument('-prb', '--prb', help='Set sending bit rate', type=int)
parser.add_argument('-a', '--address', help='Set sending bit rate', type=int)


if __name__ == '__main__':
  args = parser.parse_args()
  split = args.split_option
  prb = args.prb
  addr = args.address
  brs = []
  res_folder = 'results_cc3'
  entity= 'CU'
  if prb == 15:
    brs = [0, 2, 4, 6, 8, 10]
  elif prb == 25:
    brs = [0,10, 20, 30, 40, 50]
  else:
    brs = [0,20,40,60,80, 100]
    # brs = [80, 100]

  for br in brs:

    print(f'Bit rate {br}')
    pid = int(check_output(['pidof','-s', 'srsenb']).split()[0])
    print(pid)
        
    if br > 0:
      cmdIperf3 = f'iperf3 -c 172.16.0.{addr} -b {br}M -u -n 1000M'
      print(cmdIperf3)
      sp.Popen([cmdIperf3], start_new_session=True, shell=True)
    time.sleep(1)
    # ===========================================================
    os.system ('sudo killall top')
    os.system ('sudo killall perf')
    # ------------------------------------------------------------
    perfFile = f'{res_folder}/perf_{entity}_{split}_PRB_{prb}_BR_{br}_2.txt'
    cmdPerf = f'sudo perf stat -p {pid} -e cycles,instructions,cpu-clock -o {perfFile}'
    sp.Popen(cmdPerf, start_new_session=True, shell=True)
    # ------------------------------------------------------------
    topFile = f'{res_folder}/top_{entity}_{split}_PRB_{prb}_BR_{br}_2.txt'
    os.system(f'rm -f {topFile}')
    cmdTop = f'top -b -d 0.1 -p {pid} | ts "%.s" > {topFile}'
    sp.Popen(cmdTop, start_new_session=True, shell=True)
    # ------------------------------------------------------------
    WaitFunc()
    pidPerf = int(check_output(['pidof','-s', 'perf']).split()[0])
    os.system(f'kill -2 {pidPerf}')

    os.system('sudo killall iperf3 > /dev/null 2>&1')
    os.system('sudo killall top > /dev/null 2>&1')
    # ===========================================================
    os.system('clear')
    time.sleep(1)
