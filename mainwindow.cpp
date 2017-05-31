#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mathclass.h"
#include <math.h>
#include <QDebug>
#include <QElapsedTimer>
#include <stdlib.h>
#include <csvwriter.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createMembers();
    setGraphicsForForm();
    rawSignal.clear();

    wasZero = false;
    wasNorm = false;
}

MainWindow::~MainWindow() {
    delete ui;
}


/*!
 * \brief MainWindow::createMembers Creates children class object, connects SIGNALS-SLOTS
 */
void MainWindow::createMembers() {
    /*thinMeter = new protocol();
    QObject::connect(thinMeter, SIGNAL(sendResults(QVector<double>&)),this, SLOT(getData(QVector<double>&)));
    QObject::connect(thinMeter, SIGNAL(comPortFound(int)),this, SLOT(portFound(int)));
    QObject::connect(thinMeter,SIGNAL(ledTurnedOn()),this,SLOT(initMeasure()));
    QObject::connect(thinMeter,SIGNAL(sendAvailablePorts(QList<QSerialPortInfo>)),this,SLOT(getAvailableProts(QList<QSerialPortInfo>)));
    thinMeter->setPort();


    calib = new Calibration();
    QObject::connect(calib, SIGNAL(sendMaximumsOfNeon(QVector<double>,QVector<double>, QVector<double>)),this, SLOT(getNeonMaxes(QVector<double>,QVector<double>, QVector<double>)));
    QObject::connect(calib,SIGNAL(brokenNeonTable()),this,SLOT(neonTableError()));
    QObject::connect(calib,SIGNAL(brokenPolynomFile()),this,SLOT(polynomError()));
    calib->initialization();
    */

    processor = new SignalProcessing();

    QObject::connect(processor, SIGNAL(sendIntepolatedSignal(QVector<double>,QVector<double>)),this, SLOT(getInterpolatedSignal(QVector<double>,QVector<double>)));
    QObject::connect(processor,SIGNAL(sendInterFourierResuls(QVector<double>,QVector<double>)),this,SLOT(getInterFourierResult(QVector<double>,QVector<double>)));
    QObject::connect(processor,SIGNAL(brokenPolynom()),this,SLOT(polynomError()));
    QObject::connect(processor,SIGNAL(sendPolyNom(QVector<double>,QVector<double>)),this,SLOT(showPolyNom(QVector<double>,QVector<double>)));
}

/*!
 * \brief MainWindow::setGraphicsForForm Applies design to application form window
 */
void MainWindow::setGraphicsForForm() {

   this->setStyleSheet("background-color: rgba(90,90,90,255);");


   ui->pushButton_2->setStyleSheet("background-color:rgba(70,70,70,255);"
                                   "color:rgba(200,200,200,255);");


   ui->lcdNumber->setStyleSheet("color: rgba(255,153,0,255);");

   ui->widget_2->addGraph();
   ui->widget_2->addGraph();
   ui->widget->addGraph();
   ui->widget->addGraph();

   setGraphicsForPlots(ui->widget);
   setGraphicsForPlots(ui->widget_2);

   ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);
   ui->widget_2->graph(0)->setLineStyle(QCPGraph::lsNone);
   ui->widget->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
   ui->widget->graph(0)->setPen(QPen(QColor(250,250,0)));
   ui->widget->graph(1)->setPen(QPen(QColor(0,250,250)));


   bars1 = new QCPBars(ui->widget_2->xAxis, ui->widget_2->yAxis);
   bars1->setPen(Qt::NoPen);
   bars1->setBrush(QColor(255, 153, 0, 255));


}


/*!
 * \brief MainWindow::setGraphicsForPlots Applies design to plot areas
 * \param customPlot Graph to be changed
 */
void MainWindow::setGraphicsForPlots(QCustomPlot *customPlot) {
    customPlot->setInteraction(QCP::iRangeDrag,true);
    customPlot->setInteraction(QCP::iRangeZoom,true);
    customPlot->addGraph();

    //customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(QColor(255, 153, 30, 255)), 9));

    customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(30, 30, 30));
    plotGradient.setColorAt(1, QColor(10, 10, 10));
    customPlot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    customPlot->axisRect()->setBackground(axisRectGradient);

    customPlot->repaint();
}






