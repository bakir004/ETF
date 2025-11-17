# Implementacija simplex metode za rješavanje linearnih programskih problema
# Bakir Činjarević 19705 i Amar Handanagić 19089

using LinearAlgebra
using Printf

function kreiraj_simplex_tabelu(A, b, c, csigns)
	A_mod = A
	b_mod = b
	c_mod = c
	m = size(A, 1)
	bazniIndeksi = zeros(m, 1)
	c_M = zeros(1, size(c, 2))
	M_offset = 0
	umjetneVarijable = []

	for i ∈ 1:m
		# Normalizacija kada je desna strana negativna
		if (b_mod[i] < 0)
			A_mod[i, :] = -A_mod[i, :]
			b_mod[i] = -b_mod[i]
			if csigns[i] == -1
				csigns[i] = 1
			elseif csigns[i] == 1
				csigns[i] = -1
			end
		end

		# Uvođenje dodatnih varijabli za izjednačavanje
		if csigns[i] == -1
			kolona = zeros(m, 1)
			kolona[i] = 1
			A_mod = [A_mod kolona]
			c_M = [c_M 0]
			c_mod = [c_mod 0]
			bazniIndeksi[i] = size(c_mod, 2)
		elseif csigns[i] == 1
			kolona = zeros(m, 1)
			kolona[i] = -1
			A_mod = [A_mod kolona]
			c_M = [c_M 0]
			c_mod = [c_mod 0]
		end
	end

	# Uvođenje umjetnih varijabli za početnu bazu
	for i ∈ 1:m
		if csigns[i] == 1
			kolona = zeros(m, 1)
			kolona[i] = 1
			A_mod = [A_mod kolona]
			c_M = [c_M -1]
			c_mod = [c_mod 0]
			bazniIndeksi[i] = size(c_mod, 2)
			c_M[:] .= c_M[:] .+ A_mod[i, :]
			M_offset = M_offset .+ b_mod[i]
			# Čuvanje indeksa umjetnih varijabli
			umjetneVarijable = [umjetneVarijable; size(c_mod, 2)]
		elseif csigns[i] == 0
			kolona = zeros(m, 1)
			kolona[i] = 1
			A_mod = [A_mod kolona]
			c_M = [c_M -1]
			c_mod = [c_mod 0]
			bazniIndeksi[i] = size(c_mod, 2)
			c_M[:] .= c_M[:] .+ A_mod[i, :]
			M_offset = M_offset .+ b_mod[i]
			# Čuvanje indeksa umjetnih varijabli
			umjetneVarijable = [umjetneVarijable; size(c_mod, 2)]
		end
	end

	c_mod = [0 c_mod]
	c_M = [M_offset c_M]
	tabela = [b_mod A_mod]
	tabela = [tabela; c_M; c_mod]
	return tabela, bazniIndeksi, umjetneVarijable
end

