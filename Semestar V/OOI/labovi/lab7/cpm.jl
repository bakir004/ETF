# Laboratorijska vježba 7 - Problem pronalaska kritičnog puta (CPM)
# Critical Path Method

"""
    cpm(A, P, T)

Pronalazi kritični put primjenom metode kritičnog puta (CPM).

# Argumenti
- `A`: Vektor aktivnosti (npr. ["A", "B", "C", ...])
- `P`: Vektor prethodnika za svaku aktivnost (npr. [" – ", " – ", "C", ...])
       " – " označava da aktivnost nema prethodnika
       Više prethodnika se razdvaja zarezom (npr. "B, D")
- `T`: Vektor trajanja aktivnosti (npr. [3, 3, 2, ...])

# Povratne vrijednosti
- `Z`: Trajanje kritičnog puta
- `put`: String kritičnog puta (npr. "C – D – G – I")
"""
function cpm(A, P, T)
    n = length(A)
    
    prethodnici = Dict{String, Vector{String}}()
    for i in 1:n
        prethodnici[A[i]] = String[]
        prethodnici_str = strip(P[i])
        if prethodnici_str != " – " && prethodnici_str != "–" && prethodnici_str != ""
            for p in split(prethodnici_str, ",")
                p_stripped = strip(p)
                if p_stripped != ""
                    push!(prethodnici[A[i]], p_stripped)
                end
            end
        end
    end
    
    ES = Dict{String, Float64}()  
    EF = Dict{String, Float64}()  
    
    obradjeno = Set{String}()
    
    function izracunaj_ES_EF(aktivnost)
        if aktivnost in obradjeno
            return
        end
        
        for p in prethodnici[aktivnost]
            izracunaj_ES_EF(p)
        end
        
        if length(prethodnici[aktivnost]) == 0
            ES[aktivnost] = 0.0
        else
            ES[aktivnost] = maximum([EF[p] for p in prethodnici[aktivnost]])
        end
        
        idx = findfirst(x -> x == aktivnost, A)
        EF[aktivnost] = ES[aktivnost] + T[idx]
        
        push!(obradjeno, aktivnost)
    end
    
    for i in 1:n
        izracunaj_ES_EF(A[i])
    end
    
    Z = maximum([EF[a] for a in A])
    
    LS = Dict{String, Float64}()  
    LF = Dict{String, Float64}()  
    
    zavrsne = String[]
    for i in 1:n
        is_zavrsna = true
        for j in 1:n
            if A[i] in prethodnici[A[j]]
                is_zavrsna = false
                break
            end
        end
        if is_zavrsna
            push!(zavrsne, A[i])
        end
    end
    
    sljedbenici = Dict{String, Vector{String}}()
    for i in 1:n
        sljedbenici[A[i]] = String[]
    end
    for i in 1:n
        for p in prethodnici[A[i]]
            push!(sljedbenici[p], A[i])
        end
    end
    
    obradjeno_LS_LF = Set{String}()
    
    function izracunaj_LS_LF(aktivnost)
        if aktivnost in obradjeno_LS_LF
            return
        end
        
        for s in sljedbenici[aktivnost]
            izracunaj_LS_LF(s)
        end
        
        if length(sljedbenici[aktivnost]) == 0
            LF[aktivnost] = Z
        else
            LF[aktivnost] = minimum([LS[s] for s in sljedbenici[aktivnost]])
        end
        
        idx = findfirst(x -> x == aktivnost, A)
        LS[aktivnost] = LF[aktivnost] - T[idx]
        
        push!(obradjeno_LS_LF, aktivnost)
    end
    
    for i in 1:n
        izracunaj_LS_LF(A[i])
    end
    
    rezerve = Dict{String, Float64}()
    for i in 1:n
        rezerve[A[i]] = LS[A[i]] - ES[A[i]]
    end
    
    kriticne = [a for a in A if abs(rezerve[a]) < 1e-6]
    
    pocetna_kriticna = String[]
    for k in kriticne
        if length(prethodnici[k]) == 0
            push!(pocetna_kriticna, k)
        else
            svi_kriticni = true
            for p in prethodnici[k]
                if !(p in kriticne) || abs(EF[p] - ES[k]) > 1e-6
                    svi_kriticni = false
                    break
                end
            end
            if !svi_kriticni
                push!(pocetna_kriticna, k)
            end
        end
    end
    
    if length(pocetna_kriticna) == 0
        min_ES = minimum([ES[k] for k in kriticne])
        for k in kriticne
            if ES[k] == min_ES
                push!(pocetna_kriticna, k)
                break
            end
        end
    end
    
    put_aktivnosti = String[]
    if length(pocetna_kriticna) > 0
        trenutna = pocetna_kriticna[1]
        push!(put_aktivnosti, trenutna)
        
        while true
            nadjen = false
            for s in sljedbenici[trenutna]
                if s in kriticne && abs(EF[trenutna] - ES[s]) < 1e-6
                    trenutna = s
                    push!(put_aktivnosti, trenutna)
                    nadjen = true
                    break
                end
            end
            if !nadjen
                break
            end
        end
    end
    
    put = length(put_aktivnosti) > 0 ? join(put_aktivnosti, " – ") : ""
    
    return Z, put
end

# ==================== TESTIRANJE ====================

println("="^80)
println("LABORATORIJSKA VJEŽBA 7 - CPM (CRITICAL PATH METHOD)")
println("="^80)

# Testni primjer iz zadatka
A = ["A", "B", "C", "D", "E", "F", "G", "H", "I"]
P = [" – ", " – ", " – ", "C", "A", "A", "B, D", "E", "F, G"]
T = [3, 3, 2, 2, 4, 1, 4, 1, 4]

println("\n--- TESTNI PRIMJER ---")
println("Aktivnosti: ", A)
println("Prethodnici: ", P)
println("Trajanja: ", T)

Z, put = cpm(A, P, T)

println("\n--- REZULTAT ---")
println("Trajanje kritičnog puta (Z): ", Z)
println("Kritični put: ", put)

println("\n--- OČEKIVANI REZULTAT ---")
println("Z = 12")
println("put = \"C – D – G – I\"")

println("\n--- PROVJERA ---")
if abs(Z - 12) < 1e-6
    println("✓ Trajanje je tačno!")
else
    println("✗ Trajanje nije tačno! Očekivano: 12, Dobijeno: ", Z)
end

if put == "C – D – G – I"
    println("✓ Kritični put je tačan!")
else
    println("✗ Kritični put nije tačan! Očekivano: \"C – D – G – I\", Dobijeno: \"", put, "\"")
end

println("\n" * "="^80)
