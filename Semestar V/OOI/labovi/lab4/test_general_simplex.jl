# Test primjeri za general_simplex funkciju
# Laboratorijska vježba 5

include(joinpath(@__DIR__, "..", "lab5", "generalni_simplex.jl"))

println("=" ^ 80)
println("TEST PRIMJERI ZA GENERAL_SIMPLEX")
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
println("Optimalna vrijednost: Z = $Z1")
println("Optimalno rješenje: X = $X1")
println("Izravnavajuće varijable: Xd = $Xd1")
println("Dualne varijable (cijene u sjeni): Y = $Y1")
println("Reducirane cijene: Yd = $Yd1")
println()

# ============================================================================
# PRIMJER 2: Problem minimizacije sa ≥ ograničenjima
# ============================================================================
println("PRIMJER 2: Problem minimizacije sa ≥ ograničenjima")
println("-" ^ 80)
println("Minimizirati: Z = 3x1 + 2x2")
println("Ograničenja:")
println("  x1 + x2 ≥ 4")
println("  2x1 + x2 ≥ 6")
println("  x1, x2 ≥ 0")
println()

c2 = [3 2]
A2 = [1 1; 2 1]
b2 = [4, 6]
csigns2 = [1, 1]    # ≥ ograničenja
vsigns2 = [1, 1]    # nenegativne varijable

Z2, X2, Xd2, Y2, Yd2, status2 = general_simplex("min", c2, A2, b2, csigns2, vsigns2)

println("Status: $status2")
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
println("Optimalna vrijednost: Z = $Z3")
println("Optimalno rješenje: X = $X3")
println("Izravnavajuće varijable: Xd = $Xd3")
println("Dualne varijable (cijene u sjeni): Y = $Y3")
println("Reducirane cijene: Yd = $Yd3")
println()

println("=" ^ 80)

