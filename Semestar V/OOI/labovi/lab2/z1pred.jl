import Pkg;
using JuMP
using GLPK

model = Model(GLPK.Optimizer)

@variable(model, x1 >= 0) 
@variable(model, x2 >= 0) 
@variable(model, x3 >= 0)  

@objective(model, Max, 8*x1 + 3.5*x2 + 7.5*x3)

@constraint(model, isporuka, x1 + x2 + x3 == 100)
@constraint(model, budzet, 2*x1 + 1.5*x2 + 0.5*x3 <= 110)
@constraint(model, radni_sati, 2*x1 + x2 + x3 >= 120)

optimize!(model)

if termination_status(model) == MOI.OPTIMAL
    println("  Deterdžent D1: ", round(value(x1), digits=2), " kg")
    println("  Deterdžent D2: ", round(value(x2), digits=2), " kg")
    println("  Deterdžent D3: ", round(value(x3), digits=2), " kg")
    println()
    println("MAKSIMALNA ZARADA: ", round(objective_value(model), digits=2), " KM")
    println()
    println("TROŠKOVI I PRIHODI:")
    trosak = 2*value(x1) + 1.5*value(x2) + 0.5*value(x3)
    prihod = 10*value(x1) + 5*value(x2) + 8*value(x3)
    println("  Trošak repromaterijala: ", round(trosak, digits=2), " \$")
    println("  Ukupan prihod: ", round(prihod, digits=2), " KM")
    println()
    println("RADNI SATI:")
    sati = 2*value(x1) + value(x2) + value(x3)
    println("  Ukupno angažovano: ", round(sati, digits=2), " h")
else
    println("Rješenje nije pronađeno!")
    println("Status: ", termination_status(model))
end
