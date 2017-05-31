#ifndef MATHCLASS_H
#define MATHCLASS_H

#include <QObject>



class MathClass : public QObject
{
    Q_OBJECT
public:

    explicit MathClass(QObject *parent = 0);

    //FFT
    static QVector<double> fullFftProcess(QVector<double> &signal);
    static std::vector<double> calculateFFT(std::vector<double> &signal);

    //filtering
    static QVector<double> fullBlackManFiltering(QVector<double> &signal, int windowSize);
    static std::vector<double> convolution(std::vector<double> g, std::vector<double> f);
    static QVector<double> straight(QVector<double> &signal);
    static QVector<double> deleteConst(QVector<double> &signal);

    //spline
    static void getQspline(QVector<double> &signal, QVector<double> &x, QVector<double> &new_signal, QVector<double> &new_x,  double step, double thrLow, double thrHigh);
    static void getSpline(std::vector<double> &signal, std::vector<double> &x,std::vector<double> &new_signal, std::vector<double> &new_x,  double step, double thrLow, double thrHigh);

    //polynomial
    static QVector<double> getQpolynom(QVector<double> &dataDep, QVector<double> &dataIndep, int order);

    //maximums
    static double getFourierMaximum(QVector<double> &data);
    static QVector<double> getMaxesCoordinates(QVector<double> &data);

signals:

public slots:
};

#endif // MATHCLASS_H
