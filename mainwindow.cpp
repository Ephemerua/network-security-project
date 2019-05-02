#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
#include "packet.h"
#include <QString>
#include <iostream>
#include <pthread.h>
#include <QMessageBox>
int nums = 0;
pthread_t pid;


void packet_table_init(Ui::MainWindow *ui);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sniffer = 0;
    setWindowTitle(tr("my sniffer"));
    packet_table_init(ui);

    ui->packet_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->packet_table->setSelectionMode(QAbstractItemView::SingleSelection);


    connect(ui->packet_table, SIGNAL(cellClicked(int, int)), this, SLOT(show_raw(int, int)));
    connect(ui->packet_table, SIGNAL(cellClicked(int, int)), this, SLOT(show_hex(int, int)));
    connect(ui->filter, SIGNAL(released()), this, SLOT(setFilter()));
    connect(ui->start, SIGNAL(released()), this, SLOT(start_capture_wrapper()));
    connect(ui->stop, SIGNAL(released()), this, SLOT(stop_capture()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setFilter()
{
    QString filter = ui->filter_edit->text();
    std::cout<< filter.toStdString();
    if(sniffer)
        sniffer->set_filter(filter.toStdString().c_str());
    //start_capture("en0", filter.toStdString().c_str());

}

void MainWindow::show_raw(int a, int b)
{
    std::cout<<"called"<<std::endl;
    packet *p = &packets[a];

    std::cout<<p->pdu->rfind_pdu<Tins::IP>().src_addr();
    std::vector<uint8_t> s = p->pdu->serialize();
    std::string str;
    for(auto i : s)
      {
          if(i >= 0x20 && i <= 0x7e)
            str += i;
          else
            str +='.';
      }

    ui->raw_table->setText(QString::fromStdString(str));
    w_ptr->update();
}

void MainWindow::show_hex(int a, int b)
{
    std::cout<<"called"<<std::endl;
    packet *p = &packets[a];

    std::cout<<p->pdu->rfind_pdu<Tins::IP>().src_addr();
    std::vector<uint8_t> s = p->pdu->serialize();
    std::string str;
    int num = 0;
    for(auto i : s)
      {
          char temp[4];
          sprintf(temp, "%02X  ", i);
          str += temp;
          num++;
          if(num == 16)
            {
              str+="\n";
              num = 0;
            }
      }

    ui->format_table->setText(QString::fromStdString(str));
    w_ptr->update();
}

void packet_table_init(Ui::MainWindow *ui)
{
  ui->packet_table->setRowCount(0);
  ui->packet_table->setColumnCount(7);
  ui->packet_table->setHorizontalHeaderLabels(QStringList() << QObject::tr("序号")
                                              << QObject::tr("源MAC地址") << QObject::tr("目的MAC地址")
                                              << QObject::tr("帧类型")
                                              << QObject::tr("源IP地址") << QObject::tr("目的IP地址")
                                              << QObject::tr("协议类型"));

}

void MainWindow::insert_packet(packet* p)
{
   QTableWidget* table = ui->packet_table;
   int row_id = table->rowCount();
   QTableWidgetItem *item_p = NULL;
   uint32_t s = 0;
   QString hw;
   switch(p->pdu->inner_pdu()->pdu_type())
     {
        case Tins::PDU::ARP:
          ui->packet_table->insertRow(row_id);
          ui->packet_table->setItem(row_id, 0, int2tableItem(table->rowCount()-1));
          hw = QString::fromStdString(p->pdu->rfind_pdu<Tins::ARP>().sender_hw_addr().to_string());
          item_p = new QTableWidgetItem(hw);
          table->setItem(row_id, 2, item_p);
          delete item_p;
        break;
        case Tins::PDU::IP:
          ui->packet_table->insertRow(row_id);
          ui->packet_table->setItem(row_id, 0, int2tableItem(table->rowCount()-1));
          s = p->pdu->rfind_pdu<Tins::IP>().src_addr();
          item_p = int2ip(s);
          table->setItem(row_id, 4, item_p);
          //delete item_p;
          s = p->pdu->rfind_pdu<Tins::IP>().dst_addr();
          item_p = int2ip(s);
          table->setItem(row_id, 5, item_p);
          //delete item_p;
          item_p = new QTableWidgetItem(QString("IP"));
          table->setItem(row_id, 3, item_p);
          //delete item_p;
          hw = QString::fromStdString(p->pdu->rfind_pdu<Tins::EthernetII>().src_addr().to_string());
          item_p = new QTableWidgetItem(hw);
          table->setItem(row_id, 1, item_p);
          //delete item_p;
          hw = QString::fromStdString(p->pdu->rfind_pdu<Tins::EthernetII>().dst_addr().to_string());
          item_p = new QTableWidgetItem(hw);
          table->setItem(row_id, 2, item_p);

          switch(p->pdu->inner_pdu()->inner_pdu()->pdu_type())
            {
                case Tins::PDU::TCP:
                    item_p = new QTableWidgetItem(QString("TCP"));
                    table->setItem(row_id, 6, item_p);
              break;
            case Tins::PDU::UDP:
                    item_p = new QTableWidgetItem(QString("UDP"));
                    table->setItem(row_id, 6, item_p);
               break;
            default:
              break;

            }


       break;
     default:
       return;

     }

}

void* loop_func(void* sniffer)
{
    //sniffer->sniff_loop(packet_h);
  std::cout<<"start loop"<<std::endl;
    Tins::Sniffer* sp = (Tins::Sniffer*)sniffer;
    while(SNIFFER_CONTINUE)
      {
        if(!sniffer)
          {
            std::cout<<"no sniffer"<<std::endl;
          }

        Tins::Packet p = sp->next_packet();
        if(!p||!p.pdu()->find_pdu<Tins::IP>())
          {
              return NULL;
          }
        Tins::PDU* pdu = p.pdu();
        packet_h(pdu);
      }
    return NULL;
}
void MainWindow::start_capture_wrapper()
{
    SNIFFER_CONTINUE = true;
    QString interface = ui->interfaceEdit->text();
    QString filter = ui->filter_edit->text();
    std::cout<<"start!"<<std::endl;
    start_capture(interface.toStdString().c_str(), filter.toStdString().c_str());
}

void MainWindow::stop_capture()
{
     SNIFFER_CONTINUE = false;
     pthread_kill(pid, NULL);
}
void start_capture(const char* interface, const char* filter )
{

  Tins::SnifferConfiguration config;
  config.set_filter(filter);
  config.set_promisc_mode(true);

  try
  {
      sniffer = new Tins::Sniffer(interface, config);
  }
  catch(...)
  {
      w_ptr->warn();
      sniffer = NULL;
      return;
  }

    pthread_create(&pid, NULL, loop_func, sniffer);
}



