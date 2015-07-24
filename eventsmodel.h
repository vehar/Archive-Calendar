#ifndef EVENTSMODEL_H
#define EVENTSMODEL_H

#include <QObject>
#include <QtSql>

class EventsModel: public QObject
{
    Q_OBJECT
public:
    friend class NewEvent;
    friend class MainInterface;
    EventsModel();
    void save(int,QString,int,QString,QString,QString,QString,QString,QString,QString);
    void update(int,QString,int,QString,QString,QString,QString,QString,QString,QString,int);
    void getdata();
    void upsettings(QString, int, int);
    void getsettings();
    int getimages(int);
    void del(int);
    int count();
    int imgcount();
    int getmonth(int);
    int getmonth(QString);
private:
    int quality;
    int show;
    QString path;
    int img;
    QSqlDatabase db;
    QSqlQuery *query;
    QSqlRecord rec;
    QVector<int> id;
    QVector<int> day;
    QVector<QString> month;
    QVector<int> year;
    QVector<QString> theme;
    QVector<QString> sdesc;
    QVector<QString> ldesc;
    QVector<QString> place;
    QVector<QString> source;
    QVector<QString> extra;
    QVector<QString> images[100];
    int size;
};

#endif // EVENTSMODEL_H
