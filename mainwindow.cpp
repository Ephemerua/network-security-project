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
static inline QTableWidgetItem*   int2tableItem(unsigned int x)
{
  QString s = QString::number(x);
  auto p = new QTableWidgetItem(s);
  return p;
}

void packet_table_init(Ui::MainWindow *ui);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sniffer = 0;
    setWindowTitle(tr("my sniffer"));
    packet_table_init(ui);
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

void packet_table_init(Ui::MainWindow *ui)
{
  ui->packet_table->setRowCount(0);
  ui->packet_table->setColumnCount(6);
  ui->packet_table->setHorizontalHeaderLabels(QStringList() << QObject::tr("序号")
                                              << QObject::tr("源MAC地址") << QObject::tr("目的MAC地址")
                                               << QObject::tr("协议类型")
                                              << QObject::tr("源IP地址") << QObject::tr("目的IP地址"));

}

void MainWindow::insert_packet(packet* p)
{
   QTableWidget* table = ui->packet_table;
   int row_id = table->rowCount();
   ui->packet_table->insertRow(row_id);
   ui->packet_table->setItem(row_id, 0, int2tableItem(table->rowCount()-1));
   table->setItem(row_id, 1, int2tableItem(p->id));
   uint32_t s = p->pdu->rfind_pdu<Tins::IP>().src_addr();
   table->setItem(row_id, 2, int2tableItem(s));
   s = p->pdu->rfind_pdu<Tins::IP>().dst_addr();
   table->setItem(row_id, 3, int2tableItem(s));
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



