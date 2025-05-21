import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D


def compute_speedup(gamma, q, k=10): # n = 2^k so k = log2(n) , n = 1024
    numerator = gamma * (2**k - 1)
    denominator = 2 * q + gamma * (2**(k - q) - 1 + q)
    denominator = np.where(denominator <= 0, np.nan, denominator)
    return numerator / denominator

gamma_values = np.linspace(0.01, 0.35, 50)
q_values = np.linspace(0.01, 10, 50)
Gamma, Q = np.meshgrid(gamma_values, q_values)

Speedup = compute_speedup(Gamma, Q)

optimal_q_values = np.linspace(0.01, 10, 50)
denominator_opt = 2**(10 - optimal_q_values) - 1 + optimal_q_values
optimal_gamma_values = np.where(denominator_opt > 0, optimal_q_values / denominator_opt, np.nan)
optimal_speedup_values = compute_speedup(optimal_gamma_values, optimal_q_values)

interp_surface_speedup = np.interp(optimal_q_values, Q[:, 0], Speedup[:, 0])

valid_indices = np.where((optimal_speedup_values >= interp_surface_speedup) & 
                         (optimal_speedup_values <= np.nanmax(Speedup)))

filtered_q = optimal_q_values[valid_indices]
filtered_gamma = optimal_gamma_values[valid_indices]
filtered_speedup = optimal_speedup_values[valid_indices]

fig = plt.figure(figsize=(8, 6))
ax = fig.add_subplot(111, projection='3d')

ax.plot_wireframe(Q, Gamma, Speedup, color='blue', linewidth=0.5)
ax.plot(filtered_q, filtered_gamma, filtered_speedup, color='red', linewidth=3, label="Optimal Speedup (Intersection)")

ax.view_init(elev=20, azim=225)

ax.set_xlabel(r'$q$')
ax.set_ylabel(r'$\gamma$')
ax.set_zlabel(r'$F(\gamma, q)$')

ax.set_xlim(0, 10)
ax.set_ylim(0, 0.4)
ax.set_zlim(0, 25)

ax.legend()
plt.savefig("speedup_plot.png")
plt.show()
