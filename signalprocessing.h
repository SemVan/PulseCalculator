#ifndef SIGNALPROCESSING_H
#define SIGNALPROCESSING_H

#include <QObject>
#include <QVector>

class SignalProcessing : public QObject
{
    Q_OBJECT
public:
    explicit SignalProcessing(QObject *parent = 0);

    QVector<double> waveLengthVector;

    double fullSignalProcedure(QVector<double> signal);
    void setCoefficients(QVector<double> coefs);

    void setZero(QVector<double> signal);
    void setNorm(QVector<double> signal);

private:
    QVector<double> zero;
    QVector<double> norm;

    QVector<double> polynom;
    QVector<double> getKwaveVector(int size);
    QVector<double> normalize(QVector<double> signal);

    QVector<double> deleteZero(QVector<double> signal);
    QVector<double> devideNorm(QVector<double> signal);

    QVector<double> getLines(QVector<double> signal);
    QVector<double> getWaveLengthFromKvectors(QVector<double> kWaves);


signals:
    void sendIntepolatedSignal(QVector<double> signal, QVector<double> coords);
    void sendInterFourierResuls(QVector<double> fft, QVector<double> coords);
    void brokenPolynom();
    void sendPolyNom(QVector<double> signal, QVector<double> coords);
public slots:
};

#endif // SIGNALPROCESSING_H
