#include "util.h"

QTableWidgetItem*   int2tableItem(unsigned int x)
{
  QString s = QString::number(x);
  auto p = new QTableWidgetItem(s);
  return p;
}

QTableWidgetItem* int2ip(unsigned int x)
{
  char temp[20];
  unsigned short ip[4];
  for (int i = 0; i < 4; i++)
    {
        ip[i] = x&0xff;
        x>>=8;
    }
  sprintf(temp, "%hu.%hu.%hu.%hu", ip[0], ip[1], ip[2], ip[3]);
  QString s = QString(temp);
  auto p = new QTableWidgetItem(s);
  return p;
}


