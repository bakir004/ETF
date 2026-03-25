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

	# Pronalaženje maksimalnog koeficijenta u M redu (sa nasumičnim izborom ako postoji više opcija)
	M_red = deepcopy(tabela[end-1, :])
	popfirst!(M_red)
	(max_M, kolona_M) = findmax(M_red)
	kolona_M += 1
	
	# Pronađi sve kolone sa maksimalnim M koeficijentom
	if max_M > 0
		M_kandidati = Int[]
		for i in 2:lastindex(tabela[end-1, :])
			if abs(tabela[end-1, i] - max_M) < 1e-10
				push!(M_kandidati, i)
			end
		end
		if length(M_kandidati) > 1
			kolona_M = M_kandidati[rand(1:length(M_kandidati))]
		end
	end

	kolonaIndeks = 0
	M_red_puni = tabela[end-1, :]
	ciljniRed = tabela[end, :]
	max_koef = -Inf
	kandidati = Int[]
	for i in 2:lastindex(ciljniRed)
		if ciljniRed[i] > max_koef && (M_red_puni[i] >= 0 || M_red_puni[i] == -0)
			max_koef = ciljniRed[i]
			empty!(kandidati)
			push!(kandidati, i)
		elseif abs(ciljniRed[i] - max_koef) < 1e-10 && (M_red_puni[i] >= 0 || M_red_puni[i] == -0)
			push!(kandidati, i)
		end
	end
	if length(kandidati) > 0
		kolonaIndeks = kandidati[rand(1:length(kandidati))]
	end

	while max_koef > 0 || max_M > 0
		if max_M > 0
			pivotKolona = kolona_M
		else
			pivotKolona = kolonaIndeks
		end

		# Pronađi sve redove sa minimalnim ratio (sa nasumičnim izborom ako postoji više opcija)
		min_ratio = Inf
		ratio_kandidati = Int[]
		for i in 1:size(tabela, 1)-2
			if tabela[i, pivotKolona] > 0
				ratio = tabela[i, 1] / tabela[i, pivotKolona]
				if ratio < min_ratio
					min_ratio = ratio
					empty!(ratio_kandidati)
					push!(ratio_kandidati, i)
				elseif abs(ratio - min_ratio) < 1e-10
					push!(ratio_kandidati, i)
				end
			end
		end
		
		if length(ratio_kandidati) == 0
			throw("Rjesenje je neograniceno")
		end
		
		# Nasumično odaberi između kandidata sa minimalnim ratio
		pivotRed = ratio_kandidati[rand(1:length(ratio_kandidati))]

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
		
		# Pronađi sve kolone sa maksimalnim M koeficijentom (sa nasumičnim izborom ako postoji više opcija)
		if max_M > 0
			M_kandidati = Int[]
			for i in 2:lastindex(tabela[end-1, :])
				if abs(tabela[end-1, i] - max_M) < 1e-10
					push!(M_kandidati, i)
				end
			end
			if length(M_kandidati) > 1
				kolona_M = M_kandidati[rand(1:length(M_kandidati))]
			end
		end

		if max_M <= 1e-8
			max_M = 0
		end

		if max_M <= 0
			M_red_puni = tabela[end-1, :]
			ciljniRed = tabela[end, :]
			max_koef = -Inf
			kandidati = Int[]
			for i in 2:lastindex(ciljniRed)
				if ciljniRed[i] > max_koef && (M_red_puni[i] >= 0 || M_red_puni[i] == -0)
					max_koef = ciljniRed[i]
					empty!(kandidati)
					push!(kandidati, i)
				elseif abs(ciljniRed[i] - max_koef) < 1e-10 && (M_red_puni[i] >= 0 || M_red_puni[i] == -0)
					push!(kandidati, i)
				end
			end
			if length(kandidati) > 0
				kolonaIndeks = kandidati[rand(1:length(kandidati))]
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

	# Pronalaženje maksimalnog koeficijenta u M redu (sa nasumičnim izborom ako postoji više opcija)
	M_red = deepcopy(tabela[end-1, :])
	popfirst!(M_red)
	(max_M, kolona_M) = findmax(M_red)
	kolona_M += 1
	
	# Pronađi sve kolone sa maksimalnim M koeficijentom
	if max_M > 0
		M_kandidati = Int[]
		for i in 2:lastindex(tabela[end-1, :])
			if abs(tabela[end-1, i] - max_M) < 1e-10
				push!(M_kandidati, i)
			end
		end
		if length(M_kandidati) > 1
			kolona_M = M_kandidati[rand(1:length(M_kandidati))]
		end
	end

	kolonaIndeks = 0
	M_red_puni = tabela[end-1, :]
	ciljniRed = tabela[end, :]
	max_koef = -Inf
	kandidati = Int[]
	for i in 2:lastindex(ciljniRed)
		if ciljniRed[i] > max_koef && (M_red_puni[i] >= 0 || M_red_puni[i] == -0)
			max_koef = ciljniRed[i]
			empty!(kandidati)
			push!(kandidati, i)
		elseif abs(ciljniRed[i] - max_koef) < 1e-10 && (M_red_puni[i] >= 0 || M_red_puni[i] == -0)
			push!(kandidati, i)
		end
	end
	if length(kandidati) > 0
		kolonaIndeks = kandidati[rand(1:length(kandidati))]
	end

	while max_koef > 0 || max_M > 0
		iteracija += 1
		if max_M > 0
			pivotKolona = kolona_M
		else
			pivotKolona = kolonaIndeks
		end

		# Pronađi sve redove sa minimalnim ratio (sa nasumičnim izborom ako postoji više opcija)
		min_ratio = Inf
		ratio_kandidati = Int[]
		for i in 1:size(tabela, 1)-2
			if tabela[i, pivotKolona] > 0
				ratio = tabela[i, 1] / tabela[i, pivotKolona]
				if ratio < min_ratio
					min_ratio = ratio
					empty!(ratio_kandidati)
					push!(ratio_kandidati, i)
				elseif abs(ratio - min_ratio) < 1e-10
					push!(ratio_kandidati, i)
				end
			end
		end
		
		if length(ratio_kandidati) == 0
			throw("Rjesenje je neograniceno")
		end
		
		# Nasumično odaberi između kandidata sa minimalnim ratio
		pivotRed = ratio_kandidati[rand(1:length(ratio_kandidati))]
		
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
		
		# Pronađi sve kolone sa maksimalnim M koeficijentom (sa nasumičnim izborom ako postoji više opcija)
		if max_M > 0
			M_kandidati = Int[]
			for i in 2:lastindex(tabela[end-1, :])
				if abs(tabela[end-1, i] - max_M) < 1e-10
					push!(M_kandidati, i)
				end
			end
			if length(M_kandidati) > 1
				kolona_M = M_kandidati[rand(1:length(M_kandidati))]
			end
		end

		if max_M <= 1e-8
			max_M = 0
		end

		if max_M <= 0
			M_red_puni = tabela[end-1, :]
			ciljniRed = tabela[end, :]
			max_koef = -Inf
			kandidati = Int[]
			for i in 2:lastindex(ciljniRed)
				if ciljniRed[i] > max_koef && (M_red_puni[i] >= 0 || M_red_puni[i] == -0)
					max_koef = ciljniRed[i]
					empty!(kandidati)
					push!(kandidati, i)
				elseif abs(ciljniRed[i] - max_koef) < 1e-10 && (M_red_puni[i] >= 0 || M_red_puni[i] == -0)
					push!(kandidati, i)
				end
			end
			if length(kandidati) > 0
				kolonaIndeks = kandidati[rand(1:length(kandidati))]
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
# println("=" ^ 80)
# println("PRIMJER 1: PROBLEM MINIMIZACIJE sa ograničenjima tipa ≥")
# println("=" ^ 80)
# println("Minimizirati: Z = 3x1 + 2x2")
# println("Ograničenja:")
# println("  x1 + x2 ≥ 4")
# println("  2x1 + x2 ≥ 6")
# println("  x1, x2 ≥ 0")
# println()

