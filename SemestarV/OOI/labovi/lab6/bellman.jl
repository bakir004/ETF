# Laboratorijska vježba 6 - Bellmanov algoritam
# Pronalazak najkraćeg puta u acikličnom usmjerenom grafu

"""
    najkraci_put(M)

Bellmanov algoritam unaprijed za pronalazak najkraćeg puta.

# Argumenti
- `M`: Težinska matrica grafa (n×n). M[i,j] predstavlja težinu veze od čvora i do čvora j.
        Vrijednost 0 označava da veza ne postoji.

# Povratna vrijednost
- `putevi`: Matrica (n×3) gdje je:
    - Kolona 1: Indeks čvora
    - Kolona 2: Najkraća udaljenost od početnog čvora (čvor 1)
    - Kolona 3: Indeks prethodnog čvora na najkraćem putu
"""
function najkraci_put(M)
    n = size(M, 1)
    
    d = fill(Inf, n)
    d[1] = 0
    
    prethodnik = collect(1:n)
    
    for j in 2:n
        min_udaljenost = Inf
        min_prethodnik = j
        
        for i in (j-1):-1:1
            if M[i, j] != 0 
                nova_udaljenost = d[i] + M[i, j]
                if nova_udaljenost < min_udaljenost
                    min_udaljenost = nova_udaljenost
                    min_prethodnik = i
                end
            end
        end
        
        if min_udaljenost < Inf
            d[j] = min_udaljenost
            prethodnik[j] = min_prethodnik
        end
    end
    
    putevi = zeros(n, 3)
    for i in 1:n
        putevi[i, 1] = i
        putevi[i, 2] = d[i] == Inf ? Inf : d[i]
        putevi[i, 3] = prethodnik[i]
    end
    
    return putevi
end


"""
    najkraci_put_unazad(M)

Bellmanov algoritam unazad za pronalazak najkraćeg puta.
Algoritam ide od završnih čvorova prema početnom.

# Argumenti
- `M`: Težinska matrica grafa (n×n). M[i,j] predstavlja težinu veze od čvora i do čvora j.
        Vrijednost 0 označava da veza ne postoji.

# Povratna vrijednost
- `putevi`: Matrica (n×3) gdje je:
    - Kolona 1: Indeks čvora
    - Kolona 2: Najkraća udaljenost od tog čvora do završnog čvora
    - Kolona 3: Indeks sljedećeg čvora na najkraćem putu prema kraju
"""
function najkraci_put_unazad(M)
    n = size(M, 1)
    
    d = fill(Inf, n)
    d[n] = 0
    
    sljedbenik = collect(1:n)
    
    for i in (n-1):-1:1
        min_udaljenost = Inf
        min_sljedbenik = i
        
        for j in (i+1):n
            if M[i, j] != 0
                nova_udaljenost = M[i, j] + d[j]
                if nova_udaljenost < min_udaljenost
                    min_udaljenost = nova_udaljenost
                    min_sljedbenik = j
                end
            end
        end
        
        if min_udaljenost < Inf
            d[i] = min_udaljenost
            sljedbenik[i] = min_sljedbenik
        end
    end
    
    putevi = zeros(n, 3)
    for i in 1:n
        putevi[i, 1] = i
        putevi[i, 2] = d[i] == Inf ? Inf : d[i]
        putevi[i, 3] = sljedbenik[i]
    end
    
    return putevi
end


"""
    rekonstruisi_put(putevi, start, cilj)

Rekonstruiše najkraći put od početnog do ciljnog čvora.

# Argumenti
- `putevi`: Matrica puteva dobijena iz najkraci_put funkcije
- `start`: Početni čvor
- `cilj`: Ciljni čvor

# Povratna vrijednost
- Vektor indeksa čvorova koji čine najkraći put
"""
function rekonstruisi_put(putevi, start, cilj)
    put = [cilj]
    trenutni = cilj
    
    while trenutni != start
        prethodnik = Int(putevi[trenutni, 3])
        if prethodnik == trenutni
            println("Ne postoji put od čvora $start do čvora $cilj")
            return []
        end
        pushfirst!(put, prethodnik)
        trenutni = prethodnik
    end
    
    return put
end


# ==================== TESTIRANJE ====================

println("=" ^ 60)
println("LABORATORIJSKA VJEŽBA 6 - BELLMANOV ALGORITAM")
println("=" ^ 60)

# Testna matrica iz zadatka
M = [0  1  3  0  0  0;
     0  0  2  3  0  0;
     0  0  0 -4  9  0;
     0  0  0  0  1  2;
     0  0  0  0  0  2;
     0  0  0  0  0  0]

println("\n--- ULAZNA MATRICA M ---")
display(M)

# Test Bellmanovog algoritma unaprijed
println("\n\n--- BELLMANOV ALGORITAM UNAPRIJED ---")
putevi = najkraci_put(M)
println("Rezultat (putevi):")
println("Čvor | Udaljenost | Prethodnik")
println("-" ^ 35)
for i in 1:size(putevi, 1)
    println("  $(Int(putevi[i,1]))  |     $(putevi[i,2])      |     $(Int(putevi[i,3]))")
end

# Očekivani rezultat
println("\nOčekivani rezultat:")
println("[1 0 1; 2 1 1; 3 3 2; 4 -1 3; 5 0 4; 6 1 4]")

# Rekonstrukcija puta od čvora 1 do čvora 6
println("\nNajkraći put od čvora 1 do čvora 6:")
put = rekonstruisi_put(putevi, 1, 6)
println(join(put, " → "))
println("Ukupna dužina puta: $(putevi[6, 2])")

# Test Bellmanovog algoritma unazad
println("\n\n--- BELLMANOV ALGORITAM UNAZAD ---")
putevi_unazad = najkraci_put_unazad(M)
println("Rezultat (putevi_unazad):")
println("Čvor | Udalj. do kraja | Sljedbenik")
println("-" ^ 40)
for i in 1:size(putevi_unazad, 1)
    println("  $(Int(putevi_unazad[i,1]))  |       $(putevi_unazad[i,2])        |      $(Int(putevi_unazad[i,3]))")
end

# Rekonstrukcija puta unazad
println("\nNajkraći put od čvora 1 do čvora 6 (rekonstrukcija iz algoritma unazad):")
put_unazad = [1]
global trenutni_unazad = 1
while trenutni_unazad != 6
    global trenutni_unazad
    sljedbenik = Int(putevi_unazad[trenutni_unazad, 3])
    push!(put_unazad, sljedbenik)
    trenutni_unazad = sljedbenik
end
println(join(put_unazad, " → "))
println("Ukupna dužina puta: $(putevi_unazad[1, 2])")

println("\n" * "=" ^ 60)
println("TESTIRANJE ZAVRŠENO")
println("=" ^ 60)

