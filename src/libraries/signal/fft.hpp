/** \file fft.hpp
    \author Edward Camilo
    \version v1.0
    \date enero 2026
 */
#ifndef _FFT_H_
    #define _FFT_H_

    #include <complex>
    #include <iostream>
    #include <stdio.h>
    #include <memory>
    #include <algorithm>
    #include <vector>
    #include <type_traits>

    template <class>
    inline constexpr bool always_false = false;

    namespace SIGNALS{
        /** \brief Fast Fourier Transform
            \class
         */
        class FFT{
            public:
                /** \note Constructor disabled.
                 */
                FFT() = delete;

                /** \note Copy constrcutor disabled.
                 */
                FFT(const FFT &fft) = delete;

                /** \note Assignment operator disabled.
                 */
                FFT& operator=(FFT fft) = delete;

                /** \note Destructor disabled.
                 */
                ~FFT() = delete;

                /** \brief Calculates the module of the transform.
                    \tparam transform: The fourier transform.
                    \param n: Data length.
                    \tparam module: Module calculated.
                 */
                template<typename T>
                static void calculateModule(const std::complex<T>* transform, unsigned int n, T* module){
                    for(size_t k=0; k<n; k++){
                        module[k] = std::abs(transform[k]);
                    }
                }

                /** \brief Calculates the argument of the transform.
                    \tparam trasnform: The fourier trasnform.
                    \param n: Data length.
                    \tparam argument: The array where the result will be store.
                 */
                template<typename T>
                static void calculateArgument(const std::complex<T>* transform, unsigned int n, T* argument){
                    for(size_t k=0; k<n; k++){
                        argument[k] = std::arg(transform[k]);
                    }
                }

                /** \brief Algorithm fft
                    \tparam signal: Signal to transform.
                    \param n: Data length.
                    \tparam trasnform: Array to the transform.
                    \param normalize: 1 yes, 0 no.
                 */
                template<typename T1, typename T2>
                static void fft(const T1* signal, unsigned int N, std::complex<T2>* transform){
                    if(signal == nullptr || transform == nullptr){
                        throw std::invalid_argument("Signal or Transform is nullptr");
                    }
                    if(N == 0u) return;
                    
                    using SignalT = std::remove_cv_t<T1>;
                    using OutT = T2;
                    using COut = std::complex<OutT>;

                    if constexpr(
                        !std::is_same_v<SignalT, int> &&
                        !std::is_same_v<SignalT, float>&&
                        !std::is_same_v<SignalT, double> &&
                        !std::is_same_v<SignalT, std::complex<int>> &&
                        !std::is_same_v<SignalT, std::complex<float>> &&
                        !std::is_same_v<SignalT, std::complex<double>>)
                    {
                        static_assert(always_false<SignalT>, "Tipo de señal no soportado");
                    }

                    if((N & (N-1)) != 0u){
                        std::cerr << "La longitud debe ser potencia de 2\n";
                        return;
                    }

                    std::vector<COut> a;
                    a.resize(N);

                    if constexpr(std::is_same_v<SignalT, int> || std::is_same_v<SignalT, float> || std::is_same_v<SignalT, double>){
                        for(unsigned int i = 0; i < N; i++){
                            a[i] = COut(static_cast<OutT>(signal[i]), static_cast<OutT>(0));
                        }
                    }else{
                        for(unsigned int i = 0; i < N; i++){
                            auto s = signal[i];
                            a[i] = COut(static_cast<OutT>(s.real()), static_cast<OutT>(s.imag()));
                        }
                    }

                    //bit reversal permutation
                    for(unsigned int i = 1, j = 0; i < N; i++){
                        unsigned int bit = N >> 1;
                        for(; j & bit; bit >>= 1){
                            j ^= bit;
                        }
                        j ^= bit;
                        if(i < j){
                            std::swap(a[i], a[j]);
                        }
                    }

                    //Cooley-Tuckey iterative
                    const double PI = std::acos(-1.0);
                    for(unsigned int len = 2; len <= N; len <<= 1){
                        double angle = 2.0 * PI / static_cast<double>(len);
                        COut wlen(static_cast<OutT>(std::cos(angle)), static_cast<OutT>(-std::sin(angle)));
                        for(unsigned int i = 0; i < N; i += len){
                            COut w(static_cast<OutT>(1));
                            unsigned int half = len >> 1;
                            for(unsigned int k = 0; k < half; k++){
                                COut u = a[i+k];
                                COut v = a[i+k+half] * w;
                                a[i + k] = u + v;
                                a[i + k + half] = u - v;
                                w *= wlen;
                            }
                        }
                    }

                    OutT invN = static_cast<OutT>(1) / static_cast<OutT>(N);

                    for(unsigned int i = 0; i < N; i++){
                        transform[i] = a[i] * invN;
                    }
                }

                /** \brief Shows the transform.
                    \tparam transform: The transform.
                    \param n: Data length.
                 */
                template<typename T>
                static void seeTransform(const std::complex<T>* transform, unsigned int n){
                    std::cout << "\nFFT\n";
                    for(size_t k=0; k<n; k++){
                        std::cout << transform[k] << std::endl;
                    }
                }

                /** \brief Shows the signal module.
                    \param signalModule: Signal module.
                    \param n: Length.
                 */
                template<typename T>
                static void seeModule(const T signalModule, unsigned int n){
                    std::cout << "\nMODULE\n";
                    for(unsigned int k=0; k<n; k++){
                        std::cout << signalModule[k] << std::endl;
                    }
                }

                /** \brief Shows the signal argument.
                    \param signalModule: Signal argument.
                    \param n: Length.
                 */
                template<typename T>
                static void seeArgument(const T signalModule, unsigned int n){
                    std::cout << "\nMODULE\n";
                    for(unsigned int k=0; k<n; k++){
                        std::cout << signalModule[k] << std::endl;
                    }
                }
        };
    }
    
#endif
