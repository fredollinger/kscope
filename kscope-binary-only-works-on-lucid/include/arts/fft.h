#ifndef ARTS_FFT_H
#define ARTS_FFT_H

/*
 * BC - Status (2002-03-08): arts_fft_float
 *
 * This fft interface should be obsoleted in the future in favour of one
 * exploiting the capabilities of gsl fully. However, it will be kept binary
 * compatible to avoid everybody to reimplement/duplicate their own fft.
 */


#ifdef __cplusplus
extern "C" {
#endif

/**
 * performs an fft
 */
void arts_fft_float (
    unsigned  NumSamples,          /* must be a power of 2 */
    int       InverseTransform,    /* 0=forward FFT, 1=inverse FFT */
    float    *RealIn,              /* array of input's real samples */
    float    *ImaginaryIn,         /* array of input's imag samples */
    float    *RealOut,             /* array of output's reals */
    float    *ImaginaryOut );      /* array of output's imaginaries */


#ifdef __cplusplus
}
#endif
#endif /* ARTS_FFT_H */
