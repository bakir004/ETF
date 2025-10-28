import Pkg;
using JuMP
using GLPK

model = Model(GLPK.Optimizer)

@variable(model, x1 >= 0) 
@variable(model, x2 >= 0) 

@objective(model, Max, 3*x1 + 7*x2)

@constraint(model, sirovina, 0.25*x1 + 0.75*x2 <= 20)
@constraint(model, trziste1, x1 <= 10)
@constraint(model, trziste2, x2 <= 9)

optimize!(model)

if termination_status(model) == MOI.OPTIMAL
    println("  Proizvod 1: ", round(value(x1), digits=2), " kg")
    println("  Proizvod 2: ", round(value(x2), digits=2), " kg")
    println()
    println("MAKSIMALNA DOBIT: ", round(objective_value(model), digits=2), " KM")
else
    println("Rješenje nije pronađeno!")
    println("Status: ", termination_status(model))
end

