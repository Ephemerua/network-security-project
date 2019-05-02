#ifndef TABLEMODEL_H
#define TABLEMODEL_H


#include <QAbstractTableModel>
#include <QList>
#include "packet.h"
//! [0]



inline QDataStream &operator<<(QDataStream &stream, const packet *p_line)
{
    return stream << p_line->content;
}

inline QDataStream &operator>>(QDataStream &stream, packet* p_line)
{
    return stream >> p_line->content;
}

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TableModel(QObject *parent = 0);
    TableModel(QList<packet> packets, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    QList<packet> getPacketLine() const;
private:
    QList<packet> packets;
};
//! [0]

#endif // TABLEMODEL_H


