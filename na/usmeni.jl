function f!(F, x)
    F[1] = x[2]-x[1] .^2+1
    F[2] = x[1] -(2*x[2]-x[2].^2)/3
end
x0 = [1.0,  0.0];
x1 = [-3.0 8.0];
nlsolve(f!, x0)
nlsolve(f!, x1)
x = [-3:0.2:3;];
y = x.^2 .+1;
plot(x,y);
y = [-3:0.2:5;];
x=(2 .*y-y.^2) ./3;
plot!(x,y)