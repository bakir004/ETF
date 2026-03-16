# Implementacija Simplex metode za rješavanje standardnog oblik LP problema
# Standardni oblik: max Z(x) = c^T x, p.o. Ax ≤ b, x ≥ 0

using Printf
using LinearAlgebra

function rijesi_simplex(A, b, c; verbose=true)
    """
    Rješava standardni oblik LP problema koristeći Simplex metodu.
    
    Parametri:
    - A: matrica koeficijenata ograničenja (m × n)
    - b: vektor desnih strana (m × 1)
    - c: vektor koeficijenata funkcije cilja (n × 1)
    - verbose: da li ispisivati iteracije (default: true)
    
    Povratna vrijednost:
    - x: optimalno rješenje (vektor)
    - Z: optimalna vrijednost funkcije cilja
    - status: "OPTIMAL", "UNBOUNDED", "DEGENERATE", ili "INFEASIBLE"
    """
    
    m, n = size(A)
    
    # Provjera da li su dimenzije kompatibilne
    if length(b) != m
        error("Dimenzije A i b nisu kompatibilne")
    end
    if length(c) != n
        error("Dimenzije A i c nisu kompatibilne")
    end
    
    # Provjera da li su svi elementi b nenegativni (za početnu bazu)
    if any(b .< 0)
        error("Svi elementi b moraju biti nenegativni za standardni oblik")
    end
    
    # Kreiranje proširene tabele (dodavanje slack varijabli)
    # Tabela ima oblik: [A | I | b] za ograničenja
    # Funkcija cilja: [-c | 0 | 0]
    
    # Početna tabela
    tableau = zeros(m + 1, n + m + 1)
    
    # Kopiranje A u tabelu
    tableau[1:m, 1:n] = A
    
    # Dodavanje jedinične matrice za slack varijable
    tableau[1:m, (n+1):(n+m)] = Matrix{Float64}(I, m, m)
    
    # Dodavanje b u posljednju kolonu
    tableau[1:m, end] = b
    
    # Dodavanje funkcije cilja (negativni znak jer je maksimizacija)
    tableau[end, 1:n] = -c
    tableau[end, (n+1):(n+m)] = zeros(m)
    tableau[end, end] = 0.0
    
    # Lista baznih varijabli (indeksi slack varijabli u početnoj bazi)
    basic_vars = collect(n+1:n+m)
    
    iteration = 0
    
    if verbose
        println("="^80)
        println("POČETNA SIMPLEX TABELA")
        println("="^80)
        print_tableau(tableau, basic_vars, n, m, iteration)
    end
    
    max_iterations = 1000  # Zaštita od beskonačne petlje
    
    while iteration < max_iterations
        iteration += 1
        
        # Provjera optimalnosti - ako su svi koeficijenti u redu funkcije cilja nenegativni
        reduced_costs = tableau[end, 1:(n+m)]
        
        if all(reduced_costs .>= -1e-10)  # Koristimo malu toleranciju za greške zaokruživanja
            # Optimalno rješenje pronađeno
            x = zeros(n)
            
            # Izdvajanje vrijednosti originalnih varijabli
            for i in 1:m
                if basic_vars[i] <= n
                    x[basic_vars[i]] = tableau[i, end]
                end
            end
            
            Z = tableau[end, end]
            
            # Provjera degeneracije
            degenerate = false
            for i in 1:m
                if abs(tableau[i, end]) < 1e-10
                    degenerate = true
                    break
                end
            end
            
            if verbose
                println("\n" * "="^80)
                println("OPTIMALNO RJEŠENJE PRONAĐENO nakon $iteration iteracija")
                if degenerate
                    println("NAPOMENA: Rješenje je DEGENERISANO (neka bazna varijabla je 0)")
                end
                println("="^80)
            end
            
            status = degenerate ? "DEGENERATE" : "OPTIMAL"
            return x, Z, status
        end
        
        # Određivanje ulazne varijable (najnegativniji koeficijent u redu funkcije cilja)
        entering_idx = argmin(reduced_costs)
        
        if reduced_costs[entering_idx] >= -1e-10
            # Nema negativnih koeficijenata - optimalno
            continue
        end
        
        # Provjera da li je problem neograničen (svi koeficijenti u koloni su nenegativni)
        pivot_col = tableau[1:m, entering_idx]
        
        if all(pivot_col .<= 1e-10)
            if verbose
                println("\n" * "="^80)
                println("PROBLEM JE NEOGRANIČEN - beskonačno rješenje")
                println("="^80)
            end
            return zeros(n), Inf, "UNBOUNDED"
        end
        
        # Određivanje izlazne varijable (minimum test ratio)
        ratios = zeros(m)
        for i in 1:m
            if pivot_col[i] > 1e-10
                ratios[i] = tableau[i, end] / pivot_col[i]
            else
                ratios[i] = Inf
            end
        end
        
        # Provjera za degeneraciju (više minimuma)
        min_ratio = minimum(ratios)
        leaving_candidates = findall(x -> abs(x - min_ratio) < 1e-10, ratios)
        
        if length(leaving_candidates) > 1
            if verbose && iteration == 1
                println("\nNAPOMENA: Degeneracija detektovana - više kandidata za izlaznu varijablu")
            end
        end
        
        leaving_row = leaving_candidates[1]  # Uzmi prvi
        
        # Ažuriranje baznih varijabli
        basic_vars[leaving_row] = entering_idx
        
        # Pivot operacija
        pivot_element = tableau[leaving_row, entering_idx]
        
        # Normalizacija pivot reda
        tableau[leaving_row, :] ./= pivot_element
        
        # Eliminacija u ostalim redovima
        for i in 1:(m+1)
            if i != leaving_row
                multiplier = tableau[i, entering_idx]
                tableau[i, :] .-= multiplier .* tableau[leaving_row, :]
            end
        end
        
        if verbose
            println("\n" * "-"^80)
            println("ITERACIJA $iteration")
            println("-"^80)
            println("Ulazna varijabla: x$(entering_idx <= n ? entering_idx : "s$(entering_idx-n)")")
            println("Izlazna varijabla: x$(basic_vars[leaving_row] <= n ? basic_vars[leaving_row] : "s$(basic_vars[leaving_row]-n)")")
            print_tableau(tableau, basic_vars, n, m, iteration)
        end
    end
    
    if verbose
        println("\nMaksimalni broj iteracija dostignut!")
    end
    
    return zeros(n), 0.0, "MAX_ITERATIONS"
