# CAPMDS Project Description

## Project Overview
This project implements a multi-start algorithm with the iterated decremental local search for solving the minimum capacitated dominating set problem (CAPMDS). The code is implemented in C++ with performance optimizations for large-scale instances.

## Directory Structure
├── capmds-code/   #Source code directory  
│ ├── WeightedHymain-uniform.cpp   #Main algorithm implementation  
│ ├── Weighted-Init.hpp   #Initialization component  
│ └── set.hpp   #Set operations utility  
└── README.md   #This documentation file  

## Compilation Guide

### Environment Requirements
- Ubuntu 22.04.3 LTS
- GCC/G++ compiler

### Compilation Commands
#### Navigate to the code directory
cd capmds-code/

#### Compile with O2 optimization level
g++ WeightedHymain-uniform.cpp -O2 -o capmds

##Execution Example

### Run the bio-dmelaUniform-2 instance
./capmds Uniform2/bio-dmelaUniform-2

### General instance format (adjust according to actual directory structure)
./capmds [instance_path]/[instance_name]




## Test Datasets

Due to the large size of test instance files (containing 6 capacity variants),
they cannot be stored directly in the GitHub repository. 
Please download them via Baidu Netdisk:

Test Dataset Download Link:
https://pan.baidu.com/s/1leZ5kzTNawzibfjiyTs0xg?pwd=3t6d 
Access Code: 3t6d

## Technical Support
For any issues, please submit them through the Issues section, and we will respond promptly.
