#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include "qcustomplot.h"
#include "signalprocessing.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private:
    Ui::MainWindow *ui;
    static QVector<double> generator(int stop_pnt);
    QElapsedTimer timer;
    QVector<QCPItemText*> neonWaveLength;

    QVector<double> rawSignal;

    void clearNeonWaveVector();
    void setGraphicsForForm();
    void setGraphicsForPlots(QCustomPlot *customPlot);

    SignalProcessing *processor;

    void createMembers();
    void drawNeonSpectrum(QVector<double> spectrum);
    bool checkMeasureCommand(QString command);
    QCPBars *bars1;

    void openFile();

    bool wasZero;
    bool wasNorm;
    bool wasMeasure;
    void sendZero();
    void sendNorm();

private slots:

    void getInterpolatedSignal(QVector<double> signal, QVector<double> coords);

    void getInterFourierResult(QVector<double> fft, QVector<double> coords);

    void getNeonMaxes(QVector<double> coords,QVector<double> y_points, QVector<double> waveLength);


    void on_pushButton_2_clicked();

    void polynomError();
    void neonTableError();


    void showPolyNom(QVector<double> poly, QVector<double> coords);
};

#endif // MAINWINDOW_H
