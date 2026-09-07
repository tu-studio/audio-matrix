#ifndef FILTER_IIR_H
#define FILTER_IIR_H

double* dcof_bwlp(int n, double fcf);
double* dcof_bwhp(int n, double fcf);

int* ccof_bwlp(int n);
int* ccof_bwhp(int n);

double sf_bwlp(int n, double fcf);
double sf_bwhp(int n, double fcf);

#endif // FILTER_IIR_H
