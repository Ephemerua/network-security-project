#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMessageBox>
#include <QMainWindow>
#include <tins/tins.h>
#include "packet.h"
#include "sniffer.h"
#include "util.h"
#include <pdu.h>

extern Tins::Sniffer* sniffer;
extern std::vector<packet> packets;
extern int nums;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void insert_packet(packet* p);

    Tins::Sniffer* sniffer;
    void warn()
    {
      QMessageBox::warning(
      this,
      tr("Application Name"),
      tr("A warning message.") );
    }


public slots:
    void setFilter();
    void start_capture_wrapper();
    void stop_capture();
    void show_raw(int a, int b);
    void show_hex(int a, int b);

private:
    Ui::MainWindow *ui;
};

extern MainWindow* w_ptr;
void start_capture(const char* interface, const char* filter );
#endif // MAINWINDOW_H
