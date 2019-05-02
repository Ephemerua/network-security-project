#include "sniffer.h"
#include "mainwindow.h"
#include <iostream>

Tins::Sniffer* sniffer = NULL;
std::vector<packet> packets;
extern int nums;
bool SNIFFER_CONTINUE = true;

bool packet_h(Tins::PDU* pp)
{
  packet* p = new packet(pp, nums);
  packets.push_back(*p);
  w_ptr->insert_packet(p);
  w_ptr->update();

  return SNIFFER_CONTINUE;
}


