#!/bin/python3

# Script for running the srsENB

# base_srate MUST be multiple of 3.84...
# default → 23.04e6
# 3.84, 7.68, 11.52, 15.36, 19.2
# 15PRBs -> 3.84
# 25PRBs -> 5.76
# 50PRBs -> 15.26
# 75PRBs -> 23.04
#100PRBs -> 30.72

# rf format → tx_port=tcp://<local_ip or *>:2000, rx_port=tcp://<dst_ip or localhost>:2001

import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-c', '--config_file', help='Set the path to the config file', type=str, default='./configFiles/enb.conf')
parser.add_argument('-laa', '--local_access_addr', help='Set the access zmq local address', type=str, default='*')
parser.add_argument('-lap', '--local_access_port', help='Set the access zmq local port', type=int, default=2000)
parser.add_argument('-raa', '--remote_access_addr', help='Set the access zmq remote address', type=str, default='localhost')
parser.add_argument('-rap', '--remote_access_port', help='Set the access zmq remote port', type=int, default=2001)

parser.add_argument('-lfa', '--local_fonthaul_addr', help='Set the fronthaul zmq local address', type=str, default='0.0.0.0')
parser.add_argument('-rfa', '--remote_fonthaul_addr', help='Set the fronthaul zmq remote address', type=str, default='localhost')

parser.add_argument('-so', '--split_option', help='Set the split option by name', type=str, default='NONE')
parser.add_argument('-se', '--split_entity', help='Set the split entity: CU/DU', type=str, default='BOTH')

# parser.add_argument('-br', '--base_rate', help='Set radio base sampling rate', type=float, default=3.84e6)
parser.add_argument('-np', '--num_prbs', help='Set the number of PRBS of the base station', type=int, default=15)

parser.add_argument('-mme', '--mme_address', help='Set MME bind addresss S1 ', type=str, default='127.0.1.100')

parser.add_argument('-gba', '--gtp_bind_address', help='', type=str, default='127.0.1.1')
parser.add_argument('-sba', '--s1_bind_address', help='', type=str, default='127.0.1.1')




if __name__ == '__main__':
  args = parser.parse_args()
  cnfPath = args.config_file
  lAddr = args.local_access_addr
  lPort = args.local_access_port
  rAddr = args.remote_access_addr
  rPort = args.remote_access_port
  lfAddr = args.local_fonthaul_addr
  rfAddr = args.remote_fonthaul_addr
  split = args.split_option
  entity = args.split_entity
  # bRate = args.base_rate
  prbs = args.num_prbs
  mmeAddr = args.mme_address
  gtpBindAddr = args.gtp_bind_address
  sqBindAddr = args.s1_bind_address

  if prbs == 15:
    bRate = 3.84e6
  elif prbs == 25:
    bRate = 5.76e6
  elif prbs == 50:
    bRate = 11.52e6
  elif prbs == 75:
    bRate = 15.36e6
  elif prbs == 100:
    bRate = 23.04e6
  

  prefixPerf = f'perf stat -e cycles,instructions,cpu-clock -o perf_{entity}_{split}.txt'
  devArgs = f'"fail_on_disconnect=true,tx_port=tcp://{lAddr}:{lPort},rx_port=tcp://{rAddr}:{rPort},id=enb,base_srate={bRate}"'
  fsArgs = f'--fs.remote_ip={rfAddr} --fs.local_ip={lfAddr} --fs.type={split} --fs.role={entity}'
  addrs = f'--enb.mme_addr={mmeAddr} --enb.gtp_bind_addr={gtpBindAddr} --enb.s1c_bind_addr={sqBindAddr}'
  cmd = f'sudo ./build/srsenb/src/srsenb {cnfPath} --rf.device_name=zmq --rf.device_args={devArgs} --enb.n_prb={prbs} --log.filename=./logs/{entity}.log {fsArgs} {addrs}'
  print(cmd)
  os.system(cmd)


