#include "srsenb/hdr/stack/upper/rlc_proto.h"
#include "srsenb/hdr/stack/upper/rlc.h"

#include <iostream>

namespace srsenb {

rlc_proto::rlc_proto() {
}

rlc_proto::~rlc_proto() {
}

void
rlc_proto::ProtoizeRlc(srsenb::rlc* item) {
  std::cout << "Printing users "<< std::endl;
  for (auto& it : item->users) {
    std::cout << it.first << std::endl;
  }

}

}