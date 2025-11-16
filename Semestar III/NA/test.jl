cd(@__DIR__)
import Pkg;
# Pkg.add("ForwardDiff")
# Pkg.add("Plots")
using ForwardDiff
using LinearAlgebra
ENV["GKSwstype"] = "100"  # ensures GR saves plots to file instead of GUI
using Plots
gr()  # explicitly use GR backend


# ============= TESTNE FUNKCIJE =============

function rosenbrock_function(x)
  return (1 - x[1])^2 + 100 * (x[2] - x[1]^2)^2
end
function rastrigin_function(x; A=10)
    return A * 2 + x[1]^2 + x[2]^2 - A * (cos(2 * π * x[1]) + cos(2 * π * x[2]))
end
function sphere_function(x)
    return x[1]^2 + x[2]^2
end
function himmelblau_function(x)
    return (x[1]^2 + x[2] - 11)^2 + (x[1] + x[2]^2 - 7)^2
end
function ackley_function(x)
    return -20 * exp(-0.2 * sqrt((x[1]^2 + x[2]^2)/2)) - exp(0.5 * (cos(2 * π * x[1]) + cos(2 * π * x[2]))) + exp(1) + 20
end
function goldstein_price(x)
    term1 = (x[1] + x[2] + 1)^2
    term2 = 19 - 14*x[1] + 3*x[1]^2 - 14*x[2] + 6*x[1]*x[2] + 3*x[2]^2
    term3 = (2*x[1] - 3*x[2])^2
    term4 = 18 - 32*x[1] + 12*x[1]^2 + 48*x[2] - 36*x[1]*x[2] + 27*x[2]^2
    
    f = (1 + (term1 * term2)) * (30 + (term3 * term4))
    return f
end
dva_minimuma_i_sedlo(x) = x[1]^4+x[2]^4-4*x[1]*x[2]+2
svuda_ekstremi(x) = 1/(sin(x[1])*sin(x[2]))
uska(x) = x[1]^2+100*x[2]^2

# ============= END TESTNE FUNKCIJE =============

# ============= FUNKCIJE DESCENTOVA =============

gradient(f, x) = ForwardDiff.gradient(f, x)

function gradient_descent_classic(f; theta_init, learning_rate=0.001, max_iter=10000, tol=1e-6)
    theta = theta_init           
    iter = 0
    trajectory = []

    while iter < max_iter
        g = gradient(f, theta) 
        theta -= learning_rate * g 
        push!(trajectory, copy(theta))

        if norm(g) < tol
            break
        end

        iter += 1
    end

    return theta, iter, trajectory
end

# function gradient_descent_with_momentum(f; theta_init, learning_rate=0.001, alpha=0.9, max_iter=10000, tol=1e-6)
#     theta = theta_init           
#     v = zeros(length(theta)) 
#     iter = 0
#     trajectory = []

#     while iter < max_iter
#         g = gradient(f, theta) 
#         v = alpha * v - learning_rate * g 
#         theta += v 
#         push!(trajectory, copy(theta))

#         if norm(g) < tol
#             break
#         end

#         iter += 1
#     end

#     return theta, iter, trajectory
# end

# function nesterov_gradient_descent(f; theta_init, learning_rate=0.001, alpha=0.9, max_iter=10000, tol=1e-6)
#     theta = theta_init  
#     v = zeros(length(theta))
#     iter = 0
#     trajectory = []

#     while iter < max_iter
#         theta_tilde = theta + alpha * v
#         g = gradient(f, theta_tilde)
#         v = alpha * v - learning_rate * g
#         theta += v
#         push!(trajectory, copy(theta))

#         if norm(g) < tol
#             break
#         end

#         iter += 1
#     end

#     return theta, iter, trajectory
# end

function gradient_descent_with_momentum(f; theta_init, learning_rate=0.001, alpha=0.9, max_iter=10000, tol=1e-6)
    theta = theta_init           
    v = zeros(length(theta)) 
    iter = 0
    trajectory = []

    while iter < max_iter
        g = gradient(f, theta) 
        v = alpha * v - learning_rate * g 
        theta += v 
        push!(trajectory, copy(theta))

        if any(isnan, theta)
            learning_rate /= 2
            theta = theta_init
            v .= 0
            iter = 0
            continue
        end

        if norm(g) < tol
            break
        end

        iter += 1
    end

    return theta, iter, trajectory
