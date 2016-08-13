#include "database.h"
#include <QApplication>
#include <QDebug>
#include <QSqlError>

Database::Database(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE", "connection");
    db.setDatabaseName("db");
    db.setHostName("local");
    db.setUserName("user");
    db.setPassword("123");
    db.open();
    QSqlQuery query(db);
    query.exec("CREATE TABLE events"
            "("
            "ID INTEGER PRIMARY KEY,"
            "day INTEGER,"
            "month INTEGER,"
            "year INTEGER,"
            "theme VARCHAR(500),"
            "sdesc VARCHAR(500),"
            "ldesc VARCHAR(500),"
            "place VARCHAR(500),"
            "source VARCHAR(500),"
            "extra VARCHAR(500)"
            ");");
    query.exec("CREATE TABLE settings"
            "("
            "path VARCHAR(500),"
            "quality INTEGER,"
            "font INTEGER,"
            "anniver INTEGER,"
            "h1 INTEGER,"
            "h2 INTEGER,"
            "h3 INTEGER,"
            "h4 INTEGER,"
            "h5 INTEGER"
            ");");
    query.exec("SELECT * FROM settings");
    if(!query.first())
    {
        QString string = "INSERT INTO settings (path,quality,font,anniver,h1,h2,h3,h4,h5) VALUES ('%1','%2','%3','%4','%5','%6','%7','%8','%9')";
        QString q = string.arg(QApplication::applicationDirPath() + "/images").arg(50).arg(11).arg(0).arg("270").arg("270").arg("270").arg("270").arg("270");
        query.exec(q);
    }
    query.finish();
}

Database::~Database()
{
    QSqlQuery query(db);
    query.exec("CREATE TABLE events_sorted"
           "("
           "ID INTEGER PRIMARY KEY,"
           "day INTEGER,"
           "month INTEGER,"
           "year INTEGER,"
           "theme VARCHAR(500),"
           "sdesc VARCHAR(500),"
           "ldesc VARCHAR(500),"
           "place VARCHAR(500),"
           "source VARCHAR(500),"
           "extra VARCHAR(500)"
           ");");
    QString str = "INSERT INTO events_sorted (day,month,year,theme,sdesc,ldesc,place,source,extra) SELECT day,month,year,theme,sdesc,ldesc,place,source,extra FROM events ORDER BY year,month,day;";
    query.exec(str);
    query.finish();
    query.exec("DROP TABLE events;");
    query.exec("ALTER TABLE events_sorted RENAME TO events;");
    db.close();
}

int Database::getData(QVector<int>& id, QVector<int>& days, QVector<int>& months, QVector<int>& years, QVector<QString>& themes, QVector<QString>& sDescriptions, QVector<QString>& lDescriptions, QVector<QString>& places, QVector<QString>& sources, QVector<QString>& extra)
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM events");
    QSqlRecord rec = query.record();
    int size = 0;
    while(query.next())
    {
        id.push_back(query.value(rec.indexOf("ID")).toInt());
        days.push_back(query.value(rec.indexOf("day")).toInt());
        months.push_back(query.value(rec.indexOf("month")).toInt());
        years.push_back(query.value(rec.indexOf("year")).toInt());
        themes.push_back(query.value(rec.indexOf("theme")).toString());
        sDescriptions.push_back(query.value(rec.indexOf("sdesc")).toString());
        lDescriptions.push_back(query.value(rec.indexOf("ldesc")).toString());
        places.push_back(query.value(rec.indexOf("place")).toString());
        sources.push_back(query.value(rec.indexOf("source")).toString());
        extra.push_back(query.value(rec.indexOf("extra")).toString());
        size++;
    }
    query.finish();
    return size;
}

// получить настройки
void Database::getSettings(QString& path, int& quality, int& anniver, int& font)
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM settings");
    QSqlRecord rec = query.record();
    while(query.next())
    {
        path = query.value(rec.indexOf("path")).toString();
        quality = query.value(rec.indexOf("quality")).toInt();
        anniver = query.value(rec.indexOf("anniver")).toInt();
        font = query.value(rec.indexOf("font")).toInt();
    }
    query.finish();
}

void Database::updateSettings(QString path, int quality, int anniver)
{
    QString string = "UPDATE settings SET path='%1',quality='%2',anniver='%3'";
    QString query = string.arg(path).arg(quality).arg(anniver);
    db.exec(query);
}

void Database::updateFont(int font)
{
    QString string = "UPDATE settings SET font='%1'";
    QString query = string.arg(font);
    db.exec(query);
}

int Database::insertEvent(const QVector<QString>& data)
{
    QString string = "INSERT INTO events (day,month,year,theme,sdesc,ldesc,place,source,extra) VALUES ('%1','%2','%3','%4','%5','%6','%7','%8','%9')";
    QString queryInsert = string.arg(data[0]).arg(data[1]).arg(data[2]).arg(data[3]).arg(data[4]).arg(data[5]).arg(data[6]).arg(data[7]).arg(data[8]);
    db.exec(queryInsert);
    QString queryID = "SELECT * FROM events ORDER BY ID DESC LIMIT 1";
    QSqlQuery query(db);
    query.exec(queryID);
    QSqlRecord rec = query.record();
    int id;
    while(query.next())
        id = query.value(rec.indexOf("ID")).toInt();
    qDebug() << "INSERTED ID: " << id;
    return id;
}

void Database::updateEvent(int id, const QVector<QString>& data)
{
    QString string = "UPDATE events SET day='%1',month='%2',year='%3',theme='%4',sdesc='%5',ldesc='%6',place='%7',source='%8',extra='%9' WHERE ID='%10'";
    QString query = string.arg(data[0]).arg(data[1]).arg(data[2]).arg(data[3]).arg(data[4]).arg(data[5]).arg(data[6]).arg(data[7]).arg(data[8]).arg(id);
    db.exec(query);
}

void Database::removeEvent(int id)
{
    QString string = "DELETE FROM events WHERE ID='%1'";
    QString query = string.arg(id);
    db.exec(query);
}
