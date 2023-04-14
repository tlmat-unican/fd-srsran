#!/bin/python3

# Script for running the srsEPC

import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-c', '--config_file', help='Set the path to the config file', type=str, default='./configFiles/epc.conf')
parser.add_argument('-mba', '--mme_bind_address', help='Set MME bind addresss to listen to S1 ', type=str, default='127.0.1.100')
parser.add_argument('-gba', '--sgw_bind_address', help='Set Serving gateway bind addresss to listen to GTPU ', type=str, default='127.0.1.100')

if __name__ == '__main__':
  args = parser.parse_args()
  cnfPath = args.config_file
  mmeBa = args.mme_bind_address
  sgwBa = args.sgw_bind_address
  cmd = f'sudo ./build/srsepc/src/srsepc {cnfPath} --mme.mme_bind_addr={mmeBa} --spgw.gtpu_bind_addr={sgwBa}'
  print(cmd)
  os.system(cmd)
