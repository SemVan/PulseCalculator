#include "mathclass.h"

#include "aquila/global.h"
#include "aquila/transform/FftFactory.h"
#include <algorithm>
#include <functional>
#include <QVector>
#include <complex.h>
#include "aquila/source/window/BlackmanWindow.h"
#include "spline.h"
#include "polyfit.h"

MathClass::MathClass(QObject *parent) : QObject(parent)
{

}


QVector<double> MathClass::fullFftProcess(QVector<double> &signal) {
    std::vector<double> stdSignal = signal.toStdVector();
    return QVector<double>::fromStdVector(calculateFFT(stdSignal));

}


/*!
 * \brief MathClass::calculateFFT
 * \param signal QVector of doubles
 * \return
 */
std::vector<double> MathClass::calculateFFT(std::vector<double> &signal) {

    std::size_t FFT_SIZE = signal.size();

    double arrSignal[FFT_SIZE];
    std::copy(signal.begin(), signal.end(), arrSignal);

    auto fft = Aquila::FftFactory::getFft(FFT_SIZE);
    Aquila::SpectrumType spectrum = fft->fft(arrSignal);

    std::vector<double> resultSpec;
    for (std::vector<std::complex<double>>::iterator i = spectrum.begin(); i!=spectrum.end(); ++i) {
        resultSpec.push_back(std::abs(*i));
    }

    return resultSpec;
}


/*!
 * \brief MathClass::straight Builds signal approximation. Devides signal by approximation
 * \param signal
 * \return result[i] = signal[i] / approx[i]
 */
QVector<double> MathClass::straight(QVector<double> &signal) {

    QVector<double> coords;
    for (int i=0; i<signal.size();i++) {
        coords.append(i);
    }
    QVector<double> result;
    QVector<double> poly = getQpolynom(coords,signal,2);
    for (int i=0; i<signal.size();i++) {
        double point=0;
        for (int j=0; j<poly.size();j++) {
            point+=pow(i,j)*poly.at(j);
        }
        result.append(signal.at(i) / point);
    }

    return result;
}

/*!
 * \brief MathClass::deleteConst Builds signal approximation. Substracts approximation from signal
 * \param signal
 * \return result[i] = signal[i]  - approx[i]
 */
QVector<double> MathClass::deleteConst(QVector<double> &signal) {
    QVector<double> coords;
    for (int i=0; i<signal.size();i++) {
        coords.append(i);
    }
    QVector<double> result;
    QVector<double> poly = getQpolynom(coords,signal,5);
    for (int i=0; i<signal.size();i++) {
        double point=0;
        for (int j=0; j<poly.size();j++) {
            point+=pow(i,j)*poly.at(j);
        }
        result.append(signal.at(i) - point);
        //result.append(point);
    }

    return result;
}


/*!
 * \brief MathClass::fullBlackManFiltering Full filtering procedure
 * \param signal
 * \param windowSize filter window size
 * \return filtered signal
 */

QVector<double> MathClass::fullBlackManFiltering(QVector<double> &signal, int windowSize) {
    Aquila::BlackmanWindow filter(windowSize);
    std::vector<double> stdFilter;

    for (Aquila::BlackmanWindow::iterator iter = filter.begin(); iter!=filter.end(); ++iter) {
        stdFilter.push_back(*iter);
    }

    std::vector<double> filtered = convolution(signal.toStdVector(), stdFilter);

    return QVector<double>::fromStdVector(filtered);
}


/*!
 * \brief MathClass::convolution Operation of concolution
 * \param g First vector of convolution
 * \param f Second vector of convolution
 * \warning Appends g.size/2 last elements of first vector to first vector.Prepands g.size/2 first elements of first vector to first vector.
 * This in needed not to loose number of elements in resulting vector
 * \return
 */
std::vector<double> MathClass::convolution(std::vector<double> g, std::vector<double> f) {

    std::vector<double> convResult;
    int window = f.size();


    g.insert(g.begin(), window/2, g.at(0));
    g.insert(g.end(), window/2, g.at(g.size()-1));

    for (int i=0; i<=(g.size()-window); i++) {
        double convElem=0;
        for (int j=0; j< window; j++) {
            convElem+=f[j] * g[i+j];
        }
        convResult.push_back(convElem);
    }

    return convResult;
}


