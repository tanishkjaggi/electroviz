import pandas as pd # Import pandas library for data manipulation
import matplotlib.pyplot as plt # Import matplotlib library for plotting
import numpy as np # Import numpy library for numerical computing

# Read the log file into a DataFrame
data = pd.read_csv('electron_data.txt', sep=' ', header=None, names=['x', 'y', 'vx', 'vy'])

# Sample a subset of the data for plotting (e.g., every 100th row as the data is TOO BIG and individual plots look like a baby scribbled making it unreadable )
sampled_data = data.iloc[::100]

# Plot the sampled positions of electrons over time
plt.figure(figsize=(8, 6)) # Create a new figure with specified size
plt.plot(sampled_data['x'], sampled_data['y'], marker='o', linestyle='-', markersize=3) # Plot x and y positions with markers
plt.xlabel('X Position') # Set x-axis label
plt.ylabel('Y Position') # Set y-axis label
plt.title('Electron Trajectories (Sampled)') # Set title of the plot
plt.grid(True) # Enable grid on the plot
plt.show() # Display the plot

# Analyze electron behavior
avg_velocity = data[['vx', 'vy']].mean() # Calculate average velocity in x and y directions
distance_traveled = ((data['x'].diff() ** 2 + data['y'].diff() ** 2) ** 0.5).sum() # Calculate total distance traveled by electrons

print("Average Velocity (vx, vy):", avg_velocity) 
print("Total Distance Traveled:", distance_traveled) 
