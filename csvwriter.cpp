#include "csvwriter.h"
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QString>

CSVwriter::CSVwriter()
{

}

void CSVwriter::writeSignalToFile(QString fileName, QVector<double> signal) {
    QFile fileToWrite(fileName);

    if (fileToWrite.open(QFile::WriteOnly) || signal.size()!=0) {
          QTextStream output(&fileToWrite);

          for (QVector<double>::iterator record=signal.begin(); record<signal.end(); ++record) {
              output << QString::number(*record,'f',0) << "\n";
          }

          fileToWrite.close();
    }

}

QVector<double> CSVwriter::readFile(QString fileName) {
    QFile fileToRead(fileName);
    QVector<double> result;
    if (fileToRead.open(QFile::ReadOnly)) {
          QTextStream output(&fileToRead);

          while (!output.atEnd()) {
              QString oneLine = output.readLine();

              QStringList list = oneLine.split(" ");
              QString  res = list[1];
              bool ok;
              result.append(res.toDouble(&ok));
          }

          fileToRead.close();
    }
    return result;
}
