#include "packet.h"


packet::packet(Tins::PDU *pdu_ptr, int id)
{
    this->pdu = pdu_ptr->clone();
    this->hex_content = NULL;
    this->id = id;
    unsigned int length = pdu_ptr->size();

}


void string_parse(char* &parsed_content, char* content, int size)
{
    parsed_content = (char*)malloc(sizeof(char)*size);
    if(!parsed_content)
    {
        printf("malloc failed\n");
        return;
    }
    memcpy(parsed_content ,content, size);
    for(int i = 0; i < size; i++)
    {
        if(parsed_content[i]<0x20 || parsed_content[i]>0x7e)
            parsed_content[i]='.';
    }
}



//char* format_content(int is_hex = 1)
//{
//    char *fm_ptr = (char*)malloc(sizeof(char)*size*(is_hex+2));
//    char* dest = 0, *src;

//    if(!is_hex) goto nohex;

//    for(int i = 0; i < size; i += PACKET_LINE_W)
//    {
//        for(int j = 0; j < PACKET_LINE_W; j++)
//        {
//            dest = fm_ptr + (PACKET_LINE_W+1) * i + j;
//            src = content + (PACKET_LINE_W+1) * i + j;
//            sprintf(dest, "%2x", *src);
//        }
//        sprintf(dest+1,"\n");
//    }
//nohex:
//    for(int i = 0; i < size; i += PACKET_LINE_W)
//    {
//        for(int j = 0; j < PACKET_LINE_W; j++)
//        {
//            dest = fm_ptr + (PACKET_LINE_W+1) * i + j;
//            src = content + (PACKET_LINE_W+1) * i + j;
//            sprintf(dest, "%c", *src);
//        }
//        sprintf(dest+1,"\n");
//    }

//    return fm_ptr;



//}
