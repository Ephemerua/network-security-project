#ifndef SNIFFER_H
#define SNIFFER_H


#include <tins/tins.h>
#include <vector>
#include "packet.h"
extern Tins::Sniffer* sniffer;
extern std::vector<packet> packets;
extern int nums;
extern bool SNIFFER_CONTINUE;
bool packet_h(Tins::PDU* pp);

#endif // SNIFFER_H
