/*
 * 2019 Leandro César
 * An Arduino library for real time peak detection in sensor data.
 * Credits: https://stackoverflow.com/questions/22583391/peak-peaknal-detection-in-realtime-timeseries-data
 */

#ifndef PeakDetection_h
#define PeakDetection_h
 
#include <math.h>
#include <stdlib.h>

class PeakDetection {
  public:
    PeakDetection();
    ~PeakDetection();
    void begin();
    void begin(int, int, double); //lag, threshold, influence
    void add(double);
    double getFilt();
    double getPeak();

  private:
    int index, lag, threshold, peak;
    double influence, EPSILON, *data, *avg, *std;
    double getAvg(int, int);
    double getPoint(int, int);
    double getStd(int, int);
};

#endif