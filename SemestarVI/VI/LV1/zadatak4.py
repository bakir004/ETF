import numpy as np
import matplotlib.pyplot as plt

n = np.linspace(0, 4 * np.pi, 100)

x = np.sin(n)
y = np.cos(n)

plt.figure()
plt.plot(n, x, label='sin(n)')
plt.plot(n, y, label='cos(n)')
plt.xlabel('n (radijani)')
plt.ylabel('Amplituda')
plt.title('Sinus i Kosinus (0 do 4π)')
plt.legend()
plt.grid(True)

plt.savefig('sin_cos_combined.png')
plt.close()

fig, (ax1, ax2) = plt.subplots(2, 1)

ax1.plot(n, x, color='blue')
ax1.set_ylabel('Amplituda')
ax1.set_title('sin(n)')
ax1.grid(True)

ax2.plot(n, y, color='red')
ax2.set_xlabel('n (radijani)')
ax2.set_ylabel('Amplituda')
ax2.set_title('cos(n)')
ax2.grid(True)

fig.tight_layout()
plt.savefig('sin_cos_subplots.png', dpi=300)
plt.close()

print("Slike su sacuvane!")
