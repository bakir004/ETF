# import Pkg;
# # Pkg.add("ForwardDiff")
# # Pkg.add("Plots")
# using ForwardDiff
# using LinearAlgebra
# using Plots

# function rosenbrock_function(x)
#   return (1 - x[1])^2 + 100 * (x[2] - x[1]^2)^2
# end
# function rastrigin_function(x; A=10)
#     return A * 2 + x[1]^2 + x[2]^2 - A * (cos(2 * π * x[1]) + cos(2 * π * x[2]))
# end
# function sphere_function(x)
#     return x[1]^2 + x[2]^2
# end
# function himmelblau_function(x)
#     return (x[1]^2 + x[2] - 11)^2 + (x[1] + x[2]^2 - 7)^2
# end
# function ackley_function(x)
#     return -20 * exp(-0.2 * sqrt((x[1]^2 + x[2]^2)/2)) - exp(0.5 * (cos(2 * π * x[1]) + cos(2 * π * x[2]))) + exp(1) + 20
# end
# function rosenbrock_perturbed(x)
#     return (1 - x[1])^2 + 100 * (x[2] - x[1]^2)^2 + 0.01 * (x[1]^3 + x[2]^3)
# end

# gradient(f, x) = ForwardDiff.gradient(f, x)

# function gradient_descent_classic(f; θ_init, learning_rate=0.001, max_iter=10000, tol=1e-6)
#     θ = θ_init           
#     iter = 0
#     trajectory = []

#     while iter < max_iter
#         g = gradient(f, θ) 
#         θ -= learning_rate * g 
#         push!(trajectory, copy(θ))

#         if norm(g) < tol
#             break
#         end

#         iter += 1
#     end

#     return θ, iter, trajectory
# end

# function gradient_descent_with_momentum(f; θ_init, learning_rate=0.001, alpha=0.9, max_iter=10000, tol=1e-10)
#     θ = θ_init           
#     v = zeros(length(θ)) 
#     iter = 0
#     trajectory = []

#     while iter < max_iter
#         g = gradient(f, θ) 
#         v = alpha * v - learning_rate * g 
#         θ += v 
#         push!(trajectory, copy(θ))

#         if norm(g) < tol
#             break
#         end

#         iter += 1
#     end

#     return θ, iter, trajectory
# end

# function nesterov_gradient_descent(f; θ_init, learning_rate=0.001, alpha=0.9, max_iter=10000, tol=1e-12)
#     θ = θ_init  
#     v = zeros(length(θ))
#     iter = 0
#     trajectory = []

#     while iter < max_iter
#         θ_tilde = θ + alpha * v
#         g = gradient(f, θ_tilde)
#         v = alpha * v - learning_rate * g
#         θ += v
#         push!(trajectory, copy(θ))

#         if norm(g) < tol
#             break
#         end

#         iter += 1
#     end

#     return θ, iter, trajectory
# end

# dva_minimuma_i_sedlo(x) = x[1]^4+x[2]^4-4*x[1]*x[2]+2
# svuda_ekstremi(x) = 1/(sin(x[1])*sin(x[2]))
# uska(x) = x[1]^2+100*x[2]^2
# function goldstein_price(x)
#     term1 = (x[1] + x[2] + 1)^2
#     term2 = 19 - 14*x[1] + 3*x[1]^2 - 14*x[2] + 6*x[1]*x[2] + 3*x[2]^2
#     term3 = (2*x[1] - 3*x[2])^2
#     term4 = 18 - 32*x[1] + 12*x[1]^2 + 48*x[2] - 36*x[1]*x[2] + 27*x[2]^2

#     f = (1 + (term1 * term2)) * (30 + (term3 * term4))
#     return f
# end
# println(goldstein_price([0, -1])) # 3
# funkcija(x) = x[1]^2-x[2]^2

# plotxmin = -4
# plotxmax = 4
# f = sphere_function
# learning_rate = 0.001
# alpha = 0.9
# theta_init = [0, -2.1]
# max_iters=1000

# final_θ, iterations, trajectory_nesterov = nesterov_gradient_descent(f;
#                                                                     θ_init=theta_init,
#                                                                     learning_rate=learning_rate,
#                                                                     alpha=alpha,
#                                                                     max_iter=max_iters)




# println("Nesterov: Minimum found at: $final_θ after $iterations iterations")
# println("Function value at minimum: $(f(final_θ))")

# x_nesterov, y_nesterov = getindex.(trajectory_nesterov, 1), getindex.(trajectory_nesterov, 2)
# filtered_nesterov = [(x, y) for (x, y) in zip(x_nesterov, y_nesterov) if plotxmin <= x <= plotxmax && plotxmin <= y <= plotxmax]
# x_nesterov_filtered, y_nesterov_filtered = getindex.(filtered_nesterov, 1), getindex.(filtered_nesterov, 2)

# final_θ_momentum, iterations_momentum, trajectory_momentum = gradient_descent_with_momentum(f;
#                                                                        θ_init=theta_init,
#                                                                        learning_rate=learning_rate,
#                                                                        alpha=alpha,
#                                                                        max_iter=max_iters
#                                                                        )

# println("Momentum: Minimum found at: $final_θ_momentum after $iterations_momentum iterations")
# println("Function value at minimum: $(f(final_θ_momentum))")

