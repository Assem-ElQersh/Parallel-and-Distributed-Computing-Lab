# Lab 02: Parameterized Speedup and Efficiency Functions

## Overview
This lab implements and analyzes scaled speedup and efficiency functions in parallel computing, exploring how different sequential fractions affect parallel performance.

## Implementation Details
- Language: Python (Jupyter Notebook)
- Key Libraries: NumPy, Matplotlib
- Main Components:
  - Scaled speedup function implementation
  - Scaled efficiency function implementation
  - Comparative plots for different sequential fractions

## Functions Implemented

### 1. Scaled Speedup Function
```
S_γ(p) = (f + (1-f)p^δ) / (f + (1-f)p^(δ-1))
```

### 2. Scaled Efficiency Function
```
E_γ(p) = S_γ(p)/p = (f + (1-f)p^δ) / (pf + (1-f)p^δ)
```

where:
- f: sequential fraction (0.1 and 0.5)
- p: number of processors
- γ: parameterized compute-to-communication ratio (p^δ)

## Results
The implementation generates two plots:
1. `19b78c18-e65d-40d7-b53e-84d86e7ee5e0.png`: Speedup function plots
2. `b475512f-f01c-4c07-929e-31c176949e63.png`: Efficiency function plots

Each plot shows:
- X-axis: Number of processors (p)
- Y-axis: Speedup/Efficiency value
- Multiple curves for different sequential fractions (f=0.1 and f=0.5)

## How to Run
1. Ensure Python 3.x is installed
2. Install required packages:
   ```bash
   pip install numpy matplotlib jupyter
   ```
3. Open and run the Jupyter notebook:
   ```bash
   jupyter notebook code.ipynb
   ```

## Analysis
The plots demonstrate:
- Impact of sequential fraction on parallel performance
- Relationship between speedup and efficiency
- How different values of f affect scalability
- The trade-off between parallelization and overhead 