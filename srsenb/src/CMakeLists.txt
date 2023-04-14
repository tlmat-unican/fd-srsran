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
add_subdirectory(common)
add_subdirectory(phy)
add_subdirectory(stack)
add_subdirectory(funsplit_proto)


# Link libstdc++ and libgcc
if(BUILD_STATIC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
endif(BUILD_STATIC)

find_package(Protobuf REQUIRED)
set(PROTO_CLASSES ../hdr/funsplit_proto/proto_utils.h
                  ./funsplit_proto/rlc_config_t.pb.cc
                  ./funsplit_proto/pdcp_config_t.pb.cc
                  ./funsplit_proto/as_security_config_t.pb.cc
                  ./funsplit_proto/pdcp_lte_state_t.pb.cc
                  ./funsplit_proto/bearer_props.pb.cc
                  ./funsplit_proto/cell_cfg_t.pb.cc
                  ./funsplit_proto/ue_cfg_t.pb.cc
                  ./funsplit_proto/rrc_mcch_t.pb.cc
                  ./funsplit_proto/phy_mac.pb.cc
                  ./funsplit_proto/phy_rrc_cfg_t.pb.cc
                  ./funsplit_proto/ul_grant.pb.cc
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-array-bounds")

if (RPATH)
  SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
endif (RPATH)

add_library(enb_cfg_parser STATIC parser.cc enb_cfg_parser.cc)
target_link_libraries(enb_cfg_parser ${LIBCONFIGPP_LIBRARIES})

add_executable(srsenb main.cc enb.cc metrics_stdout.cc metrics_csv.cc metrics_json.cc ${PROTO_CLASSES})
add_executable(fsproof main_fsproof.cc)
add_executable(zmqproof main_zmqproof.cc)

set(SRSENB_SOURCES srsenb_phy srsenb_stack srsenb_common srsenb_upper srsenb_mac srsenb_rrc srslog system fun_split)
set(FSPROOF_SOURCES)
set(ZMQPROOF_SOURCES fun_split)
set(SRSRAN_SOURCES srsran_common srsran_mac srsran_phy srsran_upper srsran_radio rrc_asn1 s1ap_asn1 enb_cfg_parser srslog system)

set(SRSENB_SOURCES ${SRSENB_SOURCES} srsgnb_phy srsgnb_stack srsgnb_upper srsgnb_mac srsgnb_rrc)
set(FSPROOF_SOURCES ${FSPROOF_SOURCES})
set(ZMQPROOF_SOURCES ${ZMQPROOF_SOURCES})
set(SRSRAN_SOURCES ${SRSRAN_SOURCES} rrc_nr_asn1 ngap_nr_asn1)

target_link_libraries(srsenb  ${SRSENB_SOURCES}
                              ${SRSRAN_SOURCES}                              
                              ${CMAKE_THREAD_LIBS_INIT}
                              ${Boost_LIBRARIES}
                              ${SEC_LIBRARIES}
                              ${LIBCONFIGPP_LIBRARIES}
                              ${SCTP_LIBRARIES}
                              ${Protobuf_LIBRARIES}
                              )

target_link_libraries(fsproof ${FSPROOF_SOURCES}
                              ${SRSRAN_SOURCES}                              
                              ${CMAKE_THREAD_LIBS_INIT}
                              ${Boost_LIBRARIES}
                              ${SEC_LIBRARIES}
                              ${Protobuf_LIBRARIES}
                              )

target_link_libraries(zmqproof ${ZMQPROOF_SOURCES}
                              ${SRSRAN_SOURCES}                              
                              ${CMAKE_THREAD_LIBS_INIT}
                              ${Boost_LIBRARIES}
                              ${SEC_LIBRARIES}
                              ${Protobuf_LIBRARIES}
                              )

if (RPATH)
  set_target_properties(srsenb PROPERTIES INSTALL_RPATH ".")
endif (RPATH)

########################################################################
# Option to run command after build (useful for remote builds)
########################################################################
if (NOT ${BUILDENB_CMD} STREQUAL "")
  message(STATUS "Added custom post-build-ENB command: ${BUILDENB_CMD}")
  add_custom_command(TARGET srsenb POST_BUILD COMMAND ${BUILDENB_CMD})
else(NOT ${BUILDENB_CMD} STREQUAL "")
  message(STATUS "No post-build-ENB command defined")
endif (NOT ${BUILDENB_CMD} STREQUAL "")

install(TARGETS srsenb DESTINATION ${RUNTIME_DIR})
