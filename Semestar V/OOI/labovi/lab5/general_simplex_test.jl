# Test primjeri za general_simplex funkciju
# Laboratorijska vježba 5

include(joinpath(@__DIR__, "generalni_simplex.jl"))
include(joinpath(@__DIR__, "..", "lab4", "rijesi_simplex.jl"))

println("=" ^ 80)
println("TEST PRIMJERI ZA GENERAL_SIMPLEX - LABORATORIJSKA VJEŽBA 5")
println("=" ^ 80)
println()

# ============================================================================
# PRIMJER 1: Problem maksimizacije sa ≤ ograničenjima
# ============================================================================
println("PRIMJER 1: Problem maksimizacije sa ≤ ograničenjima")
println("-" ^ 80)
println("Maksimizirati: Z = 3x1 + 2x2")
println("Ograničenja:")
println("  x1 + x2 ≤ 4")
println("  2x1 + x2 ≤ 6")
println("  x1, x2 ≥ 0")
println()

c1 = [3 2]
A1 = [1 1; 2 1]
b1 = [4, 6]
csigns1 = [-1, -1]  # ≤ ograničenja
vsigns1 = [1, 1]    # nenegativne varijable

Z1, X1, Xd1, Y1, Yd1, status1 = general_simplex("max", c1, A1, b1, csigns1, vsigns1)

println("Status: $status1")
if status1 == 0
    println("  → Jedinstveno nedegenerirano optimalno rješenje")
elseif status1 == 1
    println("  → Jedinstveno degenerirano optimalno rješenje")
elseif status1 == 2
    println("  → Optimalno rješenje postoji, ali nije jedinstveno")
elseif status1 == 3
    println("  → Rješenje je neograničeno")
elseif status1 == 4
    println("  → Dopustiva oblast ne postoji")
elseif status1 == 5
    println("  → Greška u parametrima")
end
println("Optimalna vrijednost: Z = $Z1")
println("Optimalno rješenje: X = $X1")
println("Izravnavajuće varijable (slack): Xd = $Xd1")
println("Dualne varijable (cijene u sjeni): Y = $Y1")
println("Reducirane cijene: Yd = $Yd1")
println()

# ============================================================================
# PRIMJER 2: Problem minimizacije sa ≥ ograničenjima (sa iteracijama)
# ============================================================================
println("PRIMJER 2: Problem minimizacije sa ≥ ograničenjima")
println("-" ^ 80)
println("Minimizirati: Z = 3x1 + 2x2")
println("Ograničenja:")
println("  x1 + x2 ≥ 4")
println("  2x1 + x2 ≥ 6")
println("  x1, x2 ≥ 0")
println()
println("Ovaj primjer će biti riješen ručno i upoređen sa izlazom funkcije.")
println()

c2 = [3 2]
A2 = [1 1; 2 1]
b2 = [4, 6]
csigns2 = [1, 1]    # ≥ ograničenja
vsigns2 = [1, 1]    # nenegativne varijable

# Rješavanje sa iteracijama
println("Rješavanje pomoću rijesi_simplex_sa_iteracijama:")
println()
try
    Z2_iter, X2_iter, jedinstveno2, degenerirano2 = rijesi_simplex_sa_iteracijama(
        "min", A2, b2, c2, csigns2, vsigns2
    )
    println("\n=== REZULTAT SA ITERACIJAMA ===")
    println("Optimalna vrijednost: Z = $Z2_iter")
    println("Optimalno rješenje: X = $X2_iter")
    println(jedinstveno2)
    println(degenerirano2)
catch e
    println("Greška: $e")
end
println()

# Rješavanje pomoću general_simplex
Z2, X2, Xd2, Y2, Yd2, status2 = general_simplex("min", c2, A2, b2, csigns2, vsigns2)

println("=== REZULTAT SA GENERAL_SIMPLEX ===")
println("Status: $status2")
if status2 == 0
    println("  → Jedinstveno nedegenerirano optimalno rješenje")
elseif status2 == 1
    println("  → Jedinstveno degenerirano optimalno rješenje")
elseif status2 == 2
    println("  → Optimalno rješenje postoji, ali nije jedinstveno")
