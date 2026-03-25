# Rješavanje problema optimizacije prehrane pomoću simpleks metode
# Student želi organizirati zdravu prehranu konzumirajući piletinu i salatu

include("rijesi_simplex.jl")

println("=" ^ 80)
println("PROBLEM OPTIMIZACIJE PREHRANE")
println("=" ^ 80)
println()
println("Cilj: Minimizirati troškove uz zadovoljavanje nutritivnih zahtjeva")
println()
println("Varijable:")
println("  x1 = broj porcija piletine")
println("  x2 = broj porcija salate")
println()
println("Funkcija cilja (minimizirati):")
println("  Z = 3.5*x1 + 4.7*x2")
println()
println("Ograničenja:")
println("  Protein:     5.8*x1 + 4.5*x2 ≥ 169.9")
println("  Masti:       3.5*x1 + 0.5*x2 ≤ 550")
println("  Ugljikohidrati: 5.5*x1 + 4.5*x2 = 169.9")
println("  x1, x2 ≥ 0")
println()

# Formulacija problema
# Matrica A: koeficijenti ograničenja
# Red 1: Protein (≥)
# Red 2: Masti (≤)
# Red 3: Ugljikohidrati (=)
A = [5.8  4.5;    # Protein
     3.5  0.5;    # Masti
     5.5  4.5]    # Ugljikohidrati

# Vektor b: desne strane ograničenja
b = [169.9;       # Minimalna količina proteina
     550.0;       # Maksimalna količina masti
     169.9]       # Tačna količina ugljikohidrata

# Vektor c: koeficijenti funkcije cilja
c = [3.5  4.7]    # Cijene po porciji

# csigns: znakovi ograničenja
# 1 = ≥ (veće ili jednako)
# -1 = ≤ (manje ili jednako)
# 0 = = (jednako)
csigns = [1;      # Protein: ≥
         -1;      # Masti: ≤
          0]      # Ugljikohidrati: =

# vsigns: znakovi varijabli
# 1 = ≥ 0 (nenegativne)
vsigns = [1, 1]   # x1, x2 ≥ 0

goal = "min"

println("=" ^ 80)
println("RJEŠAVANJE POMOĆU SIMPLEKS METODE")
println("=" ^ 80)
println()

try
    Z_opt, X_opt, jedinstveno, degenerirano = rijesi_simplex_sa_iteracijama(
        goal, A, b, c, csigns, vsigns
    )
    
    println("\n" * "=" ^ 80)
    println("KONAČNO RJEŠENJE")
    println("=" ^ 80)
    println()
    println("Optimalna vrijednost funkcije cilja: Z = ", Z_opt, " KM")
    println()
    println("Optimalno rješenje:")
    println("  x1 (porcije piletine) = ", X_opt[1])
    println("  x2 (porcije salate)   = ", X_opt[2])
    println()
    println(jedinstveno)
    println(degenerirano)
    println()
    
    # Analiza rezervi i viškova
    println("=" ^ 80)
    println("ANALIZA REZERVI I VIŠKOVA")
    println("=" ^ 80)
    println()
    
    # Izračun stvarnih vrijednosti
    protein_stvarno = 5.8 * X_opt[1] + 4.5 * X_opt[2]
    masti_stvarno = 3.5 * X_opt[1] + 0.5 * X_opt[2]
    ugljikohidrati_stvarno = 5.5 * X_opt[1] + 4.5 * X_opt[2]
    
    println("Stvarne vrijednosti pri optimalnom rješenju:")
    println("  Protein:      ", protein_stvarno, " jedinica")
    println("  Masti:        ", masti_stvarno, " jedinica")
    println("  Ugljikohidrati: ", ugljikohidrati_stvarno, " jedinica")
    println()
    
    # Protein: višak u odnosu na minimum
    protein_visak = protein_stvarno - 169.9
    println("PROTEIN:")
    println("  Minimalna potrebna količina: 169.9 jedinica")
    println("  Stvarna količina:             ", protein_stvarno, " jedinica")
    println("  VIŠAK (premašena količina):  ", protein_visak, " jedinica")
    println()
    
    # Masti: rezerva u odnosu na maksimum
    masti_rezerva = 550.0 - masti_stvarno
    println("MASTI:")
    println("  Maksimalna dozvoljena količina: 550.0 jedinica")
    println("  Stvarna količina:               ", masti_stvarno, " jedinica")
    println("  REZERVA (preostala količina):   ", masti_rezerva, " jedinica")
    println()
    
    # Ugljikohidrati: treba biti tačno
    ugljikohidrati_razlika = abs(ugljikohidrati_stvarno - 169.9)
    println("UGLJIKOHIDRATI:")
    println("  Potrebna količina: 169.9 jedinica")
    println("  Stvarna količina:  ", ugljikohidrati_stvarno, " jedinica")
    if ugljikohidrati_razlika < 1e-6
        println("  Razlika:           ≈ 0 (tačno zadovoljeno)")
    else
        println("  Razlika:           ", ugljikohidrati_razlika, " jedinica")
    end
    println()
    
catch e
    println("GREŠKA: ", e)
end

println("=" ^ 80)

