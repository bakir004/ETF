import Pkg;
using JuMP
using GLPK

model = Model(GLPK.Optimizer)

@variable(model, x1 >= 0) 
@variable(model, x2 >= 0) 

@objective(model, Max, 2*x1 + 4*x2)

@constraint(model, odjeljak1, x1 <= 3)
@constraint(model, odjeljak2, x2 <= 6)
@constraint(model, odjeljak3, 3*x1 + 2*x2 <= 18)

optimize!(model)

if termination_status(model) == MOI.OPTIMAL
    println("  Proizvod 1: ", round(value(x1), digits=2), " komada")
    println("  Proizvod 2: ", round(value(x2), digits=2), " komada")
    println()
    println("MAKSIMALNA ZARADA: ", round(objective_value(model), digits=2), " KM")
else
    println("Rješenje nije pronađeno!")
    println("Status: ", termination_status(model))
end


