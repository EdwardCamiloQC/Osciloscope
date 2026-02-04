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

    /** \brief Separates the odd data in a array.
        \tparam values: Values to analyze.
        \param len: Data T1 length. 
        \tparam odd: Array where stores the odd data.
     */
    template<typename T1, typename T2>
    void oddValues(const T1* values, unsigned int len, T2* odd){
        if(!(len%2)){
            for(size_t i=0; i<(len/2); i++){
                *odd = *values;
                if(i!=(len/2-1)){
                    values+=2;
                    odd++;
                }else{
                    values-=(i*2);
                    odd-=i;
                }
            }
        }
    }

    /** \brief Separates the even data in a array.
        \tparam values: Values to analyze.
        \param len: Data T1 length.
        \tparam even: Array where stores the even data.
     */
    template<typename T1, typename T2>
    void evenValues(const T1* values, unsigned int len, T2* even){
        if(!(len%2)){
            values++;
            for(size_t i=0; i<(len/2); i++){
                *even = *values;
                if(i!=(len/2-1)){
                    values+=2;
                    even++;
                }else{
                    values-=(i*2+1);
                    even-=i;
                }
            }
        }
    }

    /** \brief Calculates the fast fourier transform.
        \tparam signal: Signal to evaluate.
        \param length: Signal length.
        \tparam transform: Signal where collect the result.
     */
    template <typename T1, typename T2>
    void calculateFFT(const T1* signal, unsigned int length, std::complex<T2>* transform){
        if(length>1){
            std::complex<float> par[int(length/2)], impar[int(length/2)];
            std::complex<float> oddTransform[int(length/2)], evenTransform[int(length/2)];
            oddValues(signal, length, par);
            evenValues(signal, length, impar);
            calculateFFT(par, sizeof(par)/sizeof(std::complex<float>), oddTransform);
            calculateFFT(impar, sizeof(impar)/sizeof(std::complex<float>), evenTransform);
            for(size_t k=0; k<length; k++){
                if(k<length/2){
                    transform[k] = oddTransform[k] + evenTransform[k] * std::polar<float>(1.0,-2.0*M_PI*k/length);
                    transform[k+length/2] = oddTransform[k] - evenTransform[k] * std::polar<float>(1.0,-2.0*M_PI*k/length);
                }
            }
        }if(length==1){
            *transform = *signal;
        }
    }

    /** \brief Calculates the module of the transform.
        \tparam transform: The fourier transform.
        \param n: Data length.
        \tparam module: Module calculated.
     */
    template<typename T>
    void calculateModule(const std::complex<T>* transform, unsigned int n, T* module){
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
    void calculateArgument(const std::complex<T>* transform, unsigned int n, T* argument){
        for(size_t k=0; k<n; k++){
            argument[k] = std::arg(transform[k]);
        }
    }

    /** \brief This function normalizes the signal.
        \tparam tansform: Transform to normalize.
        \param n: Data length.
     */
    template<typename T>
    void normalizeSignal(T *transform, unsigned int n){
        for(unsigned int i = 0; i < n; i++){
            transform[i] = transform[i]/std::abs(std::complex<T>(n,0));
        }
    }

    /** \brief Algorithm fft
        \tparam signal: Signal to transform.
        \param n: Data length.
        \tparam trasnform: Array to the transform.
        \param normalize: 1 yes, 0 no.
     */
    template<typename T1, typename T2>
    void fft(const T1* signal, unsigned int n, T2* transform, bool normalize){
        double log2n = log(n)/log(2);
        double nInt = log2n - floor(log2n);
        if(nInt==0.0){
            calculateFFT(signal, n, transform);
            if(normalize){
                normalizeSignal(transform, n);
            }
        }else{
            std::cerr << "La longitud debe ser 2^n\n";
        }
    }

    /** \brief Shows the transform.
        \tparam transform: The transform.
        \param n: Data length.
     */
    template<typename T>
    void seeTransform(const std::complex<T>* transform, unsigned int n){
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
    void seeModule(const T signalModule, unsigned int n){
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
    void seeArgument(const T signalModule, unsigned int n){
        std::cout << "\nMODULE\n";
        for(unsigned int k=0; k<n; k++){
            std::cout << signalModule[k] << std::endl;
        }
    }
    
#endif
