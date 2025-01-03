using Plots

func(x) = sin(x[1]) * x[1] + cos(5*x[1])*cos(x[1])

x = 4:0.01:6.1

function gradient_descent_with_momentum(f, θ_init, learning_rate=0.001, alpha=0.9, max_iter=10000, tol=1e-6)
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

final_x, iterations, correct = nesterov_gradient_descent(func, [6], 0.001, 0.9, 10000, 1e-6)
final_x2, iterations2, correct2 = gradient_descent_with_momentum(func, [6], 0.001, 0.9, 10000, 1e-6)
                                                     
println("Nesterov: Minimum pronadjen u tacki $final_x nakon $iterations iteracija")
println("Vrijednost funkcije u minimumu: $(func(final_x)), dostignuta tacnost: $correct")

println("Momentum: Minimum pronadjen u tacki $final_x2 nakon $iterations2 iteracija")
println("Vrijednost funkcije u minimumu: $(func(final_x2)), dostignuta tacnost:")

plot(x, g.(x), label="sin(x)*x+cos(5x)*cos(x)", xlabel="x", ylabel="f(x)")
scatter!([5.48], [func(5.48)], color=:red, label="Nesterov", markersize=4)
scatter!([4.78], [func(4.78)], color=:lime, label="Momentum", markersize=4)