#ifndef _vana_fft_h_
#define _vana_fft_h_

#include <complex>
#include <vector>

typedef std::complex<double> ComplexDbl;
typedef std::vector<ComplexDbl> VCD; //cell-values for a specific column (size()==height)
typedef std::vector<VCD> VVCD; //list of columns (size()==width)

void fft(VCD &vcd);
void ifft(VCD &vcd); //inverse


#endif
