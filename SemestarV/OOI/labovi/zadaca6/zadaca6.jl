# Zadatak 6 - Postoptimalna analiza (promjena koeficijenata sa desne strane ograničenja)
# Dualni simpleks metod

using JuMP
using GLPK
using LinearAlgebra

println("="^80)
println("ZADATAK 6 - POSTOPTIMALNA ANALIZA")
println("="^80)

# ==================== PODACI IZ ZADATKA ====================
# Radnik 1: max 40 sati, 5 KM/sat
# Radnik 2: max 50 sati, 6 KM/sat
# Radio 1: radnik 1 - 1h, radnik 2 - 2h, materijal 5 KM, prodajna cijena 25 KM
# Radio 2: radnik 1 - 2h, radnik 2 - 1h, materijal 4 KM, prodajna cijena 22 KM

# Funkcija cilja: maksimizacija dobiti
# Dobit po radiju = prodajna cijena - cijena materijala - cijena rada
# Radio 1: 25 - 5 - (1*5 + 2*6) = 25 - 5 - 17 = 3 KM
# Radio 2: 22 - 4 - (2*5 + 1*6) = 22 - 4 - 16 = 2 KM

# Varijable:
# x1 = broj radija tipa 1
# x2 = broj radija tipa 2

# Ograničenja:
# Radnik 1: 1*x1 + 2*x2 <= 40
# Radnik 2: 2*x1 + 1*x2 <= 50

# Funkcija cilja: max Z = 3*x1 + 2*x2

# Optimalna simpleks tabela:
# Baza | bi | X1 | X2 | X3 | X4
# X2   | 10 |  0 |  1 | 2/3| -1/3
# X1   | 20 |  1 |  0 | -1/3| 2/3
# Z    |-80 |  0 |  0 | -1/3| -4/3
# (X3 i X4 su slack varijable za radnika 1 i radnika 2)

# Matrica AB^(-1) se čita iz optimalne tabele u kolonama koje odgovaraju
# početnim slack varijablama (X3 i X4):
# AB^(-1) = [2/3  -1/3]
#            [-1/3  2/3]

println("\n--- a) MATEMATIČKI MODEL I DUALNI PROBLEM ---\n")

println("PRIMALNI PROBLEM:")
println("max Z = 3*x1 + 2*x2")
println("p.o.  x1 + 2*x2 <= 40   (radnik 1)")
println("      2*x1 + x2 <= 50   (radnik 2)")
println("      x1, x2 >= 0")

println("\nDUALNI PROBLEM:")
println("min W = 40*y1 + 50*y2")
println("p.o.  y1 + 2*y2 >= 3")
println("      2*y1 + y2 >= 2")
println("      y1, y2 >= 0")

# ==================== FUNKCIJA ZA RJEŠAVANJE POMOĆU JUMP ====================
function rijesi_lp(b1, b2; verbose=true)
    model = Model(GLPK.Optimizer)
    
    @variable(model, x1 >= 0)
    @variable(model, x2 >= 0)
    
    @objective(model, Max, 3*x1 + 2*x2)
    
    @constraint(model, radnik1, 1*x1 + 2*x2 <= b1)
    @constraint(model, radnik2, 2*x1 + 1*x2 <= b2)
    
    optimize!(model)
    
    if termination_status(model) == MOI.OPTIMAL
        x1_val = value(x1)
        x2_val = value(x2)
        Z_val = objective_value(model)
        
        # Provjera: Z mora biti jednako 3*x1 + 2*x2
        Z_provjera = 3*x1_val + 2*x2_val
        if abs(Z_val - Z_provjera) > 1e-6
            println("UPOZORENJE: Z ($Z_val) se ne poklapa sa 3*x1 + 2*x2 ($Z_provjera)")
        end
        
        # Dobijanje dualnih vrijednosti (cijene u sjeni)
        # JuMP vraća negativne vrijednosti za maksimizaciju, pa uzimamo apsolutnu vrijednost
        y1_val = abs(dual(radnik1))
        y2_val = abs(dual(radnik2))
        
        # Slack varijable
        slack1 = b1 - (1*x1_val + 2*x2_val)
        slack2 = b2 - (2*x1_val + 1*x2_val)
        
        if verbose
            println("\nOptimalno rješenje:")
            println("  x1 (Radio 1) = ", round(x1_val, digits=3))
            println("  x2 (Radio 2) = ", round(x2_val, digits=3))
            println("  Z (Dobit) = ", round(Z_val, digits=3), " KM")
            println("  Provjera: 3*x1 + 2*x2 = ", round(Z_provjera, digits=3), " KM")
            println("\nSlack varijable:")
            println("  Slobodni sati radnika 1 = ", round(slack1, digits=3))
            println("  Slobodni sati radnika 2 = ", round(slack2, digits=3))
            println("\nDualne varijable (cijene u sjeni):")
            println("  y1 (radnik 1) = ", round(y1_val, digits=3))
            println("  y2 (radnik 2) = ", round(y2_val, digits=3))
        end
        
        return x1_val, x2_val, Z_val, slack1, slack2, y1_val, y2_val
    else
        println("Rješenje nije pronađeno! Status: ", termination_status(model))
        return nothing
    end
