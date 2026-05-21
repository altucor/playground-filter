# playground-filter

## Building

### Simple build and run
```
mkdir -p build && cd build
cmake ../src -DCMAKE_BUILD_TYPE=Debug && cmake --build .
./app/app ../docs/signal.csv
```

### Visual debugging

```
brew install gnuplot
cd docs
gnuplot -persist -e "filename='expected-output.csv'" plot.gp
```


## Tech notes
The task is to filter out noise from the signal. The nature of noise usually have higher frequency. Let's assume that here we want to filter chunk of signal from sensor. Based on inputs we have, we can say that we need here `Low Pass Filter(LPF)` to filter out high frequecny noise out of useful signal.

### General
As for digital software filters there is 2 general architectures `Finite Impulse response(FIR)` and `Infinite Impulse response(IIR)`.

### Thoughts and findings
FIR and IIR have their pros and cons. FIR is much simpler in design, but requires much more coefficients(taps) and computational respurces just to do simple and repetetive sum and mult operations over signal. IIR allows to achieve better filter sharpness(knee) with less taps. Also IIR filters requires 2 chains for `feed-forward` and `feed-back`, basically set of buffers and coefficients;

...

Filter order of FIR filter it basically `TAPS_COUNT-1`.

...

Probably good to start from FIR as it is much simpler in implementation. Anyway it looks like it can be relatively easily upgraded to IIR





## Docs & Useful Links
https://www.gaussianwaves.com/2017/02/choosing-a-filter-fir-or-iir-understanding-the-design-perspective/  
https://fiiir.com  
https://www.desmos.com/calculator  
https://tomroelandts.com/articles/how-to-create-a-simple-low-pass-filter  
https://www.youtube.com/watch?v=ap1qXBTKU8g - IIR  
https://www.youtube.com/watch?v=_WoqwSBkhj0&t=265s - FIR  
https://dsp.stackexchange.com/questions/8685/filter-order-vs-number-of-taps-vs-number-of-coefficients  