function rijesi_simplex(goal, A, b, c, csigns, vsigns)
	if size(b, 1) != size(A, 1) || size(c, 2) != size(A, 2)
		throw("Dimenzije ulaznih parametara nisu validni")
	end

	# Transformacija za negativne i neograničene varijable
	neograniceneMapa = []
	for i in 1:lastindex(vsigns)
		if vsigns[i] == -1
			A[:, i] *= -1
			c[i] *= -1
		elseif vsigns[i] == 0
			c = [c -c[i]]
			A = [A -A[:, i]]
			push!(neograniceneMapa, (i, size(A, 2)))

		end
	end

	# Kreiranje početne simplex tabele
	(tabela, bazniIndeksi, umjetneVarijable) = kreiraj_simplex_tabelu(A, b, c, csigns)


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
			throw("Rjesenje je neograniceno");
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
            throw("Dopustiva oblast ne postoji");
		end
	end
	x = zeros(1, size(b, 1) + size(c, 2))

	# Dodela vrednosti baznim varijablama
	for i in 1:lastindex(bazniIndeksi)
		x[Int(round(bazniIndeksi[i]))] = tabela[i, 1]
	end


	# Analiza jedinstvenosti optimalnog rješenja
	jedinstveno = true
	for i in 2:(lastindex(tabela[end, :])-lastindex(umjetneVarijable))
		if x[i-1] == 0 && tabela[end, i] == 0
			jedinstveno = false
		end
	end

	jedinstvenostPoruka = ""
	if jedinstveno == true
		jedinstvenostPoruka = "Rjesenje je jedinstveno"
	else
		jedinstvenostPoruka = "Rjesenje nije jedinstveno"
	end

	# Rekonstrukcija rješenja za neograničene varijable
	# Varijable koje su bile rastavljene kao razlika se vraćaju u originalni oblik
	if !isempty(neograniceneMapa)
		for i in 1:lastindex(neograniceneMapa)
			prviIndeks = findall(y -> y == neograniceneMapa[i][1], x)
			drugiIndeks = findall(y -> y == neograniceneMapa[i][2], x)
			if !isempty(prviIndeks) && !isempty(drugiIndeks)

			elseif isempty(prviIndeks) && !isempty(drugiIndeks)
				x[neograniceneMapa[i][1]] = -x[drugiIndeks]
				deleteat!(x, drugiIndeks[1])
			end
		end
	end

	# Provjera degenerisanosti
	degenerirano = false
	for i in 1:(lastindex(tabela[:, 1])-2)
		if tabela[i, 1] == 0
			degenerirano = true
		end
	end

	degenerisanostPoruka = ""
	if degenerirano == true
		degenerisanostPoruka = "Rjesenje je degenerirano"
	else
		degenerisanostPoruka = "Rjesenje nije degenerisano"
	end


	# Korekcija znaka optimalne vrijednosti
	if goal == "min"
		Z = tabela[end, 1]
	else
		Z = -tabela[end, 1]
	end
	return Z, x, jedinstvenostPoruka, degenerisanostPoruka
end

