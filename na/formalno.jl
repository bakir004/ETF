using ForwardDiff
using LinearAlgebra

function rosenbrock_function(x)
  return (1 - x[1])^2 + 100 * (x[2] - x[1]^2)^2
end

function nesterov_gradient_descent(f, x_init, learning_rate=0.001, alpha=0.9, max_iter=10000, tol=1e-6)
    x = x_init .+ 0.01 * randn(length([x_init]))       
    v = zeros(length(x)) 
    iter = 0
    correct = false

    while iter < max_iter
        x_tilde = x + alpha * v
        g = gradient(f, x_tilde)
        v = alpha * v - learning_rate * g
        x += v

        if norm(g) < tol
            correct = true
            break
        end

        iter += 1
    end

    return x, iter, correct
end

final_x, iterations, correct = nesterov_gradient_descent(rosenbrock_function, [0.0, 0.0], 0.001, 0.9, 10000, 1e-6)
                                                     
println("Nesterov: Minimum pronadjen u tacki $final_x nakon $iterations iteracija")
println("Vrijednost funkcije u minimumu: $(f(final_x)), dostignuta tacnost: $correct")