# b1 = [4, 6]
# A1 = [1 1; 2 1]
# c1 = [3 2]
# goal1 = "min"
# csigns1 = [1, 1]  # ≥ ograničenja
# vsigns1 = [1, 1]  # nenegativne varijable

# try
# 	Z1, X1, jedinstveno1, degenerirano1 = rijesi_simplex(goal1, A1, b1, c1, csigns1, vsigns1)
# 	println("Optimalna vrijednost: Z = ", Z1)
# 	println("Optimalno rješenje: x1 = ", X1[1], ", x2 = ", X1[2])
# 	println(jedinstveno1)
# 	println(degenerirano1)
# catch e
# 	println("Greška: ", e)
# end
# println("\n" * "=" ^ 80 * "\n")

# # PRIMJER 2: PROBLEM KOJI NEMA RJEŠENJE (nepostojanje dopustive oblasti)
# # Maksimizirati: Z = x1 + 2x2
# # Ograničenja:
# #   x1 + x2 ≤ 2
# #   3x1 + 3x2 ≥ 4
# #   x1, x2 ≥ 0
# println("=" ^ 80)
# println("PRIMJER 2: PROBLEM KOJI NEMA RJEŠENJE (nepostojanje dopustive oblasti)")
# println("=" ^ 80)
# println("Maksimizirati: Z = x1 + 2x2")
# println("Ograničenja:")
# println("  x1 + x2 ≤ 2")
# println("  3x1 + 2x2 ≥ 2")
# println("  x1, x2 ≥ 0")
# println()