/*!
 * \brief MainWindow::generator Generates vector of sequence of int values starting from 0 and ending with stop_pnt
 * \param stop_pnt last value in vector
 * \return
 */
QVector<double> MainWindow::generator(int stop_pnt) {
    QVector<double> toReturn;
    for (int i=0; i<stop_pnt; i++) {
        toReturn.append((double)i);
    }
    return toReturn;
}




/*!
 * \brief MainWindow::getNeonMaxes Draws neon spectrum spikes on the graph
 * \param coords x-coordinates of spikes
 * \param y_points y-coordinates of spikes
 * \param waveLength wavelength of each spike
 */
void MainWindow::getNeonMaxes(QVector<double> coords, QVector<double> y_points, QVector<double> waveLength) {
    ui->widget->graph(0)->setData(coords, y_points);
    clearNeonWaveVector();
    for (int i=0; i<coords.size(); i++) {
        QCPItemText *waveText = new QCPItemText(ui->widget);
        waveText->setColor(QColor(250,250,250,250));
        waveText->setText(QString::number(waveLength.at(i)));
        waveText->position->setCoords(coords.at(i),y_points.at(i)+50);
        neonWaveLength.append(waveText);
    }
    ui->widget->replot();
}



/*!
 * \brief MainWindow::drawNeonSpectrum Draws neon spectrum
 * \param spectrum
 */
void MainWindow::drawNeonSpectrum(QVector<double> spectrum) {
    ui->widget->graph(1)->setData(generator(spectrum.size()),spectrum);
    ui->widget->xAxis->setRange(0,spectrum.size());
    ui->widget->yAxis->setRange(0,4100);

}

void MainWindow::showPolyNom(QVector<double> poly, QVector<double> coords) {
    ui->widget->graph(0)->setData(coords,poly);
}

/*!
 * \brief MainWindow::clearNeonWaveVector Clears vector of neon wavelenght texts.
 */
void MainWindow::clearNeonWaveVector() {
    for (int i=0; i<neonWaveLength.size(); i++) {
        delete neonWaveLength[i];
    }
    neonWaveLength.clear();
}


/*!
 * \brief MainWindow::getInterpolatedSignal Draws interpolated singal on graph
 * \param signal
 * \param coords
 */
void MainWindow::getInterpolatedSignal(QVector<double> signal, QVector<double> coords) {
    ui->widget->graph(1)->setData(coords,signal);
    ui->widget->xAxis->setRange(coords.at(0),coords.at(coords.size()-1));
    ui->widget->yAxis->setRange(-0.5,0.5);
    ui->widget->replot();
}


/*!
 * \brief MainWindow::getInterFourierResult Draws interpolated fourier on graph
 * \param fft
 * \param coords
 */
void MainWindow::getInterFourierResult(QVector<double> fft, QVector<double> coords) {
    ui->widget_2->xAxis->setRange(0,150);
    ui->widget_2->yAxis->setRange(0,1.3);
    double width = (coords.at(coords.size()-1) - coords.at(0))/(coords.size()*2);
    bars1->setWidth(width);
    bars1->setData(coords,fft);
    ui->widget_2->replot();
}




/*!
 * \brief MainWindow::openFile Calls dialog window to choose/create file. Calls functions to save last signal to file/
 */
void MainWindow::openFile() {
    QString directory = QApplication::applicationDirPath() + "//Data//";
    QDir dataDirectory(directory);

    if (!(dataDirectory.exists())) {
            dataDirectory.mkdir(directory);
    }

    QString fileName = QFileDialog::getOpenFileName(this, "Save to", directory);

    QVector<double> signal = CSVwriter::readFile(fileName);
    ui->widget->graph(0)->setData(generator(signal.size()),signal);
    ui->widget->replot();


}

void MainWindow::on_pushButton_2_clicked()
{
    openFile();
}

/*!
 * \brief MainWindow::polynomError Shows message that smth is wrong with polynom file
 */
void MainWindow::polynomError() {
    QString title = "Error";
    QString mes = "Something is wrong with polynom file";
    QMessageBox::information(this,title,mes);
}

/*!
 * \brief MainWindow::neonTableError Shows meassage thas smth is wrong with neon table file
 */
void MainWindow::neonTableError() {
    QString title = "Error";
    QString mes = "Something is wrong with neon table file";
    QMessageBox::information(this,title,mes);

}


