#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <QObject>
#include <QFile>

class CSVwriter
{

public:
    explicit CSVwriter();
    static void writeSignalToFile(QString fileName, QVector<double> signal);
    static QVector<double> readFile(QString fileName);

signals:

public slots:
};

#endif // CSVWRITER_H