# b2 = [2, 2]
# A2 = [1 1; 3 2]
# c2 = [1 2]
# goal2 = "max"
# csigns2 = [-1, 0]  # ≤ i ≥ ograničenja
# vsigns2 = [1, 1]   # nenegativne varijable

# try
# 	Z2, X2, jedinstveno2, degenerirano2 = rijesi_simplex_sa_iteracijama(goal2, A2, b2, c2, csigns2, vsigns2)
# 	println("Optimalna vrijednost: Z = ", Z2)
# 	println("Optimalno rješenje: x1 = ", X2[1], ", x2 = ", X2[2])
# 	println(jedinstveno2)
# 	println(degenerirano2)
# catch e
# 	println("GREŠKA: ", e)
# 	println("Ovo je očekivano - problem nema dopustivu oblast!")
# end
# println("\n" * "=" ^ 80 * "\n")

# # PRIMJER 3: PROBLEM SA OGRANIČENJIMA TIPA ≥ I NEGATIVNIM bi (sa ispisom iteracija)
# # Maksimizirati: Z = 2x1 + 3x2
# # Ograničenja:
# #   x1 + 2x2 ≥ 8
# #   3x1 + 2x2 ≥ 12
# #   x1, x2 ≥ 0
# println("=" ^ 80)
# println("PRIMJER 3: PROBLEM SA OGRANIČENJIMA TIPA ≥ (sa ispisom iteracija)")
# println("=" ^ 80)
# println("Maksimizirati: Z = 2x1 + 3x2")
# println("Ograničenja:")
# println("  x1 + 2x2 ≥ 8")
# println("  3x1 + 2x2 ≥ 12")
# println("  x1, x2 ≥ 0")
# println()

# b3 = [8, 12]
# A3 = [1 2; 3 2]
# c3 = [2 3]
# goal3 = "max"
# csigns3 = [1, 1]  # ≥ ograničenja
# vsigns3 = [1, 1]  # nenegativne varijable

# try
# 	Z3, X3, jedinstveno3, degenerirano3 = rijesi_simplex_sa_iteracijama(goal3, A3, b3, c3, csigns3, vsigns3)
# 	println("\n=== KONAČNO RJEŠENJE ===")
# 	println("Optimalna vrijednost: Z = ", Z3)
# 	println("Optimalno rješenje: x1 = ", X3[1], ", x2 = ", X3[2])
# 	println(jedinstveno3)
# 	println(degenerirano3)
# catch e
# 	println("Greška: ", e)
# end
# println("\n" * "=" ^ 80 * "\n")

# # PRIMJER 4: STANDARDNI PROBLEM MAKSIMIZACIJE SA ≤ OGRANIČENJIMA
# # Maksimizirati: Z = 3x1 + 5x2
# # Ograničenja:
# #   2x1 + 3x2 ≤ 25
# #   x1 + 2x2 ≤ 15
# #   x1, x2 ≥ 0
# println("=" ^ 80)
# println("PRIMJER 4: STANDARDNI PROBLEM MAKSIMIZACIJE SA ≤ OGRANIČENJIMA")
# println("=" ^ 80)
# println("Maksimizirati: Z = 3x1 + 5x2")
# println("Ograničenja:")
# println("  2x1 + 3x2 ≤ 25")
# println("  x1 + 2x2 ≤ 15")
# println("  x1, x2 ≥ 0")
# println()

