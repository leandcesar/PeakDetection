/*
 * 2019 Leandro César
 * An Arduino library for real time peak detection in sensor data.
 * Credits: https://stackoverflow.com/questions/22583391/peak-peaknal-detection-in-realtime-timeseries-data
 */

#include "PeakDetection.h"

using namespace std;
const int DEFAULT_LAG = 32;
const int DEFAULT_THRESHOLD = 2;
const int DEFAULT_INFLUENCE = 0.5;
const int DEFAULT_EPSILON = 0.01;

PeakDetection::PeakDetection() {
  index = 0;
  lag = DEFAULT_LAG;
  threshold = DEFAULT_THRESHOLD;
  influence = DEFAULT_INFLUENCE;
  EPSILON = DEFAULT_EPSILON;
  int peak = 0;
}

PeakDetection::~PeakDetection() {
  delete data;
  delete avg;
  delete std;
}

void PeakDetection::begin() {
  data = (double *)malloc(sizeof(double) * (lag + 1));
  avg = (double *)malloc(sizeof(double) * (lag + 1));
  std = (double *)malloc(sizeof(double) * (lag + 1));
  for (int i = 0; i < lag; ++i) {
    data[i] = 0.0;
    avg[i] = 0.0;
    std[i] = 0.0;
  }
}

void PeakDetection::begin(int lag, int threshold, double influence) {
  this->lag = lag;
  this->threshold = threshold;
  this->influence = influence;
  data = (double *)malloc(sizeof(double) * (lag + 1));
  avg = (double *)malloc(sizeof(double) * (lag + 1));
  std = (double *)malloc(sizeof(double) * (lag + 1));
  for (int i = 0; i < lag; ++i) {
    data[i] = 0.0;
    avg[i] = 0.0;
    std[i] = 0.0;
  }
}

void PeakDetection::add(double newSample) {
  peak = 0;
  int i = index % lag; //current index
  int j = (index + 1) % lag; //next index
  double deviation = newSample - avg[i];
  if (deviation > threshold * std[i]) {
    data[j] = influence * newSample + (1.0 - influence) * data[i];
    peak = 1;
  }
  else if (deviation < -threshold * std[i]) {
    data[j] = influence * newSample + (1.0 - influence) * data[i];
    peak = -1;
  }
  else
    data[j] = newSample;
  avg[j] = getAvg(j, lag);
  std[j] = getStd(j, lag);
  index++;
  if (index >= 16383) //2^14
    index = lag + j;
}

double PeakDetection::getFilt() {
  int i = index % lag;
  return avg[i];
}

double PeakDetection::getPeak() {
  return peak;
}

double PeakDetection::getAvg(int start, int len) {
  double x = 0.0;
  for (int i = 0; i < len; ++i)
    x += data[(start + i) % lag];
  return x / len;
}

double PeakDetection::getPoint(int start, int len) {
  double xi = 0.0;
  for (int i = 0; i < len; ++i)
    xi += data[(start + i) % lag] * data[(start + i) % lag];
  return xi / len;
}

double PeakDetection::getStd(int start, int len) {
  double x1 = getAvg(start, len);
  double x2 = getPoint(start, len);
  double powx1 = x1 * x1;
  double std = x2 - powx1;
  if (std > -EPSILON && std < EPSILON)
    return 0.0;
  else
    return sqrt(x2 - powx1);
}