# x_momentum, y_momentum = getindex.(trajectory_momentum, 1), getindex.(trajectory_momentum, 2)
# filtered_momentum = [(x, y) for (x, y) in zip(x_momentum, y_momentum) if plotxmin <= x <= plotxmax && plotxmin <= y <= plotxmax]
# x_momentum_filtered, y_momentum_filtered = getindex.(filtered_momentum, 1), getindex.(filtered_momentum, 2)

# final_θ_gd, iterations_gd, trajectory_gd = gradient_descent_classic(f;
#                                                              θ_init=theta_init,
#                                                              learning_rate=learning_rate,
#                                                              max_iter=max_iters)

# println("Gradient Descent: Minimum found at: $final_θ_gd after $iterations_gd iterations")
# println("Function value at minimum: $(f(final_θ_gd))")

# x_gd, y_gd = getindex.(trajectory_gd, 1), getindex.(trajectory_gd, 2)
# filtered_gd = [(x, y) for (x, y) in zip(x_gd, y_gd) if plotxmin <= x <= plotxmax && plotxmin <= y <= plotxmax]
# x_gd_filtered, y_gd_filtered = getindex.(filtered_gd, 1), getindex.(filtered_gd, 2)

# # function compute_absolute_error(f, trajectory, f_star)
# #     return [abs(f(θ) - f_star) for θ in trajectory]
# # end
# # f_star = minimum([f(θ) for θ in trajectory_nesterov])# Approximate f* as the minimum in trajectory
# # absolute_errors = compute_absolute_error(f, trajectory_nesterov, f_star)
# # absolute_errors2 = compute_absolute_error(f, trajectory_momentum, f_star)

# # # Clip errors to a small value to avoid log10 issues
# # absolute_errors = max.(absolute_errors, 1e-10)
# # absolute_errors2 = max.(absolute_errors2, 1e-10)
# # # Plot the error term over iterations
# # iterations = 1:length(absolute_errors)
# # iterations_momentum = 1:length(absolute_errors2)
# # plot(iterations, absolute_errors, xlabel="Iteracija", ylabel="Greška f(θ) - f*", yscale=:log10, title="Greška kroz iteracije", label="Nesterov Gradient Descent")
# # plot!(iterations_momentum, absolute_errors2, xlabel="Iteracija", ylabel="Greška f(θ) - f*", yscale=:log10, title="Greška kroz iteracije", label="Momentum Gradient Descent")

# xx = LinRange(plotxmin, plotxmax, 100)
# yy = LinRange(plotxmin, plotxmax, 100)
# zz = [f([x, y]) for x in xx, y in yy]

# contour(xx, yy, zz, xlabel="x", ylabel="y", title="f(x)=x^2+100y^2", color=:viridis, levels=50)

# scatter!(x_nesterov_filtered, y_nesterov_filtered, label="Nesterov", color=:blue, markersize=3, marker=:circle)
# scatter!(x_momentum_filtered, y_momentum_filtered, label="Momentum", color=:red, markersize=3, marker=:cross)
# # scatter!(x_gd_filtered, y_gd_filtered, label="Gradient Descent", color=:green, markersize=4, marker=:circle)
# max_value = maximum(final_theta_matrix2)
# min_value = minimum(final_theta_matrix2)

# # Find the indices (row, column) of the maximum value
# max_index = argmax(final_theta_matrix2)
# println(max_index)
# println(min_value)
# println(max_value)
# # xv = LinRange(plotxmin, plotxmax, 110)
# # yv = LinRange(plotxmin, plotxmax, 110)

# # final_theta_matrix = zeros(Float64, length(xv), length(yv))
# # final_theta_matrix2 = zeros(Float64, length(xv), length(yv))

# # # Iterate over all combinations of learning rates and alphas
# # for (i, x) in enumerate(xv)
# #     for (j, y) in enumerate(yv)
# #         point, _, _ = gradient_descent_with_momentum(
# #             f;
# #             θ_init=[x,y],
# #             learning_rate=learning_rate,
# #             alpha=alpha,
# #             max_iter=max_iters
# #         )
# #         point2, _, _ = nesterov_gradient_descent(
# #             f;
# #             θ_init=[x,y],
# #             learning_rate=learning_rate,
# #             alpha=alpha,
# #             max_iter=max_iters
# #         )
# #         final_theta_matrix[i, j] = f(point)  # Store x-coordinate
# #         final_theta_matrix2[i, j] = f(point2)  # Store x-coordinate
# #     end
# #     println("Row $i done")
# # end

scatter_x = repeat(xv, inner=length(yv))  # Repeat xv for all yv
scatter_y = repeat(yv, outer=length(xv))  # Repeat yv for all xv
scatter_color = vec(final_theta_matrix2')  # Flatten matrix into a vector

# Create scatter plot
scatter(
    scatter_x, 
    scatter_y, 
    zcolor=scatter_color,  # Color based on function value
    xlabel="x (početno)", 
    ylabel="y (početno)", 
    color=:viridis,
    markershape=:rect,  # Use rectangle (square) markers
    markersize=2,       # Adjust size as needed
    markerstrokecolor=:transparent,  # Ensure marker border is transparent
    markerfillcolor=:auto,
    markerstrokewidth=0,  # Ensure the fill color is applied correctly
    legend=false,
    aspect_ratio=:equal,
    colorbar=true,           # Enable colorbar
    size=(600,600)
)