import Pkg;
using JuMP
using GLPK

model = Model(GLPK.Optimizer)

@variable(model, x1 >= 0) 
@variable(model, x2 >= 0) 

@objective(model, Max, 3*x1 + x2)

@constraint(model, ogr1, 0.5*x1 + 0.3*x2 <= 150)
@constraint(model, ogr2, 0.1*x1 + 0.2*x2 <= 60)

optimize!(model)

if termination_status(model) == MOI.OPTIMAL
    println("  x1 = ", round(value(x1), digits=2))
    println("  x2 = ", round(value(x2), digits=2))
    println()
    println("Z = ", round(objective_value(model), digits=2))
else
    println("Rješenje nije pronađeno!")
    println("Status: ", termination_status(model))
end

