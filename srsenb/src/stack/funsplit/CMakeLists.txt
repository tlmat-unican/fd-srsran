#
# Copyright 2013-2021 Software Radio Systems Limited
#
# This file is part of srsRAN
#
# srsRAN is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# srsRAN is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# A copy of the GNU Affero General Public License can be found in
# the LICENSE file in the top-level directory of this distribution
# and at http://www.gnu.org/licenses/.
#
set(SOURCES 
        fs_socket.cc
        FsClientPool.cc
        FsServer.cc
        log.cc
        gtpu_rrc_wrapper.cc
        pdcp_rlc_wrapper.cc
        pdcp_rrc_wrapper.cc
        rlc_pdcp_wrapper.cc
        rlc_rrc_wrapper.cc
        rlc_mac_wrapper.cc
        rrc_phy_wrapper.cc
        rrc_pdcp_wrapper.cc
        rrc_mac_wrapper.cc
        rrc_rlc_wrapper.cc
        rrc_gtpu_wrapper.cc
        rrc_phy_wrapper.cc
        mac_common.cc
        rrc_common.cc
        mac_rrc_wrapper.cc
        mac_rlc_wrapper.cc
        mac_phy_wrapper.cc
        phy_mac_wrapper.cc
        phy_rrc_wrapper.cc
        stack_phy_proxy.cc
        phy_stack_proxy.cc
        )
add_library(fun_split STATIC ${SOURCES})
