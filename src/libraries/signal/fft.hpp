#ifndef _FFT_H_
    #define _FFT_H_

    #include <complex>
    #include <iostream>
    #include <stdio.h>

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

    template<typename T>
    void calculateModule(const std::complex<T>* transform, unsigned int n, T* module){
        for(size_t k=0; k<n; k++){
            module[k] = std::abs(transform[k]);
        }
    }

    template<typename T>
    void calculateArgument(const std::complex<T>* transform, unsigned int n, T* argument){
        for(size_t k=0; k<n; k++){
            argument[k] = std::arg(transform[k]);
        }
    }

    template<typename T>
    void normalizeSignal(T *transform, unsigned int n){
        for(unsigned int i = 0; i < n; i++){
            if(std::abs(transform[i]) != 0.0){
                transform[i] = transform[i]/std::abs(transform[i]);
            }
        }
    }

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

    template<typename T>
    void seeTransform(const std::complex<T>* transform, unsigned int n){
        for(size_t k=0; k<n; k++){
            std::cout << transform[k];
        }
    }
    
#endif
