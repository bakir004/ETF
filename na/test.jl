import Pkg;
# Pkg.add("ForwardDiff")
# Pkg.add("Plots")
using ForwardDiff
using LinearAlgebra
using Plots

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
function rosenbrock_perturbed(x)
    return (1 - x[1])^2 + 100 * (x[2] - x[1]^2)^2 + 0.01 * (x[1]^3 + x[2]^3)
end

gradient(f, x) = ForwardDiff.gradient(f, x)

function gradient_descent_classic(f; θ_init=[0.0, 0.0], learning_rate=0.001, max_iter=10000, tol=1e-6)
    θ = θ_init           
    iter = 0
    trajectory = []

    while iter < max_iter
        g = gradient(f, θ) 
        θ -= learning_rate * g 
        push!(trajectory, copy(θ))

        if norm(g) < tol
            break
        end

        iter += 1
    end

    return θ, iter, trajectory
end

function gradient_descent_with_momentum(f; θ_init, learning_rate=0.001, alpha=0.9, max_iter=10000, tol=1e-10)
    θ = θ_init           
    v = zeros(length(θ)) 
    iter = 0
    trajectory = []

    while iter < max_iter
        g = gradient(f, θ) 
        v = alpha * v - learning_rate * g 
        θ += v 
        push!(trajectory, copy(θ))

        if norm(g) < tol
            break
        end

        iter += 1
    end

    return θ, iter, trajectory
end

function nesterov_gradient_descent(f; θ_init, learning_rate=0.001, alpha=0.9, max_iter=10000, tol=1e-12)
    θ = θ_init  
    v = zeros(length(θ))
    iter = 0
    trajectory = []

    while iter < max_iter
        θ_tilde = θ + alpha * v
        g = gradient(f, θ_tilde)
        v = alpha * v - learning_rate * g
        θ += v
        push!(trajectory, copy(θ))

        if norm(g) < tol
            break
        end

        iter += 1
    end

    return θ, iter, trajectory
end

dva_minimuma_i_sedlo(x) = x[1]^4+x[2]^4-4*x[1]*x[2]+2
svuda_ekstremi(x) = 1/(sin(x[1])*sin(x[2]))
nigga(x) = x[1]^4+x[2]^4-2(x[1]-x[2])^2

funkcija(x) = x[1]^2-x[2]^2

plotxmin = -0.1
plotxmax = 1
f = nigga
learning_rate = 0.00134
alpha = 0.955
theta_init = [0.5, 0.4]
max_iters=10000

final_θ, iterations, trajectory_nesterov = nesterov_gradient_descent(f;
                                                                    θ_init=theta_init,
                                                                    learning_rate=learning_rate,
                                                                    alpha=alpha,
                                                                    max_iter=max_iters)




println("Nesterov: Minimum found at: $final_θ after $iterations iterations")
println("Function value at minimum: $(f(final_θ))")

x_nesterov, y_nesterov = getindex.(trajectory_nesterov, 1), getindex.(trajectory_nesterov, 2)
filtered_nesterov = [(x, y) for (x, y) in zip(x_nesterov, y_nesterov) if plotxmin <= x <= plotxmax && plotxmin <= y <= plotxmax]
x_nesterov_filtered, y_nesterov_filtered = getindex.(filtered_nesterov, 1), getindex.(filtered_nesterov, 2)

final_θ_momentum, iterations_momentum, trajectory_momentum = gradient_descent_with_momentum(f;
                                                                       θ_init=theta_init,
                                                                       learning_rate=learning_rate,
                                                                       alpha=alpha,
                                                                       max_iter=max_iters
                                                                       )

println("Momentum: Minimum found at: $final_θ_momentum after $iterations_momentum iterations")
println("Function value at minimum: $(f(final_θ_momentum))")

x_momentum, y_momentum = getindex.(trajectory_momentum, 1), getindex.(trajectory_momentum, 2)
filtered_momentum = [(x, y) for (x, y) in zip(x_momentum, y_momentum) if plotxmin <= x <= plotxmax && plotxmin <= y <= plotxmax]
x_momentum_filtered, y_momentum_filtered = getindex.(filtered_momentum, 1), getindex.(filtered_momentum, 2)

