#ifndef RLC_PROTO_H
#define RLC_PROTO_H



namespace srsenb {

class rlc;
class rlc_proto {
public:
  rlc_proto();
  ~rlc_proto();
  
  void ProtoizeRlc (srsenb::rlc* item);
private:

};


}
#endif /* RLC_PROTO_H */