end
println("Optimalna vrijednost: Z = $Z2")
println("Optimalno rješenje: X = $X2")
println("Izravnavajuće varijable: Xd = $Xd2")
println("Dualne varijable (cijene u sjeni): Y = $Y2")
println("Reducirane cijene: Yd = $Yd2")
println()

# ============================================================================
# PRIMJER 3: Problem sa neograničenom varijablom
# ============================================================================
println("PRIMJER 3: Problem sa neograničenom varijablom")
println("-" ^ 80)
println("Maksimizirati: Z = 2x1 + 3x2")
println("Ograničenja:")
println("  x1 + x2 ≤ 5")
println("  x1 - x2 ≤ 2")
println("  x1 ≥ 0, x2 neograničeno")
println()

c3 = [2 3]
A3 = [1 1; 1 -1]
b3 = [5, 2]
csigns3 = [-1, -1]  # ≤ ograničenja
vsigns3 = [1, 0]    # x1 ≥ 0, x2 neograničeno

Z3, X3, Xd3, Y3, Yd3, status3 = general_simplex("max", c3, A3, b3, csigns3, vsigns3)

println("Status: $status3")
if status3 == 0
    println("  → Jedinstveno nedegenerirano optimalno rješenje")
elseif status3 == 1
    println("  → Jedinstveno degenerirano optimalno rješenje")
elseif status3 == 2
    println("  → Optimalno rješenje postoji, ali nije jedinstveno")
elseif status3 == 3
    println("  → Rješenje je neograničeno")
elseif status3 == 4
    println("  → Dopustiva oblast ne postoji")
end
println("Optimalna vrijednost: Z = $Z3")
println("Optimalno rješenje: X = $X3")
println("Izravnavajuće varijable (slack): Xd = $Xd3")
println("Dualne varijable (cijene u sjeni): Y = $Y3")
println("Reducirane cijene: Yd = $Yd3")
println()

# ============================================================================
# PRIMJER 4: Problem koji nema rješenje
# ============================================================================
println("PRIMJER 4: Problem koji nema rješenje (nepostojanje dopustive oblasti)")
println("-" ^ 80)
println("Maksimizirati: Z = x1 + 2x2")
println("Ograničenja:")
println("  x1 + x2 ≤ 2")
println("  3x1 + 3x2 ≥ 9")
println("  x1, x2 ≥ 0")
println()

c4 = [1 2]
A4 = [1 1; 3 3]
b4 = [2, 9]
csigns4 = [-1, 1]  # ≤ i ≥ ograničenja
vsigns4 = [1, 1]   # nenegativne varijable

Z4, X4, Xd4, Y4, Yd4, status4 = general_simplex("max", c4, A4, b4, csigns4, vsigns4)

println("Status: $status4")
if status4 == 4
    println("  → Dopustiva oblast ne postoji (očekivano)")
end
println("Optimalna vrijednost: Z = $Z4")
println()

# ============================================================================
# PRIMJER 5: Problem sa neograničenim rješenjem
# ============================================================================
println("PRIMJER 5: Problem sa neograničenim rješenjem")
println("-" ^ 80)
println("Maksimizirati: Z = x1 + x2")
println("Ograničenja:")
println("  x1 - x2 ≤ 1")
println("  -x1 + x2 ≤ 1")
println("  x1, x2 ≥ 0")
println()

c5 = [1 1]
A5 = [1 -1; -1 1]
b5 = [1, 1]
csigns5 = [-1, -1]  # ≤ ograničenja
vsigns5 = [1, 1]    # nenegativne varijable

Z5, X5, Xd5, Y5, Yd5, status5 = general_simplex("max", c5, A5, b5, csigns5, vsigns5)

println("Status: $status5")
if status5 == 3
    println("  → Rješenje je neograničeno (očekivano)")
elseif status5 == 0
    println("  → Rješenje je ograničeno")
    println("Optimalna vrijednost: Z = $Z5")
    println("Optimalno rješenje: X = $X5")
end
println()

println("=" ^ 80)
println("ZAVRŠETAK TESTIRANJA")
println("=" ^ 80)

