import Pkg;
Pkg.add("Plots")
using Plots;
x=range(-pi, pi, length = 101);
y=sin.(x);
plot(x,y);