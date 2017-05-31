#include "signalprocessing.h"
#include "mathclass.h"
#include <algorithm>
#include <QDebug>

SignalProcessing::SignalProcessing(QObject *parent) : QObject(parent)
{
    zero.clear();
    norm.clear();
}


void SignalProcessing::setCoefficients(QVector<double> coefs) {
    polynom = coefs;
}

/*!
 * \brief SignalProcessing::fullSignalProcedure All signal operations
 * \param signal raw-signal data
 * \return calculated thickness
 */
double SignalProcessing::fullSignalProcedure(QVector<double> signal) {
    int fft_size =2048;

    QVector<double> filteredSignal = MathClass::fullBlackManFiltering(signal,7);




    QVector<double> fourier = MathClass::fullFftProcess(filteredSignal);
    fourier = fourier.mid(0,fourier.size()/2);
    QVector<double> coords;

    //get frequency coords
    /*!for (int i=0; i<fourier.size();i++) {
        double thick = i/(fft_size*step);//optical length of sinus in nm
        thick = thick/1000;//the same in mkm
        thick = thick/2;//two way
        thick = thick*3.14/2;//fuck knows why
        coords.append(thick);
    }*/

    //get interpolated fouruier
    fourier = normalize(fourier);

    double start = coords.at(0);
    double stop = coords.at(coords.size()-1);
    double step = (stop - start) / (fft_size*8);
    QVector<double> interpolatedFourier;
    QVector<double> interpolatedFFTcoords;
    MathClass::getQspline(fourier, coords,interpolatedFourier, interpolatedFFTcoords, step, start, stop);
    sendInterFourierResuls(interpolatedFourier, interpolatedFFTcoords);
    //calculate thickness
    double result = coords.at(MathClass::getFourierMaximum(fourier));
    qDebug()<<result;
    return result;
}

/*!
 * \brief SignalProcessing::getKwaveVector Calculates vector of wave-vectors (1/wavelength)
 * \param size siae of final vector
 * \return qvecotr of wave_vectors
 */
QVector<double> SignalProcessing::getKwaveVector(int size) {
    QVector<double> kWave;

    for (int i=0; i<size; i++) {
        double waveLength = 0;
        for (int j = 0; j<polynom.size(); j++) {
            waveLength+= polynom.at(j) * std::pow(i,j);
        }
        kWave.append( 1/waveLength);
    }
    return kWave;
}

QVector<double> SignalProcessing::getWaveLengthFromKvectors(QVector<double> kWaves) {
    QVector<double> result;

    for (QVector<double>::iterator item=kWaves.begin(); item<kWaves.end(); ++item) {
        result.append(1/(*item));
    }
    return result;
}

/*!
 * \brief SignalProcessing::normalize Normalizes signal vector (signal[i] = max(signal)/signal[i])
 * \param signal
 * \return vector of normalized signal
 */
QVector<double> SignalProcessing::normalize(QVector<double> signal) {
    double max = *std::max_element(signal.begin(), signal.end());
    QVector<double> normalized;

    for (int i = 0; i<signal.size(); i++) {
        normalized.append(signal.at(i) / max);
    }
    return normalized;

}


void SignalProcessing::setZero(QVector<double> signal) {
    zero = signal;
}

void SignalProcessing::setNorm(QVector<double> signal) {
    norm = signal;
}

QVector<double> SignalProcessing::deleteZero(QVector<double> signal) {
    QVector<double> result;

    for (int i=0; i<signal.size();i++) {
        result.append(signal[i] - zero[i]);
    }

    return result;

}

QVector<double> SignalProcessing::devideNorm(QVector<double> signal) {
    QVector<double> result;

    for (int i=0; i<signal.size();i++) {
        double devider = norm[i] -zero[i];

        if (devider<=0){
            devider = 1;
        }

        result.append(signal[i]/devider);
    }

    return result;
}

QVector<double> SignalProcessing::getLines(QVector<double> signal) {
    QVector<double> abs_signal;
    for (int i=0; i<signal.size();i++) {
        abs_signal.append(fabs(signal[i]));
    }

    QVector<double> maxCoords = MathClass::getMaxesCoordinates(abs_signal);
    QVector<double> max_trend;
    QVector<double> max_coords;
    QVector<double> min_trend;
    QVector<double> min_coords;

    for (int i = 0; i<maxCoords.size(); i++) {
        if (signal.at(maxCoords.at(i))>signal.at(maxCoords.at(i)+1)) {
            max_trend.append(signal.at(maxCoords.at(i)));
            max_coords.append(maxCoords.at(i));
        } else {
            min_trend.append(signal.at(maxCoords.at(i)));
            min_coords.append(maxCoords.at(i));
        }
    }

    QVector<double> max_poly = MathClass::getQpolynom(max_coords,max_trend,4);
    QVector<double> min_poly = MathClass::getQpolynom(min_coords,min_trend,4);


    QVector<double> trend;

    for (int i=0; i<signal.size();i++) {
        double max = 0;
        double min =0;
        for (int j=0; j<max_poly.size();j++) {
            max+=pow(i,j)*max_poly.at(j);
            min+=pow(i,j)*min_poly.at(j);
        }
        trend.append(max);
    }
    return trend;
}