end

function nesterov_gradient_descent(f; theta_init, learning_rate=0.001, alpha=0.9, max_iter=10000, tol=1e-6)
    theta = theta_init  
    v = zeros(length(theta))
    iter = 0
    trajectory = []

    while iter < max_iter
        theta_tilde = theta + alpha * v
        g = gradient(f, theta_tilde)
        v = alpha * v - learning_rate * g
        theta += v
        push!(trajectory, copy(theta))

        if any(isnan, theta)
            learning_rate /= 2
            theta = theta_init
            v .= 0
            iter = 0
            continue
        end

        if norm(g) < tol
            break
        end

        iter += 1
    end

    return theta, iter, trajectory
end

# ============= END FUNKCIJE DESCENTOVA =============

# ============= PODESAVANJE PARAMETARA =============

plotxmin = -10
plotxmax = 10
f = himmelblau_function 
learning_rate = 0.001
alpha = 0.8
theta_init = [0.1, 2]
max_iters=100

# ============= END PODESAVANJA PARAMETARA =============

# ============= RACUNANJE ALGORITAMA =============

final_theta, iterations, trajectory_nesterov = nesterov_gradient_descent(f;
                                                                    theta_init=theta_init,
                                                                    learning_rate=learning_rate,
                                                                    alpha=alpha,
                                                                    max_iter=max_iters)




println("Nesterov: Minimum found at: $final_theta after $iterations iterations")
println("Function value at minimum: $(f(final_theta))")

x_nesterov, y_nesterov = getindex.(trajectory_nesterov, 1), getindex.(trajectory_nesterov, 2)
filtered_nesterov = [(x, y) for (x, y) in zip(x_nesterov, y_nesterov) if plotxmin <= x <= plotxmax && plotxmin <= y <= plotxmax]
x_nesterov_filtered, y_nesterov_filtered = getindex.(filtered_nesterov, 1), getindex.(filtered_nesterov, 2)

final_theta_momentum, iterations_momentum, trajectory_momentum = gradient_descent_with_momentum(f;
                                                                       theta_init=theta_init,
                                                                       learning_rate=learning_rate,
                                                                       alpha=alpha,
                                                                       max_iter=max_iters
                                                                       )

println("Momentum: Minimum found at: $final_theta_momentum after $iterations_momentum iterations")
println("Function value at minimum: $(f(final_theta_momentum))")

x_momentum, y_momentum = getindex.(trajectory_momentum, 1), getindex.(trajectory_momentum, 2)
filtered_momentum = [(x, y) for (x, y) in zip(x_momentum, y_momentum) if plotxmin <= x <= plotxmax && plotxmin <= y <= plotxmax]
x_momentum_filtered, y_momentum_filtered = getindex.(filtered_momentum, 1), getindex.(filtered_momentum, 2)

final_theta_gd, iterations_gd, trajectory_gd = gradient_descent_classic(f;
                                                             theta_init=theta_init,
                                                             learning_rate=learning_rate,
                                                             max_iter=max_iters)

println("Gradient Descent: Minimum found at: $final_theta_gd after $iterations_gd iterations")
println("Function value at minimum: $(f(final_theta_gd))")

x_gd, y_gd = getindex.(trajectory_gd, 1), getindex.(trajectory_gd, 2)
filtered_gd = [(x, y) for (x, y) in zip(x_gd, y_gd) if plotxmin <= x <= plotxmax && plotxmin <= y <= plotxmax]
x_gd_filtered, y_gd_filtered = getindex.(filtered_gd, 1), getindex.(filtered_gd, 2)

# ============= END RACUNANJE ALGORITAMA =============

# ============= PLOTANJE PUTANJE SPUSTA =============

# xx = LinRange(plotxmin, plotxmax, 100)
# yy = LinRange(plotxmin, plotxmax, 100)
# zz = [f([x, y]) for x in xx, y in yy]

# contour(xx, yy, zz, xlabel="x", ylabel="y", color=:viridis, levels=50)

# scatter!(x_nesterov_filtered, y_nesterov_filtered, label="Nesterov", color=:blue, markersize=3, marker=:circle)
# scatter!(x_momentum_filtered, y_momentum_filtered, label="Momentum", color=:red, markersize=3, marker=:cross)
# scatter!(x_gd_filtered, y_gd_filtered, label="Gradient Descent", color=:green, markersize=4, marker=:circle)

