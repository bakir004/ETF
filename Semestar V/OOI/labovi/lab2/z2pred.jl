import Pkg;
using JuMP
using GLPK

model = Model(GLPK.Optimizer)

@variable(model, x1 >= 0) 
@variable(model, x2 >= 0) 

@objective(model, Max, 1500*x1 + 300*x2)

@constraint(model, gotovina, 500*x1 - 200*x2 <= 3000)
@constraint(model, vrijeme, 0.5*x1 + 0.25*x2 <= 12)

optimize!(model)

if termination_status(model) == MOI.OPTIMAL
    println("  Prva vrsta tereta: ", round(value(x1), digits=2), " tona")
    println("  Druga vrsta tereta: ", round(value(x2), digits=2), " tona")
    println()
    println("MAKSIMALNA ZARADA (NETO): ", round(objective_value(model), digits=2), " KM")
    println()
    println("DETALJI FINANSIJA:")
    zarada_prevoz1 = 2000*value(x1)
    zarada_prevoz2 = 100*value(x2)
    taksa = 500*value(x1)
    predujam = 200*value(x2)
    println("  Zarada od prevoza 1. vrste: ", round(zarada_prevoz1, digits=2), " KM")
    println("  Zarada od prevoza 2. vrste: ", round(zarada_prevoz2, digits=2), " KM")
    println("  Taksa za 1. vrstu (trošak): -", round(taksa, digits=2), " KM")
    println("  Predujam od 2. vrste (dobit): +", round(predujam, digits=2), " KM")
    println("  -----------------------------------------")
    println("  UKUPNO NETO: ", round(zarada_prevoz1 + zarada_prevoz2 - taksa + predujam, digits=2), " KM")
    println()
    println("GOTOVINA ZA UTOVAR:")
    potrebna_gotovina = taksa - predujam
    println("  Potrebna gotovina: ", round(potrebna_gotovina, digits=2), " KM (od max 3000 KM)")
    println()
    println("VRIJEME UTOVARA:")
    ukupno_vrijeme = 0.5*value(x1) + 0.25*value(x2)
    println("  Ukupno vrijeme: ", round(ukupno_vrijeme, digits=2), " h (od max 12h)")
else
    println("Rješenje nije pronađeno!")
    println("Status: ", termination_status(model))
end