# Funkcija za lijepo formatiranje i ispis Simplex tabele
function ispisi_simplex_tabelu(tabela, bazniIndeksi)
	m, n = size(tabela)
	
	# Formatiranje brojeva
	function format_broj(x)
		if abs(x) < 1e-10
			return "0"
		elseif abs(x - round(x)) < 1e-10
			return string(round(Int, x))
		else
			return @sprintf("%.4f", x)
		end
	end
	
	# Pronalaženje maksimalne širine za svaku kolonu
	# Prva kolona: bazna varijabla
	# Druga kolona: b vrijednosti (slobodna kolona)
	# Ostale kolone: koeficijenti varijabli
	kolona_sirine = [10]  # Za baznu varijablu
	kolona_sirine_b = [10]  # Za b kolonu (slobodni članovi)
	
	# Izračunaj širinu za b kolonu
	for i in 1:m
		str = format_broj(tabela[i, 1])
		kolona_sirine_b[1] = max(kolona_sirine_b[1], length(str) + 2)
	end
	kolona_sirine_b[1] = max(kolona_sirine_b[1], 10)  # Minimum za "    b    "
	
	for j in 2:n
		max_sirina = 10
		# Provjeri sve redove
		for i in 1:m
			str = format_broj(tabela[i, j])
			max_sirina = max(max_sirina, length(str) + 2)
		end
		# Provjeri i naziv kolone
		naziv = "x$(j-1)"
		max_sirina = max(max_sirina, length(naziv) + 2)
		push!(kolona_sirine, max_sirina)
	end
	
	# Ispis zaglavlja
	print("┌")
	# Bazna varijabla kolona
	print(repeat("─", kolona_sirine[1]))
	print("┬")
	# b kolona
	print(repeat("─", kolona_sirine_b[1]))
	# Ostale kolone
	for (idx, sirina) in enumerate(kolona_sirine[2:end])
		print("┬")
		print(repeat("─", sirina))
	end
	println("┐")
	
	# Ispis naziva kolona
	print("│")
	print(lpad("Bazna", kolona_sirine[1]))
	print("│")
	print(lpad("    b    ", kolona_sirine_b[1]))
	for j in 2:n
		print("│", lpad("x$(j-1)", kolona_sirine[j]))
	end
	println("│")
	
	# Linija ispod zaglavlja
	print("├")
	print(repeat("─", kolona_sirine[1]))
	print("┼")
	print(repeat("─", kolona_sirine_b[1]))
	for (idx, sirina) in enumerate(kolona_sirine[2:end])
		print("┼")
		print(repeat("─", sirina))
	end
	println("┤")
	
	# Ispis redova (osim zadnja dva - M red i Z red)
	for i in 1:(m-2)
		print("│")
		# Ispis bazne varijable
		if i <= length(bazniIndeksi) && bazniIndeksi[i] > 0
			baznaVar = Int(round(bazniIndeksi[i]))
			print(lpad("x$baznaVar", kolona_sirine[1]))
		else
			print(lpad("", kolona_sirine[1]))
		end
		print("│")
		# Ispis b vrijednosti (slobodne kolone)
		print(lpad(format_broj(tabela[i, 1]), kolona_sirine_b[1]))
		
		# Ispis koeficijenata
		for j in 2:n
			print("│", lpad(format_broj(tabela[i, j]), kolona_sirine[j]))
		end
		println("│")
	end
	
	# Linija iznad M reda
	print("├")
	print(repeat("─", kolona_sirine[1]))
	print("┼")
	print(repeat("─", kolona_sirine_b[1]))
	for (idx, sirina) in enumerate(kolona_sirine[2:end])
		print("┼")
		print(repeat("─", sirina))
	end
	println("┤")
	
	# M red
	print("│")
	print(lpad("  M   ", kolona_sirine[1]))
	print("│")
	print(lpad(format_broj(tabela[m-1, 1]), kolona_sirine_b[1]))
	for j in 2:n
		print("│", lpad(format_broj(tabela[m-1, j]), kolona_sirine[j]))
	end
	println("│")
	
	# Ciljni red (Z)
	print("│")
	print(lpad("  Z   ", kolona_sirine[1]))
	print("│")
	print(lpad(format_broj(tabela[m, 1]), kolona_sirine_b[1]))
	for j in 2:n
		print("│", lpad(format_broj(tabela[m, j]), kolona_sirine[j]))
	end
	println("│")
	
	# Donja linija
	print("└")
	print(repeat("─", kolona_sirine[1]))
	print("┴")
	print(repeat("─", kolona_sirine_b[1]))
	for (idx, sirina) in enumerate(kolona_sirine[2:end])
		print("┴")
		print(repeat("─", sirina))
	end
	println("┘")
end

