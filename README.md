# ElectroViz : Simulating and Analyzing Electron Trajectories

## Overview
This project consists of a C++ program that simulates the movement of electrons in a 2D space and a Python script for analyzing the simulation data. The simulation generates trajectories of electrons moving within a defined window, and the analysis script visualizes these trajectories and calculates key metrics about electron behavior.

## How it Works
### Simulation (C++)
- The C++ program uses OpenGL for rendering a window and simulating electron movement.
- Electrons are represented as points with associated velocities and labels.
- Electrons move within the defined window, reflecting off the edges when they reach them.
- Electron positions and velocities are logged periodically to a text file (`electron_data.txt`).

### Analysis (Python)
- The Python script reads the logged data from `electron_data.txt` into a DataFrame using pandas.
- It samples the data for plotting trajectories and visualizes the sampled trajectories using matplotlib.
- The script calculates average velocity and total distance traveled by electrons for further analysis.

## Outputs
The project generates the following outputs:
- Electron trajectories plotted over time.
- Average velocity of electrons in the x and y directions.
- Total distance traveled by electrons.

## Libraries Used
- C++: OpenGL, Windows API ( resource folders included )
- Python: pandas, matplotlib, numpy

## Math Formulas
- Total distance traveled by an electron: \( \sqrt{{(\Delta x)^2 + (\Delta y)^2}} \)

## Languages Used
- C++
- Python

## Usage
1. Compile and run the C++ program to generate electron movement data.
2. Execute the Python script to analyze the generated data and visualize electron trajectories , sampled at every 100th row as data generated is very large

## Contributors & Maintainers
- Tanishk Jaggi
