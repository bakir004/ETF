
using JuMP
using GLPK

"""
    min_tree(W)

Pronalazi minimalno povezujuće stablo pomoću linearnog programiranja.

# Argumenti
- `W`: Težinska matrica (n×n). W[i,j] je dužina grane između čvora i i čvora j.
       W[i,j] = Inf označava da grana ne postoji.
       W[i,i] = 0 (dijagonala).

# Povratne vrijednosti
- `T`: Matrica (m×2) gdje svaki red predstavlja jednu granu minimalnog stabla.
       Prva kolona = početni čvor, druga kolona = krajnji čvor.
- `V`: Težina (dužina) minimalnog povezujućeg stabla.
"""
function min_tree(W)
    n = size(W, 1)
    
    model = Model(GLPK.Optimizer)
    
    @variable(model, x[i=1:n, j=1:n; i < j && W[i,j] < Inf], Int, lower_bound=0, upper_bound=1)
    
    @objective(model, Min, sum(W[i,j] * x[i,j] for i in 1:n for j in (i+1):n if W[i,j] < Inf))
    
    @constraint(model, sum(x[i,j] for i in 1:n for j in (i+1):n if W[i,j] < Inf) == n - 1)
    
    svi_podskupovi = []
    for mask in 1:(2^n - 2)
        podskup = [i for i in 1:n if (mask & (1 << (i-1))) != 0]
        if length(podskup) > 0 && length(podskup) < n
            push!(svi_podskupovi, podskup)
        end
    end
    
    for S in svi_podskupovi
        komplement_S = [i for i in 1:n if !(i in S)]
        grane_izmedju = []
        for i in S
            for j in komplement_S
                if i < j && W[i,j] < Inf
                    push!(grane_izmedju, x[i,j])
                elseif j < i && W[j,i] < Inf
                    push!(grane_izmedju, x[j,i])
                end
            end
        end
        if length(grane_izmedju) > 0
            @constraint(model, sum(grane_izmedju) >= 1)
        end
    end
    
    optimize!(model)
    
    if termination_status(model) != MOI.OPTIMAL
        error("Problem nije riješen optimalno! Status: ", termination_status(model))
    end
    
    grane = []
    for i in 1:n
        for j in (i+1):n
            if W[i,j] < Inf
                if value(x[i,j]) > 0.5
                    push!(grane, [i, j])
                end
            end
        end
    end
    
    sort!(grane, by = g -> (g[1], g[2]))
    
    if length(grane) > 0
        T = hcat([g[1] for g in grane], [g[2] for g in grane])
    else
        T = zeros(Int, 0, 2)
    end
    
    V = objective_value(model)
    
    return T, V
end

println("="^80)
println("ZADATAK 7 - MINIMALNO POVEZUJUĆE STABLO (MST)")
println("="^80)
println("\n--- TESTNI PRIMJER 1 (Primjer iz zadatka) ---")
W1 = [0 2 3 Inf 8 Inf Inf Inf;
      2 0 4 Inf 9 Inf Inf Inf;
      3 4 0 7 Inf Inf Inf Inf;
      Inf Inf 7 0 4 3 Inf Inf;
      8 Inf Inf 4 0 5 5 Inf;
      Inf 9 Inf 3 5 0 7 6;
      Inf Inf Inf Inf 5 7 0 1;
      Inf Inf Inf Inf Inf 6 1 0]

println("Težinska matrica W:")
display(W1)

T1, V1 = min_tree(W1)

println("\nRezultat:")
println("T =")
display(T1)
println("V = ", V1)

println("\nOčekivano:")
println("T = [7 8; 1 2; 1 3; 4 6; 4 5; 5 7; 3 4]")
println("V = 25")

println("\n\n--- TESTNI PRIMJER 2 (Graf sa 4 čvora) ---")
W2 = [0 1 3 4;
      1 0 2 Inf;
      3 2 0 5;
      4 Inf 5 0]

println("Težinska matrica W:")
display(W2)

T2, V2 = min_tree(W2)

println("\nRezultat:")
println("T =")
display(T2)
println("V = ", V2)

println("\nOčekivano: Minimalno stablo bi trebalo imati 3 grane (4-1=3)")
println("Moguće rješenje: [1 2; 2 3; 3 4] ili [1 2; 2 3; 1 4]")
println("Težina bi trebala biti: 1 + 2 + 5 = 8 ili 1 + 2 + 4 = 7")

println("\n\n--- TESTNI PRIMJER 3 (Kompletan graf sa 3 čvora) ---")
W3 = [0 5 3;
      5 0 2;
      3 2 0]

println("Težinska matrica W:")
display(W3)

T3, V3 = min_tree(W3)

println("\nRezultat:")
println("T =")
display(T3)
println("V = ", V3)

println("\nOčekivano: Minimalno stablo bi trebalo imati 2 grane (3-1=2)")
println("Moguće rješenje: [2 3; 1 3] (težina: 2 + 3 = 5)")
println("Ili: [1 2; 2 3] (težina: 5 + 2 = 7) - nije optimalno")
println("Ili: [1 3; 2 3] (težina: 3 + 2 = 5) - optimalno")

println("\n\n--- TESTNI PRIMJER 4 (Graf sa 5 čvorova) ---")
W4 = [0 2 3 Inf Inf;
      2 0 1 4 Inf;
      3 1 0 5 6;
      Inf 4 5 0 7;
      Inf Inf 6 7 0]

println("Težinska matrica W:")
display(W4)

T4, V4 = min_tree(W4)

println("\nRezultat:")
println("T =")
display(T4)
println("V = ", V4)

println("\nOčekivano: Minimalno stablo bi trebalo imati 4 grane (5-1=4)")
println("Moguće rješenje: [1 2; 2 3; 2 4; 3 5] (težina: 2 + 1 + 4 + 6 = 13)")

println("\n" * "="^80)
println("TESTIRANJE ZAVRŠENO")
println("="^80)

