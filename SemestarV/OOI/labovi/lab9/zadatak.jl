"""
    nadji_pocetno_SZU(C, I, O)

Pronalazi pocetno rjesenje transportnog problema koristeci metodu sjeverozapadnog ugla.

# Argumenti
- `C`: Matrica jedinicnih cijena transporta (m×n). C[i,j] je cijena transporta iz izvorista I[i] do odredista O[j].
- `I`: Vektor izvorista sa kapacitetima (duzine m).
- `O`: Vektor odredista sa kapacitetima (duzine n).

# Povratne vrijednosti
- `A`: Matrica pocetnog rjesenja (m×n). A[i,j] je kolicina koja se prenosi preko rute I[i] → O[j].
       A[i,j] = -1 oznacava epsilon (polje koje se ne uracunava u troskove).
- `T`: Minimalni troskovi transporta (skalarna vrijednost).
"""
function nadji_pocetno_SZU(C, I, O)
    I_kopija = copy(I)
    O_kopija = copy(O)
    C_kopija = copy(C)
    
    m = length(I_kopija)
    n = length(O_kopija)
    
    suma_I = sum(I_kopija)
    suma_O = sum(O_kopija)
    
    if suma_I > suma_O
        push!(O_kopija, suma_I - suma_O)
        C_kopija = hcat(C_kopija, zeros(m))
        n = n + 1
    elseif suma_I < suma_O
        push!(I_kopija, suma_O - suma_I)
        C_kopija = vcat(C_kopija, zeros(1, n))
        m = m + 1
    end
    
    A = zeros(m, n)
    
    i = 1
    j = 1
    
    while !(i == m && j == n)
        if I_kopija[i] < O_kopija[j]
            A[i, j] = I_kopija[i]
            O_kopija[j] = O_kopija[j] - I_kopija[i]
            I_kopija[i] = 0
            i = i + 1
        elseif I_kopija[i] > O_kopija[j]
            A[i, j] = O_kopija[j]
            I_kopija[i] = I_kopija[i] - O_kopija[j]
            O_kopija[j] = 0
            j = j + 1
        else
            A[i, j] = -1  
            I_kopija[i] = 0
            O_kopija[j] = 0
            if i == m && j == n
                break  
            end
            j = j + 1
            i = i + 1
        end
    end
    
    if A[i, j] == 0 && (I_kopija[i] > 0 || O_kopija[j] > 0)
        if I_kopija[i] == O_kopija[j]
            A[i, j] = -1
        else
            A[i, j] = min(I_kopija[i], O_kopija[j])
        end
    end
    
    T = 0.0
    for i in 1:m
        for j in 1:n
            if A[i, j] > 0  
                T += A[i, j] * C_kopija[i, j]
            end
        end
    end
    
    return A, T
end

println("=" ^ 80)
println("LABORATORIJSKA VJEZBA 9 - TRANSPORTNI PROBLEM (SZU)")
println("=" ^ 80)

println("\n--- TESTNI PRIMJER 1 (Balansiran problem) ---")
C1 = [3 2 4; 1 5 2; 4 3 1]
I1 = [10, 15, 20]
O1 = [12, 18, 15]
println("C =")
println(C1)
println("I = ", I1)
println("O = ", O1)
A1, T1 = nadji_pocetno_SZU(C1, I1, O1)
println("\nPocetno rjesenje A:")
println(A1)
println("Minimalni troskovi transporta T = ", T1)
println("Provjera: suma I = ", sum(I1), ", suma O = ", sum(O1))

println("\n--- TESTNI PRIMJER 2 (suma I > suma O) ---")
C2 = [2 3; 4 1]
I2 = [20, 15]
O2 = [10, 20]
println("C =")
println(C2)
println("I = ", I2, " (suma = ", sum(I2), ")")
println("O = ", O2, " (suma = ", sum(O2), ")")
A2, T2 = nadji_pocetno_SZU(C2, I2, O2)
println("\nPocetno rjesenje A:")
println(A2)
println("Minimalni troskovi transporta T = ", T2)

println("\n--- TESTNI PRIMJER 3 (suma I < suma O) ---")
C3 = [5 2 3; 1 4 2]
I3 = [10, 15]
O3 = [8, 12, 10]
println("C =")
println(C3)
println("I = ", I3, " (suma = ", sum(I3), ")")
println("O = ", O3, " (suma = ", sum(O3), ")")
A3, T3 = nadji_pocetno_SZU(C3, I3, O3)
println("\nPocetno rjesenje A:")
println(A3)
println("Minimalni troskovi transporta T = ", T3)

