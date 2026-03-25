import Pkg;
using JuMP
using GLPK

model = Model(GLPK.Optimizer)

@variable(model, x1 >= 0) 
@variable(model, x2 >= 0) 

@objective(model, Min, -7x1-2x2)

@constraint(model, c1, 6x1+3x2<=2)
@constraint(model, c2, 5x1+1x2<=5)
@constraint(model, c3, 5x1+6x2<=3)

optimize!(model)

if termination_status(model) == MOI.OPTIMAL
    println("  Deterdžent D1: ", round(value(x1), digits=2), " kg")
    println("  Deterdžent D2: ", round(value(x2), digits=2), " kg")
    println()
    println("MAKSIMALNA ZARADA: ", round(objective_value(model), digits=2), " KM")
else
    println("Rješenje nije pronađeno!")
    println("Status: ", termination_status(model))
end