end

function print_tableau(tableau, basic_vars, n, m, iteration)
    """Pomoćna funkcija za ispis Simplex tabele"""
    
    rows, cols = size(tableau)
    
    # Header
    print("\nBazna var | ")
    for j in 1:(n+m)
        if j <= n
            print("x$j")
        else
            print("s$(j-n)")
        end
        print(" | ")
    end
    println("RHS")
    println("-"^80)
    
    # Redovi ograničenja
    for i in 1:m
        if basic_vars[i] <= n
            print("x$(basic_vars[i])       | ")
        else
            print("s$(basic_vars[i]-n)       | ")
        end
        
        for j in 1:(n+m+1)
            val = tableau[i, j]
            if abs(val) < 1e-10
                print("0.0     ")
            else
                print(@sprintf("%8.4f", val))
            end
            print(" | ")
        end
        println()
    end
    
    # Red funkcije cilja
    println("-"^80)
    print("Z         | ")
    for j in 1:(n+m+1)
        val = tableau[end, j]
        if abs(val) < 1e-10
            print("0.0     ")
        else
            print(@sprintf("%8.4f", val))
        end
        print(" | ")
    end
    println()
    println()
end

# Test primjeri

println("\n" * "="^80)
println("TEST PRIMJER 1: Standardni LP problem")
println("max Z = 3x1 + 2x2")
println("p.o. x1 + x2 <= 4")
println("     2x1 + x2 <= 6")
println("     x1, x2 >= 0")
println("="^80)
A1 = [1.0 1.0; 2.0 1.0]
b1 = [4.0, 6.0]
c1 = [3.0, 2.0]
x1, Z1, status1 = rijesi_simplex(A1, b1, c1)
println("\nRezultat:")
println("x = ", x1)
println("Z = ", Z1)
println("Status: ", status1)

