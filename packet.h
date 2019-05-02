#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <tins/tins.h>
#ifndef PACKET_H
#define PACKET_H

#define PACKET_LINE_W 32



class packet
{
public:
    Tins::PDU * pdu;
    int id;
    char* hex_content;
    char* raw_content;
    packet(Tins::PDU *pdu_ptr, int id);
    void parse_hex_content();
    ~packet()
    {
      if (hex_content)
        free(hex_content);
    }
};
#endif // PACKET_H
