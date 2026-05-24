# playground-filter

## Building
```
mkdir -p build && cd build && cmake ../src -DCMAKE_BUILD_TYPE=Debug && cmake --build .
```

## Usage:
| Key | Description | Example |
| ------ | ------ | ------ |
| --input | Sets input `.csv` file. Expected format with only 2 collumns: time and value | --input=./docs/signal.csv |
| --output | Allows to set where filtered output will be stored. Format same as in reference `expected-output.csv` | --output=./docs/output.csv |
| --type | Allows to select type of applied FIR filter. There is only 2 available: `moving-average` and `blackman`(which means blackman windowing) | --type=moving-average |
| --order | Sets order of FIR filter when `--type=moving-average` selected. Default is `--type=moving-average` | --order=10 |
| --cutoff | Sets cutoff frequency of FIR filter when `--type=blackman` selected. Keep in mind that input data is sampled at 1kHz so based on niquist theory maximum frequency of useful signal is 500Hz. Values higher then 500 will be overwritten to 500 | --cutoff=200 |


## Visual debugging with gnuplot

1. Install gnuplot via `apt` or `brew`

```
brew install gnuplot
```

2. Plot your file with 2 signals and with config from `docs/plot.gp`
```
gnuplot -persist -e "filename='docs/expected-output.csv'" docs/plot.gp
```

One-shot automated ways:
```
# ./run-moving-average.sh <filename-from-docs-dir> <order>
./run-moving-average.sh signal 20
```

```
# ./run-blackman.sg <filename-from-docs-dir> <cutoff-freq>
./run-blackman.sh signal 30
```

## Tech notes
The task is to filter out noise from the signal. The nature of noise usually have higher frequency. Let's assume that here we want to filter chunk of signal from sensor. Based on inputs we have, we can say that we need here `Low Pass Filter(LPF)` to filter out high frequecny noise out of useful signal.

### General
As for digital software filters there is 2 general architectures `Finite Impulse response(FIR)` and `Infinite Impulse response(IIR)`. Main difference here is:
1) FIR - Only feed forward, buffering of input samples
2) IIR - Also feed forward and additional feed back part which reinjects filtered data back in to filter input

### Thoughts and findings
FIR and IIR have their pros and cons. FIR is much simpler in design, but requires much more coefficients(taps) and computational resources just to do simple and repetetive `sum` and `mult` operations over signal. IIR allows to achieve same filter sharpness(knee) but with less taps then FIR. Also IIR filters requires 2 chains for `feed-forward(FF)` and `feed-back(FB)`, basically set of buffers and coefficients. And it may require much complex computation to derive taps for FF and FB, also alignment of them can be tricky. To keep things simple while learning i decided to stay on FIR.

### Window method
Rectangular, Hann, Hamming, Blackman, Kaiser.
Let's chose here `Blackman` as it should have more aggresive cutting knee.

## Conclusion
I have tried to run both types of filters over signal and it looks like moving average is most suitable for our needs. With smaller FIR order moving-average produces less noisy signal with smaller delay. The blackman windowing method requires much more higher filter order(taps and buffer size) to achieve close to reference result. Higher order results in significant constant delay or phase shift.

The best config is moving-average and order around 10-20:
```
./app/app --input=../docs/signal.csv --output=../docs/output.csv --type=moving-average --order=10
```

The best for blackman is: `cutoff frequency=30` but phase shift is around `100ms`:
```
./app/app --input=../docs/signal.csv --output=../docs/output.csv --type=blackman --cutoff=30
```

### Areas for improvement:
1) Implement filter method which can process chunks of samples per step instead of one sample per step
2) Normalize all sample values and filter coefficients from `float` to for example `int16_t`. Decimal math operations much cheaper then floating


## Docs & Useful Links
https://www.gaussianwaves.com/2017/02/choosing-a-filter-fir-or-iir-understanding-the-design-perspective/  
https://fiiir.com  
https://www.desmos.com/calculator  
https://tomroelandts.com/articles/how-to-create-a-simple-low-pass-filter  
https://www.youtube.com/watch?v=ap1qXBTKU8g - IIR  
https://www.youtube.com/watch?v=_WoqwSBkhj0&t=265s - FIR  
https://dsp.stackexchange.com/questions/8685/filter-order-vs-number-of-taps-vs-number-of-coefficients  
Claude  
Gemini  