println("\n" * "="^80)
println("TEST PRIMJER 2: Degeneracija")
println("max Z = x1 + x2")
println("p.o. x1 + x2 <= 2")
println("     2x1 + x2 <= 4")
println("     x1 <= 2")
println("     x1, x2 >= 0")
println("="^80)
A2 = [1.0 1.0; 2.0 1.0; 1.0 0.0]
b2 = [2.0, 4.0, 2.0]
c2 = [1.0, 1.0]
x2, Z2, status2 = rijesi_simplex(A2, b2, c2)
println("\nRezultat:")
println("x = ", x2)
println("Z = ", Z2)
println("Status: ", status2)

println("\n" * "="^80)
println("TEST PRIMJER 3: Beskonačno rješenje (neograničen problem)")
println("max Z = x1 + x2")
println("p.o. -x1 + x2 <= 1")
println("     x1 - x2 <= 1")
println("     x1, x2 >= 0")
println("="^80)
A3 = [-1.0 1.0; 1.0 -1.0]
b3 = [1.0, 1.0]
c3 = [1.0, 1.0]
x3, Z3, status3 = rijesi_simplex(A3, b3, c3)
println("\nRezultat:")
println("x = ", x3)
println("Z = ", Z3)
println("Status: ", status3)

println("\n" * "="^80)
println("PRIMJER: Fabrika pločica (P1, P2, P3) - standardni LP problem")
println("max Z = 23*x1 + 17*x2 + 17*x3")
println("p.o.  x1 + x2 + 5*x3 <= 9600      # (preračunato: 0.01*x1 + 0.01*x2 + 0.05*x3 <= 96, množenjem sa 100)")
println("      15*x1 + 19*x2 + 18*x3 <= 53324")
println("      x1, x2, x3 >= 0")
println("="^80)

A1 = [1.0  1.0  5.0;
      15.0 19.0 18.0]

b1 = [9600.0, 53324.0]

c1 = [23.0, 17.0, 17.0]

x1, Z1, status1 = rijesi_simplex(A1, b1, c1)

println("\nRezultat:")
println("x = ", x1)        # vektor [x1, x2, x3]
println("Z = ", Z1)
println("Status: ", status1)

println("\n" * "="^80)
println("PRIMJER: Standardni LP problem")
println("max Z = 16.8*x1 + 21.6*x2 + 19.2*x3")
println("p.o.  2.1*x1 + 2.7*x2 + 2.4*x3 <= 9.45")
println("      x1 + x2 + x3 <= 21.2")
println("      x1, x2, x3 >= 0")
println("="^80)

A1 = [2.1  2.7  2.4;
      1.0  1.0  1.0]

b1 = [9.45, 21.2]

c1 = [16.8, 21.6, 19.2]

x1, Z1, status1 = rijesi_simplex(A1, b1, c1)

println("\nRezultat:")
println("x = ", x1)        # vektor [x1, x2, x3]
println("Z = ", Z1)
println("Status: ", status1)

println("\n" * "="^80)
println("PRIMJER: Standardni LP problem")
println("max Z = 23*x1 + 17*x2 + 17*x3")
println("p.o.  15*x1 + 19*x2 + 18*x3 <= 53324")
println("      x1 + x2 + 5*x3 <= 9600")
println("      x1, x2, x3 >= 0")
println("="^80)

A_example = [15.0  19.0  18.0;
             1.0   1.0   5.0]

b_example = [53324.0, 9600.0]

c_example = [23.0, 17.0, 17.0]

x_example, Z_example, status_example = rijesi_simplex(A_example, b_example, c_example)

println("\nRezultat:")
println("x = ", x_example)        # vektor [x1, x2, x3]
println("Z = ", Z_example)
println("Status: ", status_example)