final_θ_gd, iterations_gd, trajectory_gd = gradient_descent_classic(f;
                                                             θ_init=theta_init,
                                                             learning_rate=learning_rate,
                                                             max_iter=max_iters)

println("Gradient Descent: Minimum found at: $final_θ_gd after $iterations_gd iterations")
println("Function value at minimum: $(f(final_θ_gd))")

x_gd, y_gd = getindex.(trajectory_gd, 1), getindex.(trajectory_gd, 2)
filtered_gd = [(x, y) for (x, y) in zip(x_gd, y_gd) if plotxmin <= x <= plotxmax && plotxmin <= y <= plotxmax]
x_gd_filtered, y_gd_filtered = getindex.(filtered_gd, 1), getindex.(filtered_gd, 2)

function compute_absolute_error(f, trajectory, f_star)
    return [abs(f(θ) - f_star) for θ in trajectory]
end
f_star = minimum([f(θ) for θ in trajectory_nesterov])# Approximate f* as the minimum in trajectory
absolute_errors = compute_absolute_error(f, trajectory_nesterov, f_star)
absolute_errors2 = compute_absolute_error(f, trajectory_momentum, f_star)

# Clip errors to a small value to avoid log10 issues
absolute_errors = max.(absolute_errors, 1e-10)
absolute_errors2 = max.(absolute_errors2, 1e-10)
# Plot the error term over iterations
iterations = 1:length(absolute_errors)
iterations_momentum = 1:length(absolute_errors2)
plot(iterations, absolute_errors, xlabel="Iteracija", ylabel="Greška f(θ) - f*", yscale=:log10, title="Greška kroz iteracije", label="Nesterov Gradient Descent")
plot!(iterations_momentum, absolute_errors2, xlabel="Iteracija", ylabel="Greška f(θ) - f*", yscale=:log10, title="Greška kroz iteracije", label="Momentum Gradient Descent")

# xx = LinRange(plotxmin, plotxmax, 100)
# yy = LinRange(plotxmin, plotxmax, 100)
# zz = [f([x, y]) for x in xx, y in yy]

# contour(xx, yy, zz, xlabel="x", ylabel="y", title="Himmelblauova funkcija", color=:viridis, levels=50)

# scatter!(x_nesterov_filtered, y_nesterov_filtered, label="Nesterov", color=:blue, markersize=3, marker=:circle)
# scatter!(x_momentum_filtered, y_momentum_filtered, label="Momentum", color=:red, markersize=3, marker=:cross)
# scatter!(x_gd_filtered, y_gd_filtered, label="Gradient Descent", color=:green, markersize=4, marker=:cross)

# learning_rates = LinRange(0.0013, 0.0015, 200)
# alphas = LinRange(0.954, 0.958, 100)

# iterations_matrix = zeros(Float64, length(learning_rates), length(alphas))

# for (i, lr) in enumerate(learning_rates)
#     for (j, alpha) in enumerate(alphas)
#         _, iter, _ = nesterov_gradient_descent(rosenbrock_function; 
#                                                learning_rate=lr, 
#                                                alpha=alpha)
#         iterations_matrix[i, j] = iter
#     end
# end

# contour(
#     learning_rates, 
#     alphas, 
#     iterations_matrix', # Transpose the matrix to match axes correctly
#     xlabel="Learning Rate", 
#     ylabel="Momentum (α)", 
#     title="Nesterov GD Iterations Contour Plot", 
#     color=:viridis,
#     levels=15 # Adjust the number of contour levels as needed
# )
# scatter!(
#     repeat(learning_rates, inner=length(alphas)),  # Repeat learning rates
#     repeat(alphas, outer=length(learning_rates)),  # Repeat alphas
#     zcolor=iterations_matrix'[:],                  # Flatten iterations
#     label="Iterations",
#     color=:red,
#     markersize=4
# )