end

# ==================== FUNKCIJA ZA POSTOPTIMALNU ANALIZU ====================
function postoptimalna_analiza(b1_novo, b2_novo)
    # Matrica AB^(-1) iz optimalne tabele
    # Kolone X3 i X4 u optimalnoj tabeli daju AB^(-1)
    AB_inv = [2/3  -1/3;
              -1/3  2/3]
    
    # Novi vektor desnih strana
    b_novo = [b1_novo, b2_novo]
    
    # Izračunavanje novog baznog rješenja
    xB_novo = AB_inv * b_novo
    
    println("\nKorištenje formule xB' = AB^(-1) * b':")
    println("AB^(-1) = ")
    display(AB_inv)
    println("b' = ", b_novo)
    println("xB' = AB^(-1) * b' = ", round.(xB_novo, digits=3))
    
    # Provjera dopustivosti
    if all(xB_novo .>= 0)
        println("✓ Rješenje je dopustivo (xB' >= 0)")
        println("  Novo optimalno rješenje:")
        println("    x2 = ", round(xB_novo[1], digits=3))
        println("    x1 = ", round(xB_novo[2], digits=3))
        
        # Izračunavanje nove vrijednosti funkcije cilja
        # Iz optimalne tabele: Z = 80 + (1/3)*delta_b1 + (4/3)*delta_b2
        # gdje su delta_b razlike između novog i starog b
        b_staro = [40, 50]
        delta_b = b_novo - b_staro
        # Dualne vrijednosti iz optimalne tabele su -1/3 i -4/3, ali to je zbog načina zapisa
        # Stvarna vrijednost funkcije cilja u optimalnoj tabeli je 80 (ne -80)
        delta_Z = (1/3)*delta_b[1] + (4/3)*delta_b[2]
        Z_novo = 80 + delta_Z
        
        println("    Z = ", round(Z_novo, digits=3), " KM")
        
        return true, xB_novo[2], xB_novo[1], Z_novo
    else
        println("✗ Rješenje NIJE dopustivo (neki elementi xB' < 0)")
        println("  Potrebno je primijeniti dualni simpleks metod")
        return false, nothing, nothing, nothing
    end
end

# ==================== DIJELOVI ZADATKA ====================

println("\n\n--- b) RADNIK 1 MOŽE RADITI MAKSIMALNO 28 SATI ---\n")
println("Originalno: b1 = 40, b2 = 50")
println("Novo: b1' = 28, b2' = 50")

b1_novo_b = 28
b2_novo_b = 50

dopustivo_b, x1_b, x2_b, Z_b = postoptimalna_analiza(b1_novo_b, b2_novo_b)

println("\nProvjera pomoću JuMP:")
x1_jump_b, x2_jump_b, Z_jump_b, slack1_b, slack2_b, y1_b, y2_b = rijesi_lp(b1_novo_b, b2_novo_b)

println("\n\n--- c) RADNIK 1 MOŽE RADITI MAKSIMALNO 102 SATA ---\n")
println("Originalno: b1 = 40, b2 = 50")
println("Novo: b1' = 102, b2' = 50")

b1_novo_c = 102
b2_novo_c = 50

dopustivo_c, x1_c, x2_c, Z_c = postoptimalna_analiza(b1_novo_c, b2_novo_c)

if !dopustivo_c
    println("\nRješenje nije dopustivo, rješavamo pomoću JuMP (dualni simpleks):")
end

