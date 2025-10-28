import Pkg;
using JuMP
using GLPK

model = Model(GLPK.Optimizer)

@variable(model, x1 >= 0) 
@variable(model, x2 >= 0) 
@variable(model, x3 >= 0)  

@objective(model, Max, 2*x1 + 3*x2 + 1*x3)

@constraint(model, sirovina1, 2*x1 + 2*x2 + 2*x3 <= 4)
@constraint(model, sirovina2, 3*x1 + 3*x2 <= 2)
@constraint(model, sirovina3, x2 + x3 <= 3)

optimize!(model)

if termination_status(model) == MOI.OPTIMAL
    println("  Proizvod P1: ", round(value(x1), digits=2), " jedinica")
    println("  Proizvod P2: ", round(value(x2), digits=2), " jedinica")
    println("  Proizvod P3: ", round(value(x3), digits=2), " jedinica")
    println()
    println("MAKSIMALNA DOBIT: ", round(objective_value(model), digits=2), " novčanih jedinica")
else
    println("Rješenje nije pronađeno!")
    println("Status: ", termination_status(model))
end

