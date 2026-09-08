#ifndef FILTER_IIR_H
#define FILTER_IIR_H

#include <cstddef>

void dcof_bwlp(double* dcof, size_t dcof_size, double* rcof, size_t rcof_size, int n, double fcf);
void dcof_bwhp(double* dcof, size_t dcof_size, double* rcof, size_t rcof_size, int n, double fcf);

void ccof_bwlp(int* ccof, size_t size, int n);
void ccof_bwhp(int* ccof, size_t size, int n);

double sf_bwlp(int n, double fcf);
double sf_bwhp(int n, double fcf);

#endif // FILTER_IIR_H