/*!
 * \brief MathClass::getQspline Procedes cubic spline interpolation with QVectors
 * \param signal Singal to be interpolated
 * \param x X-coordinates of signal
 * \param new_signal Where to put resultin vecotr
 * \param new_x X-coords of resulting vecotr
 * \param step Step of interpolation
 * \param thrLow X-coordinate of the first point of interpolation
 * \param thrHigh X-coordinates of the last point of interpolation
 */

void MathClass::getQspline(QVector<double> &signal, QVector<double> &x, QVector<double> &new_signal, QVector<double> &new_x, double step, double thrLow, double thrHigh) {
    std::vector<double> sig = signal.toStdVector();
    std::vector<double> stdX = x.toStdVector();
    std::vector<double> new_sig;
    std::vector<double> new_stdX;

    getSpline(sig,stdX,new_sig,new_stdX,step,thrLow,thrHigh);

    new_signal = QVector<double>::fromStdVector(new_sig);
    new_x = QVector<double>::fromStdVector(new_stdX);

    return;

}

/*!
 * \brief MathClass::getSpline Procedes cubic interpolation with std::vectors
 * \param signal Singal to be interpolated
 * \param x X-coordinates of signal
 * \param new_signal Where to put resultin vecotr
 * \param new_x X-coords of resulting vecotr
 * \param step Step of interpolation
 * \param thrLow X-coordinate of the first point of interpolation
 * \param thrHigh X-coordinates of the last point of interpolation
 */
void MathClass::getSpline(std::vector<double> &signal, std::vector<double> &x, std::vector<double> &new_signal, std::vector<double> &new_x, double step, double thrLow, double thrHigh) {

    tk::spline spl;
    spl.set_points(x,signal,true);


    double start_pnt=thrLow;

    while(start_pnt < thrHigh) {
        new_signal.push_back(spl(start_pnt));
        new_x.push_back(start_pnt);
        start_pnt=start_pnt+step;
    }

}

/*!
 * \brief MathClass::getQpolynom Builds approximation polynom of QVector
 * \param dataDep Signal points
 * \param dataIndep X-coordinates of signal
 * \param order Order of polynom
 * \return QVector with polynom coefficients
 */
QVector<double> MathClass::getQpolynom(QVector<double> &dataDep, QVector<double> &dataIndep, int order  ) {
    int dataSize = dataDep.size();
    double a_dataDep[dataSize];
    double a_dataIndep[dataSize];
    double coeffs[order+1];

    for (int i = 0; i < dataDep.size(); i++) {
        a_dataDep[i] = dataDep.at(i);
        a_dataIndep[i] = dataIndep.at(i);
    }
    polyfit(a_dataDep,a_dataIndep,dataSize, order,coeffs );


    QVector<double> coefficients;
    for (int i=0; i<order+1; i++) {
        coefficients.append(coeffs[i]);
    }

    return coefficients;
}



/*!
 * \brief MathClass::getFourierMaximum Finds maximum spike of fourier specturm
 * \param data Fourier spectrum vector
 * \return number if maximum spike element
 */
double MathClass::getFourierMaximum(QVector<double> &data) {
    QVector<double> coords = getMaxesCoordinates(data);

    double max_coord = 0;
    double max=0;
    for (int i=0; i<coords.size(); i++) {
        if (data.at(coords.at(i))>max) {
            max = data.at(coords.at(i));
            max_coord = coords.at(i);
        }
    }

    return max_coord;
}

/*!
 * \brief MathClass::getMaxesCoordinates Finds vecotr of spikes in data
 * \param data Vector to find spikes in
 * \return vector of coordinates of spikes
 */
QVector<double> MathClass::getMaxesCoordinates(QVector<double> &data) {
    QVector<double> coords;
    coords.clear();

    for(int i=3; i<data.size()-3; i++) {
        if (data.at(i)>data.at(i-1) && data.at(i)>data.at(i-2) && data.at(i)>data.at(i+1) && data.at(i)>data.at(i+2)) {
            coords.append(i);
        }
    }

    return coords;
}