# Funkcija za ispis iteracija Simplex tabele
function rijesi_simplex_sa_iteracijama(goal, A, b, c, csigns, vsigns)
	if size(b, 1) != size(A, 1) || size(c, 2) != size(A, 2)
		throw("Dimenzije ulaznih parametara nisu validni")
	end

	# Transformacija za negativne i neograničene varijable
	neograniceneMapa = []
	for i in 1:lastindex(vsigns)
		if vsigns[i] == -1
			A[:, i] *= -1
			c[i] *= -1
		elseif vsigns[i] == 0
			c = [c -c[i]]
			A = [A -A[:, i]]
			push!(neograniceneMapa, (i, size(A, 2)))
		end
	end

	# Kreiranje početne simplex tabele
	(tabela, bazniIndeksi, umjetneVarijable) = kreiraj_simplex_tabelu(A, b, c, csigns)

	# Prilagodba tabele ovisno o tipu optimizacije
	if goal == "min"
		tabela[end, :] *= -1
	else
		tabela[end-1, 1] = abs(tabela[end-1, 1])
	end

	iteracija = 0
	println("\n" * "=" ^ 70)
	println("ITERACIJA ", iteracija, " - Početna Simplex tabela")
	println("=" ^ 70)
	ispisi_simplex_tabelu(tabela, bazniIndeksi)
	println()

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

	while max_koef > 0 || max_M > 0
		iteracija += 1
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
			throw("Rjesenje je neograniceno");
		end
		
		bazniIndeksi[pivotRed] = pivotKolona - 1
		pivot = tabela[pivotRed, pivotKolona]
		
		println("\n" * "=" ^ 70)
		println("ITERACIJA ", iteracija)
		println("=" ^ 70)
		println("Pivot element: tabela[$pivotRed, $pivotKolona] = $(@sprintf("%.4f", pivot))")
		println("Vodeća kolona: $pivotKolona, Vodeći red: $pivotRed")
		println()
		
		tabela[pivotRed, :] ./= pivot

		for i in 1:size(tabela, 1)
			if i != pivotRed
				faktor = tabela[i, pivotKolona]
				for j in 1:size(tabela, 2)
					tabela[i, j] -= tabela[pivotRed, j] * faktor
				end
			end
		end

		println("Simplex tabela nakon pivotiranja:")
		ispisi_simplex_tabelu(tabela, bazniIndeksi)
		println()

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
            throw("Dopustiva oblast ne postoji");
		end
	end
	x = zeros(1, size(b, 1) + size(c, 2))

	# Dodela vrednosti baznim varijablama
	for i in 1:lastindex(bazniIndeksi)
		x[Int(round(bazniIndeksi[i]))] = tabela[i, 1]
	end

	# Analiza jedinstvenosti optimalnog rješenja
	jedinstveno = true
	for i in 2:(lastindex(tabela[end, :])-lastindex(umjetneVarijable))
		if x[i-1] == 0 && tabela[end, i] == 0
			jedinstveno = false
		end
	end

	jedinstvenostPoruka = ""
	if jedinstveno == true
		jedinstvenostPoruka = "Rjesenje je jedinstveno"
	else
		jedinstvenostPoruka = "Rjesenje nije jedinstveno"
	end

	# Rekonstrukcija rješenja za neograničene varijable
	if !isempty(neograniceneMapa)
		for i in 1:lastindex(neograniceneMapa)
			prviIndeks = findall(y -> y == neograniceneMapa[i][1], x)
			drugiIndeks = findall(y -> y == neograniceneMapa[i][2], x)
			if !isempty(prviIndeks) && !isempty(drugiIndeks)

			elseif isempty(prviIndeks) && !isempty(drugiIndeks)
				x[neograniceneMapa[i][1]] = -x[drugiIndeks]
				deleteat!(x, drugiIndeks[1])
			end
		end
	end

	# Provjera degenerisanosti
	degenerirano = false
	for i in 1:(lastindex(tabela[:, 1])-2)
		if tabela[i, 1] == 0
			degenerirano = true
		end
	end

	degenerisanostPoruka = ""
	if degenerirano == true
		degenerisanostPoruka = "Rjesenje je degenerirano"
	else
		degenerisanostPoruka = "Rjesenje nije degenerisano"
	end

	# Korekcija znaka optimalne vrijednosti
	if goal == "min"
		Z = tabela[end, 1]
	else
		Z = -tabela[end, 1]
	end
	return Z, x, jedinstvenostPoruka, degenerisanostPoruka
end

# ============================================================================
# PRIMJERI ZA LABORATORIJSKU VJEŽBU 4
# ============================================================================

# PRIMJER 1: PROBLEM MINIMIZACIJE sa ograničenjima tipa ≥
# Minimizirati: Z = 3x1 + 2x2
# Ograničenja:
#   x1 + x2 ≥ 4
#   2x1 + x2 ≥ 6
#   x1, x2 ≥ 0
println("=" ^ 80)
println("PRIMJER 1: PROBLEM MINIMIZACIJE sa ograničenjima tipa ≥")
println("=" ^ 80)
println("Minimizirati: Z = 3x1 + 2x2")
println("Ograničenja:")
println("  x1 + x2 ≥ 4")
println("  2x1 + x2 ≥ 6")
println("  x1, x2 ≥ 0")
println()