# b4 = [25, 15]
# A4 = [2 3; 1 2]
# c4 = [3 5]
# goal4 = "max"
# csigns4 = [-1, -1]  # ≤ ograničenja
# vsigns4 = [1, 1]    # nenegativne varijable

# try
# 	Z4, X4, jedinstveno4, degenerirano4 = rijesi_simplex_sa_iteracijama(goal4, A4, b4, c4, csigns4, vsigns4)
# 	println("Optimalna vrijednost: Z = ", Z4)
# 	println("Optimalno rješenje: x1 = ", X4[1], ", x2 = ", X4[2])
# 	println(jedinstveno4)
# 	println(degenerirano4)
# catch e
# 	println("Greška: ", e)
# end
# println("\n" * "=" ^ 80 * "\n")

# PRIMJER 5: PRIMALNI PROBLEM
# Minimizirati: Z = 5*x1 - 10*x2 + 16*x3
# Ograničenja:
#   -3*x1 + 0*x2 - 4*x3 <= -4
#   3*x1 - 6*x2 + 2*x3 = 11
#   x1 ≥ 0, x2 ≤ 0, x3 ≥ 0
println("=" ^ 80)
println("PRIMJER 5: PRIMALNI PROBLEM")
println("=" ^ 80)
println("Minimizirati: Z = 5*x1 - 10*x2 + 16*x3")
println("Ograničenja:")
println("  -3*x1 + 0*x2 - 4*x3 <= -4")
println("  3*x1 - 6*x2 + 2*x3 = 11")
println("  x1 ≥ 0, x2 ≤ 0, x3 ≥ 0")
println()

b5_primal = [-4, 11]
A5_primal = [-3 0 -4; 3 -6 2]
c5_primal = [5 -10 16]
goal5_primal = "min"
csigns5_primal = [-1, 0]  # <=, = ograničenja
vsigns5_primal = [1, -1, 1]  # x1 ≥ 0, x2 ≤ 0, x3 ≥ 0

try
	Z5_primal, X5_primal, jedinstveno5_primal, degenerirano5_primal = rijesi_simplex_sa_iteracijama(
		goal5_primal, A5_primal, b5_primal, c5_primal, csigns5_primal, vsigns5_primal
	)
	println("\n=== KONAČNO RJEŠENJE PRIMALNOG PROBLEMA ===")
	println("Optimalna vrijednost: Z = ", Z5_primal)
	println("Optimalno rješenje: x1 = ", X5_primal[1], ", x2 = ", X5_primal[2], ", x3 = ", X5_primal[3])
	println(jedinstveno5_primal)
	println(degenerirano5_primal)
catch e
	println("Greška: ", e)
end
println("\n" * "=" ^ 80 * "\n")

# PRIMJER 6: DUALNI PROBLEM SA NEPOZITIVNOM I NEOGRANIČENOM VARIJABLOM
# Maksimizirati: W = -4*y1 + 11*y2
# Ograničenja:
#   -3*y1 + 3*y2 <= 5
#   0*y1 - 6*y2 >= -10
#   -4*y1 + 2*y2 = 16
#   y1 ≤ 0, y2 neograničeno
println("=" ^ 80)
println("PRIMJER 6: DUALNI PROBLEM (dual od Primjera 5)")
println("=" ^ 80)
println("Maksimizirati: W = -4*y1 + 11*y2")
println("Ograničenja:")
println("  -3*y1 + 3*y2 <= 5")
println("  0*y1 - 6*y2 >= -10")
println("  -4*y1 + 2*y2 <= 16")
println("  y1 ≤ 0, y2 neograničeno")
println()

b6 = [5, 10, 16]
A6 = [-3 3; 0 6; -4 2]
c6 = [-4 11]
goal6 = "max"
csigns6 = [-1, -1, -1]  # <=, >=, <= ograničenja
vsigns6 = [-1, 0]     # y1 ≤ 0 (nepozitivna), y2 neograničeno

