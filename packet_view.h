#ifndef PACKET_VIEW_H
#define PACKET_VIEW_H



#include <QItemSelection>
#include <QTabWidget>

QT_BEGIN_NAMESPACE


class packet_table : public QTabWidget
{
    Q_OBJECT

public:
    packet_table(QWidget *parent = 0);


public slots:
    void showAddEntryDialog();
    void addEntry(QString name, QString address);
    void editEntry();
    void removeEntry();

signals:
    void selectionChanged (const QItemSelection &selected);

private:
    void setupTabs();


};

#endif // PACKET_VIEW_H
