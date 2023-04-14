#!/bin/python3

# Script for running the srsENB

# base_srate MUST be multiple of 3.84...
# default → 23.04e6
# 3.84, 7.68, 11.52, 15.36, 19.2
# 15PRBs -> 3.84
# 25PRBs -> 7.68
# 50PRBs -> 15.26
# 75PRBs -> 23.04
#100PRBs -> 30.72

# rf format → tx_port=tcp://<local_ip or *>:2000, rx_port=tcp://<dst_ip or localhost>:2001

import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-c', '--config_file', help='Set the path to the config file', type=str, default='./configFiles/ue.conf')
parser.add_argument('-n', '--namespace', help='Set the user namespace', type=str, default='ue1')
# parser.add_argument('-br', '--base_rate', help='Set radio base sampling rate', type=float, default=3.84e6)
parser.add_argument('-np', '--num_prbs', help='Set the number of PRBS of the base station. It is easier than setting the sampling rate', type=int, default=15)

parser.add_argument('-la', '--local_addr', help='Set the radio zmq local address', type=str, default='*')
parser.add_argument('-lp', '--local_port', help='Set the radio zmq local port', type=int, default=2001)
parser.add_argument('-ra', '--remote_addr', help='Set the radio zmq remote address', type=str, default='localhost')
parser.add_argument('-rp', '--remote_port', help='Set the radio zmq remote port', type=int, default=2000)

if __name__ == '__main__':
  args = parser.parse_args()
  cnfPath = args.config_file
  lAddr = args.local_addr
  lPort = args.local_port
  rAddr = args.remote_addr
  rPort = args.remote_port
  # bRate = args.base_rate
  namespace = args.namespace
  prbs = args.num_prbs

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

  cmd = f'sudo ip netns add {namespace}'
  print(cmd)
  os.system(cmd)
  devArgs = f'"fail_on_disconnect=true,tx_port=tcp://{lAddr}:{lPort},rx_port=tcp://{rAddr}:{rPort},id=enb,base_srate={bRate}"'
  cmd = f'sudo ./build/srsue/src/srsue {cnfPath} --rf.device_name=zmq --rf.device_args={devArgs} --gw.netns="{namespace}"'
  print(cmd)
  os.system(cmd)

