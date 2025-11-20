# Generalizovani Simplex - Laboratorijska vježba 5
# Implementacija generalizovanog simpleks metoda za rješavanje linearnih programskih problema

using LinearAlgebra
using Printf

# Učitaj potrebne funkcije iz lab4
include(joinpath(@__DIR__, "..", "lab4", "rijesi_simplex.jl"))

"""
    general_simplex(goal, c, A, b, csigns, vsigns)

Generalizovani simplex metod za rješavanje linearnih programskih problema.

Parametri:
- goal: 'max' ili 'min' - tip optimizacije
- c: vektor koeficijenata funkcije cilja
- A: matrica koeficijenata sa lijeve strane ograničenja
- b: vektor sa desne strane ograničenja
- csigns: vektor sa elementima +1 (≥), -1 (≤), 0 (=)
- vsigns: vektor sa elementima +1 (nenegativna), -1 (nepozitivna), 0 (neograničena)

Izlaz:
- Z: optimalna vrijednost funkcije cilja
- X: vektor optimalnih vrijednosti izvornih promjenljivih
- Xd: vektor optimalnih vrijednosti izravnavajućih promjenljivih
- Y: vektor cijena u sjeni (dualne promjenljive)
- Yd: vektor reducirane cijene (dualne izravnavajuće promjenljive)
- status: status kod (0-5)
"""
function general_simplex(goal, c, A, b, csigns, vsigns)
	# Validacija goal parametra
	if goal != "max" && goal != "min"
		n = length(c)
		m = length(b)
		return NaN, zeros(n), zeros(m), zeros(m), zeros(m), 5
	end
	
	# Validacija dimenzija
	if size(b, 1) != size(A, 1) || size(c, 2) != size(A, 2)
		n = length(c)
		m = length(b)
		return NaN, zeros(n), zeros(m), zeros(m), zeros(m), 5
	end
	
	# Praćenje transformacija varijabli
	original_n = length(c)
	original_m = length(b)
	neograniceneMapa = []
	
	# Transformacija za negativne i neograničene varijable
	A_mod = copy(A)
	c_mod = copy(c)
	b_mod = copy(b)
	csigns_mod = copy(csigns)
	
	for i in 1:lastindex(vsigns)
		if vsigns[i] == -1
			# Nepozitivna varijabla: xi ≤ 0 -> zamjenjujemo sa xi' = -xi, xi' ≥ 0
			A_mod[:, i] *= -1
			c_mod[i] *= -1
		elseif vsigns[i] == 0
			# Neograničena varijabla: xi = xi+ - xi-, gdje xi+, xi- ≥ 0
			c_mod = [c_mod -c_mod[i]]
			A_mod = [A_mod -A_mod[:, i]]
			push!(neograniceneMapa, (i, size(A_mod, 2)))
		end
	end
	
	# Kreiranje početne simplex tabele
	(tabela, bazniIndeksi, umjetneVarijable) = kreiraj_simplex_tabelu(A_mod, b_mod, c_mod, csigns_mod)
	
	# Prilagodba tabele ovisno o tipu optimizacije
	if goal == "min"
		tabela[end, :] *= -1
	else
		tabela[end-1, 1] = abs(tabela[end-1, 1])
	end
	
	# Pronalaženje maksimalnog koeficijenta u M redu
	M_red = deepcopy(tabela[end-1, :])
	popfirst!(M_red)
	(max_M, kolona_M) = findmax(M_red)
	kolona_M += 1
	
	kolonaIndeks = 0
	M_red_puni = tabela[end-1, :]
	ciljniRed = tabela[end, :]
	max_koef = -Inf
	for i in 2:lastindex(ciljniRed)
		if ciljniRed[i] > max_koef && (M_red_puni[i] >= 0 || M_red_puni[i] == -0)
			max_koef = ciljniRed[i]
			kolonaIndeks = i
		end
	end
	
	# Simplex iteracije
	while max_koef > 0 || max_M > 0
		if max_M > 0
			pivotKolona = kolona_M
		else
			pivotKolona = kolonaIndeks
		end
		
		min_ratio = Inf
		pivotRed = -1
		for i in 1:size(tabela, 1)-2
			if tabela[i, pivotKolona] > 0
				ratio = tabela[i, 1] / tabela[i, pivotKolona]
				if (ratio < min_ratio || (ratio == min_ratio && rand() > 0.5))
					min_ratio = ratio
					pivotRed = i
				end
			end
		end
		
		if min_ratio == Inf
			# Rješenje je neograničeno
			n = original_n
			m = original_m
			return Inf, zeros(n), zeros(m), zeros(m), zeros(m), 3
		end
		
		bazniIndeksi[pivotRed] = pivotKolona - 1
		pivot = tabela[pivotRed, pivotKolona]
		
		tabela[pivotRed, :] ./= pivot
		
		for i in 1:size(tabela, 1)
			if i != pivotRed
				faktor = tabela[i, pivotKolona]
				for j in 1:size(tabela, 2)
					tabela[i, j] -= tabela[pivotRed, j] * faktor
				end
			end
		end
		
		M_red = deepcopy(tabela[end-1, :])
		popfirst!(M_red)
		(max_M, kolona_M) = findmax(M_red)
		kolona_M += 1
		
		if max_M <= 1e-8
			max_M = 0
		end
		
		if max_M <= 0
			M_red_puni = tabela[end-1, :]
			ciljniRed = tabela[end, :]
			max_koef = -Inf
			for i in 2:lastindex(ciljniRed)
				if ciljniRed[i] > max_koef && (M_red_puni[i] >= 0 || M_red_puni[i] == -0)
					max_koef = ciljniRed[i]
					kolonaIndeks = i
				end
			end
		end
	end
	
	# Provjera postojanja umjetnih varijabli u bazi
	for i in 1:lastindex(umjetneVarijable)
		if (Float64(umjetneVarijable[i]) in bazniIndeksi)
			# Dopustiva oblast ne postoji
			n = original_n
			m = original_m
			return NaN, zeros(n), zeros(m), zeros(m), zeros(m), 4
		end
	end
	
	# Rekonstrukcija rješenja
	x_full = zeros(size(tabela, 2) - 1)
	for i in 1:lastindex(bazniIndeksi)
		if bazniIndeksi[i] > 0
			x_full[Int(round(bazniIndeksi[i]))] = tabela[i, 1]
		end
	end
	
	# Analiza jedinstvenosti
	jedinstveno = true
	for i in 2:(size(tabela, 2) - length(umjetneVarijable))
		if abs(x_full[i-1]) < 1e-10 && abs(tabela[end, i]) < 1e-10
			jedinstveno = false
		end
	end
	
	# Provjera degenerisanosti
	degenerirano = false
	for i in 1:(size(tabela, 1)-2)
		if abs(tabela[i, 1]) < 1e-10
			degenerirano = true
		end
	end
	
	# Rekonstrukcija izvornih varijabli
	X = zeros(original_n)
	for i in 1:original_n
		if vsigns[i] == -1
			# Nepozitivna varijabla: vraćamo transformaciju
			X[i] = -x_full[i]
		elseif vsigns[i] == 0
			# Neograničena varijabla: xi = xi+ - xi-
			prvi_idx = i
			drugi_idx = findfirst(x -> x[1] == i, neograniceneMapa)
			if drugi_idx !== nothing
				(_, drugi_var_idx) = neograniceneMapa[drugi_idx]
				if drugi_var_idx <= length(x_full)
					X[i] = x_full[prvi_idx] - x_full[drugi_var_idx]
				else
					X[i] = x_full[prvi_idx]
				end
			else
				X[i] = x_full[prvi_idx]
			end
		else
			X[i] = x_full[i]
		end
	end
	
	# Izravnavajuće varijable i dualne varijable
	n_original_vars = size(A_mod, 2)
	Xd = zeros(original_m)
	Y = zeros(original_m)
	Yd = zeros(original_m)
	
	# Praćenje koje su izravnavajuće varijable (slack/surplus)
	# Redoslijed: prvo se dodaju slack varijable za ≤, zatim surplus za ≥
	slack_count = 0
	surplus_count = 0
	
	for i in 1:original_m
		if csigns_mod[i] == -1  # ≤ ograničenje - slack varijabla
			slack_count += 1
			slack_idx = n_original_vars + slack_count
			if slack_idx <= length(x_full)
				Xd[i] = x_full[slack_idx]
			end
			# Dualna varijabla je koeficijent u Z redu za slack varijablu
			if slack_idx < size(tabela, 2)
				Y[i] = -tabela[end, slack_idx + 1]  # +1 jer prva kolona je b
			end
		elseif csigns_mod[i] == 1  # ≥ ograničenje - surplus varijabla
			surplus_count += 1
			surplus_idx = n_original_vars + sum(csigns_mod .== -1) + surplus_count
			# Dualna varijabla je koeficijent u Z redu za surplus varijablu
			if surplus_idx < size(tabela, 2)
				Y[i] = -tabela[end, surplus_idx + 1]
			end
		else  # = ograničenje - nema izravnavajuće varijable, ali ima umjetnu
			# Za = ograničenja, dualna varijabla se čita iz Z reda
			# Pronađimo koeficijent koji odgovara umjetnoj varijabli tog ograničenja
			# ili koeficijent u Z redu za ograničenje koje je u bazi
			# Alternativno, možemo koristiti koeficijent u Z redu za ograničenje
			# koje je u bazi kao baznu varijablu
		end
	end
	
	# Za = ograničenja, dualne varijable se čitaju iz Z reda
	# Provjerimo koja ograničenja su u bazi
	for i in 1:original_m
		if csigns_mod[i] == 0  # = ograničenje
			# Pronađimo umjetnu varijablu za ovo ograničenje
			# Umjetne varijable se dodaju nakon svih izravnavajućih
			# Tražimo u baznim indeksima da li postoji umjetna varijabla za ovo ograničenje
			for j in 1:length(bazniIndeksi)
				if bazniIndeksi[j] == i  # Ako je i-to ograničenje u bazi
					# Dualna varijabla je koeficijent u Z redu za baznu varijablu
					# Ali to nije tačno - trebamo pronaći koeficijent u Z redu
					# koji odgovara umjetnoj varijabli
				end
			end
			# Alternativno, za = ograničenja koja nisu u bazi,
			# dualna varijabla je koeficijent u Z redu za umjetnu varijablu
			# Umjetne varijable se dodaju nakon svih izravnavajućih
			umjetna_idx = n_original_vars + sum(csigns_mod .== -1) + sum(csigns_mod .== 1) + 
			              sum(csigns_mod[1:i] .== 0)
			if umjetna_idx < size(tabela, 2)
				# Provjerimo da li je ova umjetna varijabla u bazi
				if !(umjetna_idx in bazniIndeksi)
					Y[i] = -tabela[end, umjetna_idx + 1]
				end
			end
		end
	end
	
	# Korekcija znaka optimalne vrijednosti
	if goal == "min"
		Z = tabela[end, 1]
	else
		Z = -tabela[end, 1]
	end
	
	# Određivanje statusa
	status = 0
	if !jedinstveno
		status = 2
	end
	if degenerirano && status == 0
		status = 1
	end
	if degenerirano && !jedinstveno
		status = 2
	end
	
	return Z, X, Xd, Y, Yd, status
end

