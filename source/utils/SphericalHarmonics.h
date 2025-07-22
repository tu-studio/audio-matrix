#ifndef SPHERICALHARMONICS_H
#define SPHERICALHARMONICS_H


#include <vector>
#include <memory>
#include <cmath>
#include <iostream>
#include <Common.h>
#include <CustomAtomic.h>

class SphericalHarmonics {
    public:
        SphericalHarmonics() = default;
        
        void initalize(int order){
            m_order = order;
            m_channels = (m_order+1)*(m_order+1);
            calculate_normalization_factors();
        }

        void update_spherical_harmonics(float azim, float elev, float dist, float nearfield_size, std::vector<float>& sh_container){
            int i = 0;
            float distance_gain = distance_gain_function(dist);

            // in the nearfield the audio is assumed to come from every direction at the same strength
            float farfield_gain = 1.0;
            float nearfield_gain = 0.0;
            if (dist < nearfield_size){
                float dist_rad = M_PI_2 * (dist / nearfield_size);
                farfield_gain = sin(dist_rad);
                nearfield_gain = cos(dist_rad);
            }
            for (int n = 0; n <= m_order;n++){
                for (int m = -n; m <= n; m++){
                    float a_factor = (m<0) ? sin(abs(m) * azim) : cos(abs(m) * azim);
                    float l = legendre(n, m, elev);
                    
                    if (n==0){
                        sh_container[i] = (m_norm_factors[i] * (a_factor * l * distance_gain + nearfield_gain));
                    } else {
                        sh_container[i] = (m_norm_factors[i] * (a_factor * l * farfield_gain) * distance_gain);
                    }
                    i++;
                }
            }
        }
    private:

        void calculate_normalization_factors(){
            m_norm_factors.clear();
            for (int n = 0; n <= m_order; n++){
                for (int m = -n; m <= n; m++){
                    float d = (m==0) ? 1.0 : 0.0;
                    float factor1 = 2-d;// / (4*pi);
                    float factor2 = 1.0 * factorial(n-abs(m)) / factorial(n+abs(m));
                    float norm = sqrt(factor1 * factor2);
                    m_norm_factors.push_back(norm);
                }
            }
        }
        static long factorial(const int n)
        {
            long f = 1;
            for (int i=1; i<=n; ++i)
                f *= i;
            return f;
        }
        static float legendre(const int n, const int m, float elev){
            switch (n)
            {
            case 0:
                return 1;
            case 1:
                switch (abs(m)){
                case 0:
                    return sin(elev);
                case 1:
                    return cos(elev);
                default:
                    break;
                }
                break;
            case 2:
                switch (abs(m)){
                case 0:
                    return 1 / 2 * (3 * sin(elev) * sin(elev) - 1);
                case 1:
                    return 3 * sin(elev) * cos(elev);
                case 2:
                    return 3 * cos(elev) * cos(elev);           
                default:
                    break;
                }
                break;
            case 3:
                switch (abs(m)){
                    case 0:
                        {
                            float sin_elev = sin(elev);
                            return 1 / 2 * (5 * sin_elev * sin_elev * sin_elev - 3 * sin_elev);

                        }
                    case 1:
                        {
                            float sin_elev = sin(elev);
                            return 3 / 2 * (5 * sin_elev * sin_elev - 1) * cos(elev);
                        }
                    case 2:
                        {
                            float cos_elev = cos(elev);
                            return 15 * sin(elev) * cos_elev * cos_elev;
                        }
                    case 3:
                    
                        {
                            float cos_elev = cos(elev);
                            return 15 * cos_elev * cos_elev * cos_elev;
                        }
                    default:
                        break;
                }
            case 4:
                switch (abs(m)){
                    case 0:
                        {
                            float sin_elev = sin(elev);
                            float sin_elev2 = sin_elev * sin_elev;
                            return 1 / 8 * (35 * sin_elev2 * sin_elev2 - 30 * sin_elev2 + 3);
                        }
                    case 1:
                    
                        {
                            float sin_elev = sin(elev);
                            float cos_elev = cos(elev);
                            return 5 / 2 * (7 * sin_elev * sin_elev * sin_elev  - 3 * sin_elev) * cos_elev;
                        }
                    case 2:
                        {
                            float sin_elev = sin(elev);
                            float cos_elev = cos(elev);
                            return 15 / 2 * (7 * sin_elev * sin_elev - 1) * cos_elev * cos_elev;
                        }
                    case 3:
                        {
                            float sin_elev = sin(elev);
                            float cos_elev = cos(elev);
                            return 105 * sin_elev * cos_elev * cos_elev * cos_elev;
                        }
                    case 4:
                        {
                            float cos_elev = cos(elev);
                            return 105 * cos_elev * cos_elev * cos_elev * cos_elev;
                        }
                    default:
                        break;
                }
            
            default:
                break;
            }
            std::cout << "Invalid legendre function" << std::endl;
            return 0;
            
        }
        int m_order;
        int m_channels;
        std::vector<float> m_norm_factors;

};

#endif //SPHERICALHARMONICS_H
