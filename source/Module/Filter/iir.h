#ifndef FILTER_IIR_H
#define FILTER_IIR_H

#include <cstddef>
#include <vector>

void dcof_bwlp(double* dcof, size_t dcof_size, double* rcof, size_t rcof_size, int n, double fcf);
void dcof_bwhp(double* dcof, size_t dcof_size, double* rcof, size_t rcof_size, int n, double fcf);

void ccof_bwlp(int* ccof, size_t size, int n);
void ccof_bwhp(int* ccof, size_t size, int n);

double sf_bwlp(int n, double fcf);
double sf_bwhp(int n, double fcf);

class filter_coeff_calculator {
public:
    void prepare(const size_t o, const double i_sr) {
        order = o;
        n_taps = order / 2 + 1;
        inv_sr = i_sr;
        a.resize(order);
        b.resize(n_taps);
        b_int.resize(n_taps);
        rcof.resize(order);
    }

    void calc_lowpass(const double freq) {
        clear_coeffs();
        const int order_half = order / 2;
        double cutoff_freq = 2 * freq * inv_sr ;
            dcof_bwlp(a.data(), a.size(), rcof.data(), rcof.size(),
                    order_half, cutoff_freq);

            ccof_bwlp(b_int.data(), b_int.size(), order_half);
            double scaling_factor = sf_bwlp(order_half, cutoff_freq);

            for (size_t i = 0; i < n_taps; i++) {
                b[i] = scaling_factor * b_int[i];
            }
    }

    void calc_highpass(const double freq) {
        clear_coeffs();
        const int order_half = order / 2;
        double cutoff_freq = 2 * freq * inv_sr ;
        dcof_bwhp(a.data(), a.size(), rcof.data(), rcof.size(),
                order_half, cutoff_freq );

        ccof_bwhp(b_int.data(), b_int.size(), order_half);
        double scaling_factor = sf_bwhp(order_half, cutoff_freq);

        for (size_t i = 0; i < n_taps; i++) {
            b[i] = scaling_factor * b_int[i];
        }
    }

    const std::vector<double>& getA() const {
        return a;
    }

    const std::vector<double>& getB() const {
        return b;
    }
    
private:
    void clear_coeffs() {
        for(auto& a : a)  a = 0.;
        for(auto& b : b)  b = 0.;
        for(auto& r : rcof)  r = 0.;
        for(auto& b : b_int)  b = 0.;
    }

    size_t order;
    size_t n_taps;
    double inv_sr;
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> rcof;
    std::vector<int> b_int;
};

#endif // FILTER_IIR_H
