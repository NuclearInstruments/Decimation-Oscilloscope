# Decimation using gated averaging

In the realm of data acquisition and signal processing, the process of decimation plays a crucial role in enhancing the efficiency and accuracy of signal analysis. Decimation, in simple terms, involves reducing the number of samples in a waveform without losing significant information. One popular technique for decimation is averaging, which offers several advantages in signal processing applications.

The principle behind decimation with averaging is to combine adjacent data points and calculate their average, resulting in fewer data points that represent a smoother version of the original signal. This process reduces the computational burden, storage requirements, and noise in the acquired data, making it a valuable technique in various fields, such as telecommunications, digital signal processing, and scientific measurements.

In this document, we will explore the advantages and benefits of utilizing decimation with averaging in the context of our project involving an NIU DT5560 oscilloscope. By implementing this technique, we aim to achieve higher efficiency, reduce acquisition time, and improve the overall quality of the acquired data, ultimately leading to more accurate and insightful signal analysis. Let's delve into the details of how decimation with averaging can significantly contribute to the success of our project.

See effect of decimation on images in the repo DecimationX.png where X indicate the log2 of the decimation factor

# Repository Description

This repository contains the code related to a project that involves various software and firmware for data acquisition and analysis from an NIU DT5560 oscilloscope. Below are the main components of the repository:

## C++ Project

The C++ project is located in the directory `Software\C\lib\VC++\VTC_A.sln`. Within this project, there is a file called `VTC_A_example.cpp` that contains a code example.

### Configuration

In the code example, at line 52 of the file `VTC_A_example.cpp`, you can find a configuration with the following settings:

```cpp
string ip = "192.168.102.219"; // IP OF THE DEVICE
double acquisition_time = 0.005; // SET ACQUISITION LENGTH IN MS
int32_t pre_trigger = 10; // SET PRETRIGGER IN SAMPLE
int32_t trigger_source = 1; // 0: EXTERNAL LEMO 0, 1: INTERNAL PERIODIC
int acquisition_target = 10; // NUMBER OF ACQUISITIONS IN THE RUN
```

### Wave Saving

The code example presents two modes for saving the acquired waveforms:

1. Save as text: This option is slow and takes approximately 150ms to save one waveform.
2. Save in binary format: This option is faster, requiring only 35ms to save one waveform.

Binary format was used to reduce the saving time, but the customer has the flexibility to customize the saving format according to their preferences.

### Decimation Factors

The oscilloscope allows summing all points to be decimated and then dividing them by the number of summed points to obtain a single sample. The decimation factors used are powers of 2. In the provided configuration, the oscilloscope acquires 8192 samples with a decimation factor set to 7, which corresponds to a decimation of 128 times, resulting in an acquisition time of approximately 8ms.

# Compile FPGA
In order to compiler the scf file for R5560A, download and install SciCompiler 2022.8.8.1 or newer. http://www.scicompiler.cloud