println("\nProvjera pomoću JuMP:")
x1_jump_c, x2_jump_c, Z_jump_c, slack1_c, slack2_c, y1_c, y2_c = rijesi_lp(b1_novo_c, b2_novo_c)

println("\n\n--- d) ODGOVORI ZA RJEŠENJE POD c) ---\n")
println("Koliko radija 1 i radija 2 treba proizvesti?")
println("  Radio 1: ", round(x1_jump_c, digits=3), " jedinica")
println("  Radio 2: ", round(x2_jump_c, digits=3), " jedinica")

println("\nKoliko slobodnih sati će ostati na raspolaganju radniku 1 i radniku 2?")
println("  Radnik 1: ", round(slack1_c, digits=3), " sati")
println("  Radnik 2: ", round(slack2_c, digits=3), " sati")

println("\nDa li su oba radija isplativa za proizvodnju?")
println("  Radio 1: ", x1_jump_c > 0 ? "DA (x1 > 0)" : "NE (x1 = 0)")
println("  Radio 2: ", x2_jump_c > 0 ? "DA (x2 > 0)" : "NE (x2 = 0)")

println("\nKoliko će se povećati vrijednost funkcije cilja pri jediničnom povećanju raspoloživih vremena radnika 1 i 2?")
println("  Radnik 1 (y1 - cijena u sjeni): ", round(y1_c, digits=3), " KM/sat")
println("  Radnik 2 (y2 - cijena u sjeni): ", round(y2_c, digits=3), " KM/sat")
println("  (Ovo znači da bi povećanje kapaciteta radnika 1 za 1 sat povećalo dobit za ", round(y1_c, digits=3), " KM)")

println("\nUkoliko neki od radija nije isplativ za proizvodnju, koliko mora iznositi dobit po proizvedenom radiju da bi on postao isplativ?")
# Reducirane cijene iz optimalne tabele: -1/3 za X3, -4/3 za X4
# Ali ovo su za slack varijable. Za sami proizvod, trebamo provjeriti reduciranu cijenu.
# U optimalnoj tabeli, koeficijenti u redu Z za X1 i X2 su 0 (jer su bazne varijable).
# Reducirana cijena za neku varijablu pokazuje za koliko se funkcija cilja smanji povećanjem te varijable za 1.

# Ako je x1 > 0 i x2 > 0, oba su isplativa
if x1_jump_c > 0 && x2_jump_c > 0
    println("  Oba radija su isplativa za proizvodnju.")
else
    if x1_jump_c == 0
        println("  Radio 1 nije isplativ. Da bi postao isplativ, dobit po radiju 1 mora biti veća.")
        println("  Trenutna dobit po radiju 1: 3 KM")
        println("  (Za tačnu vrijednost potrebno je izračunati reduciranu cijenu iz simpleks tabele)")
    end
    if x2_jump_c == 0
        println("  Radio 2 nije isplativ. Da bi postao isplativ, dobit po radiju 2 mora biti veća.")
        println("  Trenutna dobit po radiju 2: 2 KM")
        println("  (Za tačnu vrijednost potrebno je izračunati reduciranu cijenu iz simpleks tabele)")
    end
end

println("\n\n--- e) PROVJERA RJEŠENJA U JULIJI ---\n")

println("=== POD b) - Radnik 1: 28 sati ===")
println("Ulazni podaci:")
println("  b1 = ", b1_novo_b)
println("  b2 = ", b2_novo_b)
println("  Funkcija cilja: max Z = 3*x1 + 2*x2")
println("  Ograničenja: x1 + 2*x2 <= ", b1_novo_b, ", 2*x1 + x2 <= ", b2_novo_b)
x1_b, x2_b, Z_b, slack1_b, slack2_b, y1_b, y2_b = rijesi_lp(b1_novo_b, b2_novo_b)

println("\n=== POD c) - Radnik 1: 102 sata ===")
println("Ulazni podaci:")
println("  b1 = ", b1_novo_c)
println("  b2 = ", b2_novo_c)
println("  Funkcija cilja: max Z = 3*x1 + 2*x2")
println("  Ograničenja: x1 + 2*x2 <= ", b1_novo_c, ", 2*x1 + x2 <= ", b2_novo_c)
x1_c, x2_c, Z_c, slack1_c, slack2_c, y1_c, y2_c = rijesi_lp(b1_novo_c, b2_novo_c)

println("\n" * "="^80)
println("ZADATAK ZAVRŠEN")
println("="^80)