# ============= END PLOTANJE PUTANJE SPUSTA =============

# ============ PLOTANJE GRESKE ============

# function compute_absolute_error(f, trajectory, f_star)
#     return [abs(f(theta) - f_star) for theta in trajectory]
# end

# f_star = minimum([f(theta) for theta in trajectory_nesterov])
# absolute_errors = compute_absolute_error(f, trajectory_nesterov, f_star)
# absolute_errors2 = compute_absolute_error(f, trajectory_momentum, f_star)

# absolute_errors = Base.max.(absolute_errors, 1e-10)
# absolute_errors2 = Base.max.(absolute_errors2, 1e-10)

# iterations = 1:length(absolute_errors)
# iterations_momentum = 1:length(absolute_errors2)
# plot(iterations, absolute_errors, xlabel="Iteracija", ylabel="Greška f(theta) - f*", yscale=:log10, label="Nesterov Gradient Descent")
# plot!(iterations_momentum, absolute_errors2, xlabel="Iteracija", ylabel="Greška f(theta) - f*", yscale=:log10, label="Momentum Gradient Descent")

# ============ END PLOTANJE GRESKE ============

# ============= PLOTANJE KONVERGENCIJA =============

xv = LinRange(plotxmin, plotxmax, 110)
yv = LinRange(plotxmin, plotxmax, 110)

final_theta_matrix = zeros(Float64, length(xv), length(yv))
final_theta_matrix2 = zeros(Float64, length(xv), length(yv))

for (i, x) in enumerate(xv)
    for (j, y) in enumerate(yv)
        point, _, _ = gradient_descent_with_momentum(
            f;
            theta_init=[x,y],
            learning_rate=learning_rate,
            alpha=alpha,
            max_iter=max_iters
        )
        point2, _, _ = nesterov_gradient_descent(
            f;
            theta_init=[x,y],
            learning_rate=learning_rate,
            alpha=alpha,
            max_iter=max_iters
        )
        final_theta_matrix[i, j] = f(point)
        final_theta_matrix2[i, j] = f(point2)
    end
    println("Row $i done")
end


min_nesterov = minimum(final_theta_matrix2[.!isnan.(final_theta_matrix2)])
println("Minimum Nesterov: $min_nesterov")
max_nesterov = maximum(final_theta_matrix2[.!isnan.(final_theta_matrix2)])
println("Maximum Nesterov: $max_nesterov")

min_momentum = minimum(final_theta_matrix[.!isnan.(final_theta_matrix)])
println("Minimum Momentum: $min_momentum")
max_momentum = maximum(final_theta_matrix[.!isnan.(final_theta_matrix)])
println("Maximum Momentum: $max_momentum")

scatter_x = repeat(xv, inner=length(yv))  
scatter_y = repeat(yv, outer=length(xv)) 
scatter_color1 = vec(final_theta_matrix')
scatter_color2 = vec(final_theta_matrix2')

p1 = scatter(
    scatter_x,
    scatter_y,
    zcolor=scatter_color1,
    xlabel="x (početno)",
    ylabel="y (početno)",
    color=:viridis,
    markershape=:rect,
    markersize=2,
    markerstrokecolor=:transparent,
    markerfillcolor=:auto,
    markerstrokewidth=0,
    legend=false,
    aspect_ratio=:equal,
    colorbar=true,
    size=(600, 600)
)

p2 = scatter(
    scatter_x,
    scatter_y,
    zcolor=scatter_color2,
    xlabel="x (početno)",
    color=:viridis,
    markershape=:rect,
    markersize=2,
    markerstrokecolor=:transparent,
    markerfillcolor=:auto,
    markerstrokewidth=0,
    legend=false,
    aspect_ratio=:equal,
    colorbar=true,
    size=(600, 600)
)

# plot(p1, p2, layout=(1, 2), size=(1400, 600))
combined = plot!(p1, p2, layout = (1, 2), size = (1400, 600))
output_path = joinpath(pwd(), "scatter_comparison.png")
savefig(combined, output_path)
# ============= END PLOTANJE KONVERGENCIJA =============