b1 = [4, 6]
A1 = [1 1; 2 1]
c1 = [3 2]
goal1 = "min"
csigns1 = [1, 1]  # ≥ ograničenja
vsigns1 = [1, 1]  # nenegativne varijable

try
	Z1, X1, jedinstveno1, degenerirano1 = rijesi_simplex(goal1, A1, b1, c1, csigns1, vsigns1)
	println("Optimalna vrijednost: Z = ", Z1)
	println("Optimalno rješenje: x1 = ", X1[1], ", x2 = ", X1[2])
	println(jedinstveno1)
	println(degenerirano1)
catch e
	println("Greška: ", e)
end
println("\n" * "=" ^ 80 * "\n")

# PRIMJER 2: PROBLEM KOJI NEMA RJEŠENJE (nepostojanje dopustive oblasti)
# Maksimizirati: Z = x1 + 2x2
# Ograničenja:
#   x1 + x2 ≤ 2
#   3x1 + 3x2 ≥ 4
#   x1, x2 ≥ 0
println("=" ^ 80)
println("PRIMJER 2: PROBLEM KOJI NEMA RJEŠENJE (nepostojanje dopustive oblasti)")
println("=" ^ 80)
println("Maksimizirati: Z = x1 + 2x2")
println("Ograničenja:")
println("  x1 + x2 ≤ 2")
println("  3x1 + 3x2 ≥ 9")
println("  x1, x2 ≥ 0")
println()

b2 = [2, 9]
A2 = [1 1; 3 3]
c2 = [1 2]
goal2 = "max"
csigns2 = [-1, 1]  # ≤ i ≥ ograničenja
vsigns2 = [1, 1]   # nenegativne varijable

try
	Z2, X2, jedinstveno2, degenerirano2 = rijesi_simplex(goal2, A2, b2, c2, csigns2, vsigns2)
	println("Optimalna vrijednost: Z = ", Z2)
	println("Optimalno rješenje: x1 = ", X2[1], ", x2 = ", X2[2])
	println(jedinstveno2)
	println(degenerirano2)
catch e
	println("GREŠKA: ", e)
	println("Ovo je očekivano - problem nema dopustivu oblast!")
end
println("\n" * "=" ^ 80 * "\n")

# PRIMJER 3: PROBLEM SA OGRANIČENJIMA TIPA ≥ I NEGATIVNIM bi (sa ispisom iteracija)
# Maksimizirati: Z = 2x1 + 3x2
# Ograničenja:
#   x1 + 2x2 ≥ 8
#   3x1 + 2x2 ≥ 12
#   x1, x2 ≥ 0
println("=" ^ 80)
println("PRIMJER 3: PROBLEM SA OGRANIČENJIMA TIPA ≥ (sa ispisom iteracija)")
println("=" ^ 80)
println("Maksimizirati: Z = 2x1 + 3x2")
println("Ograničenja:")
println("  x1 + 2x2 ≥ 8")
println("  3x1 + 2x2 ≥ 12")
println("  x1, x2 ≥ 0")
println()

b3 = [8, 12]
A3 = [1 2; 3 2]
c3 = [2 3]
goal3 = "max"
csigns3 = [1, 1]  # ≥ ograničenja
vsigns3 = [1, 1]  # nenegativne varijable

try
	Z3, X3, jedinstveno3, degenerirano3 = rijesi_simplex_sa_iteracijama(goal3, A3, b3, c3, csigns3, vsigns3)
	println("\n=== KONAČNO RJEŠENJE ===")
	println("Optimalna vrijednost: Z = ", Z3)
	println("Optimalno rješenje: x1 = ", X3[1], ", x2 = ", X3[2])
	println(jedinstveno3)
	println(degenerirano3)
catch e
	println("Greška: ", e)
end
println("\n" * "=" ^ 80 * "\n")