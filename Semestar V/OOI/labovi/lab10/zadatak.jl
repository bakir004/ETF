function rasporedi(M)
    M_kopija = copy(M)
    n = size(M_kopija, 1)
    
    for i in 1:n
        min_red = minimum(M_kopija[i, :])
        if min_red > 0
            M_kopija[i, :] .-= min_red
        end
    end
    
    for j in 1:n
        min_kolona = minimum(M_kopija[:, j])
        if min_kolona > 0
            M_kopija[:, j] .-= min_kolona
        end
    end
    
    raspored = zeros(Int, n, n)
    oznacene_nule = zeros(Bool, n, n)
    dostupne_nule = zeros(Bool, n, n)
    
    for i in 1:n
        for j in 1:n
            if M_kopija[i, j] == 0
                dostupne_nule[i, j] = true
            end
        end
    end
    
    while true
        broj_nula_prije = sum(dostupne_nule)
        
        if broj_nula_prije == 0
            break
        end
        
        for i in 1:n
            nule_u_redu = Int[]
            for j in 1:n
                if dostupne_nule[i, j] && !oznacene_nule[i, j]
                    push!(nule_u_redu, j)
                end
            end
            
            if length(nule_u_redu) == 1
                j = nule_u_redu[1]
                oznacene_nule[i, j] = true
                for k in 1:n
                    if k != i && dostupne_nule[k, j] && !oznacene_nule[k, j]
                        dostupne_nule[k, j] = false
                    end
                end
            end
        end
        
        for j in 1:n
            nule_u_koloni = Int[]
            for i in 1:n
                if dostupne_nule[i, j] && !oznacene_nule[i, j]
                    push!(nule_u_koloni, i)
                end
            end
            
            if length(nule_u_koloni) == 1
                i = nule_u_koloni[1]
                oznacene_nule[i, j] = true
                for k in 1:n
                    if k != j && dostupne_nule[i, k] && !oznacene_nule[i, k]
                        dostupne_nule[i, k] = false
                    end
                end
            end
        end
        
        broj_nula_nakon = sum(dostupne_nule)
        
        if broj_nula_prije == broj_nula_nakon
            break
        end
    end
    
    for i in 1:n
        for j in 1:n
            if oznacene_nule[i, j]
                raspored[i, j] = 1
            end
        end
    end
    
    Z = 0.0
    for i in 1:n
        for j in 1:n
            if raspored[i, j] == 1
                Z += M[i, j]
            end
        end
    end
    
    return raspored, Z
end


println("\n--- TESTNI PRIMJER 1 ---")
M1 = [80 20 23; 31 40 12; 61 1 1]
println("M =")
println(M1)
raspored1, Z1 = rasporedi(M1)
println("\nMatrica rasporedivanja:")
println(raspored1)
println("Optimalna vrijednost funkcije cilja Z = ", Z1)

println("\n--- TESTNI PRIMJER 2 ---")
M2 = [25 55 40 80; 75 40 60 95; 35 50 120 80; 15 30 55 65]
println("M =")
println(M2)
raspored2, Z2 = rasporedi(M2)
println("\nMatrica rasporedivanja:")
println(raspored2)
println("Optimalna vrijednost funkcije cilja Z = ", Z2)