try
	Z6, X6, jedinstveno6, degenerirano6 = rijesi_simplex_sa_iteracijama(goal6, A6, b6, c6, csigns6, vsigns6)
	println("\n=== KONAČNO RJEŠENJE DUALNOG PROBLEMA ===")
	println("Optimalna vrijednost: W = ", Z6)
	println("Optimalno rješenje: y1 = ", X6[1], ", y2 = ", X6[2])
	println(jedinstveno6)
	println(degenerirano6)
	println("\nNAPOMENA: Optimalna vrijednost dualnog problema W treba biti jednaka")
	println("optimalnoj vrijednosti primalnog problema Z (slabost dualnosti).")
catch e
	println("Greška: ", e)
end
println("\n" * "=" ^ 80 * "\n")

# PRIMJER 7: PRIMALNI PROBLEM
# Maksimizirati: Z = 10x1 + 25x2
# Ograničenja:
#   x1 ≤ 100
#   x1 + 2x2 ≤ 120
#   -2x1 + 8x2 ≤ 0
#   x1 ≥ 0, x2 ≥ 0
println("=" ^ 80)
println("PRIMJER 7: PRIMALNI PROBLEM")
println("=" ^ 80)
println("Maksimizirati: Z = 10x1 + 25x2")
println("Ograničenja:")
println("  x1 ≤ 100")
println("  x1 + 2x2 ≤ 120")
println("  -2x1 + 8x2 ≤ 0")
println("  x1 ≥ 0, x2 ≥ 0")
println()

b7_primal = [100, 120, 0]
A7_primal = [1 0; 1 2; -2 8]
c7_primal = [10 25]
goal7_primal = "max"
csigns7_primal = [-1, -1, -1]  # ≤ ograničenja
vsigns7_primal = [1, 1]  # nenegativne varijable

try
	Z7_primal, X7_primal, jedinstveno7_primal, degenerirano7_primal = rijesi_simplex_sa_iteracijama(
		goal7_primal, A7_primal, b7_primal, c7_primal, csigns7_primal, vsigns7_primal
	)
	println("\n=== KONAČNO RJEŠENJE PRIMALNOG PROBLEMA ===")
	println("Optimalna vrijednost: Z = ", Z7_primal)
	println("Optimalno rješenje: x1 = ", X7_primal[1], ", x2 = ", X7_primal[2])
	println(jedinstveno7_primal)
	println(degenerirano7_primal)
catch e
	println("Greška: ", e)
end
println("\n" * "=" ^ 80 * "\n")

# PRIMJER 8: DUALNI PROBLEM (dual od Primjera 7)
# Minimizirati: W = 100y1 + 120y2 + 0y3
# Ograničenja:
#   y1 + y2 - 2y3 ≥ 10
#   2y2 + 8y3 ≥ 25
#   y1, y2, y3 ≥ 0
println("=" ^ 80)
println("PRIMJER 8: DUALNI PROBLEM (dual od Primjera 7)")
println("=" ^ 80)
println("Minimizirati: W = 100y1 + 120y2 + 0y3")
println("Ograničenja:")
println("  y1 + y2 - 2y3 ≥ 10")
println("  2y2 + 8y3 ≥ 25")
println("  y1, y2, y3 ≥ 0")
println()

b8_dual = [10, 25]
A8_dual = [1 1 -2; 0 2 8]
c8_dual = [100 120 0]
goal8_dual = "min"
csigns8_dual = [1, 1]  # ≥ ograničenja
vsigns8_dual = [1, 1, 1]  # nenegativne varijable

try
	Z8_dual, X8_dual, jedinstveno8_dual, degenerirano8_dual = rijesi_simplex_sa_iteracijama(
		goal8_dual, A8_dual, b8_dual, c8_dual, csigns8_dual, vsigns8_dual
	)
	println("\n=== KONAČNO RJEŠENJE DUALNOG PROBLEMA ===")
	println("Optimalna vrijednost: W = ", Z8_dual)
	println("Optimalno rješenje: y1 = ", X8_dual[1], ", y2 = ", X8_dual[2], ", y3 = ", X8_dual[3])
	println(jedinstveno8_dual)
	println(degenerirano8_dual)
	println("\nNAPOMENA: Optimalna vrijednost dualnog problema W treba biti jednaka")
	println("optimalnoj vrijednosti primalnog problema Z (slabost dualnosti).")
catch e
	println("Greška: ", e)
end
println("\n" * "=" ^ 80 * "\n")