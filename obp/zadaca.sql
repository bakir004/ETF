-- 1
SELECT k.naziv AS kontinent, nvl(d.naziv, 'Nema drzave') AS drzava, nvl(g.naziv, 'Nema grada') AS grad, count(l.lokacija_id) AS brojlokacija
FROM kontinent k
LEFT JOIN drzava d ON d.kontinent_id = k.kontinent_id
LEFT JOIN grad g ON g.drzava_id = d.drzava_id
LEFT JOIN lokacija l ON l.grad_id = g.grad_id
GROUP BY k.naziv, d.naziv, g.naziv
ORDER BY 1, 2, 3

-- 2
SELECT * FROM pravno_lice pl, ugovor_za_pravno_lice uzpl
WHERE uzpl.pravno_lice_id=pl.pravno_lice_id
AND (datum_raskidanja IS NULL OR datum_raskidanja > sysdate)
AND MOD(to_number(to_char(uzpl.datum_potpisivanja, 'yyyy')), 2) = 0

-- 3
SELECT d.naziv drzava, p.naziv proizvod, k.kolicina_proizvoda
FROM kolicina k, skladiste s, proizvod p, lokacija l, grad g, drzava d 
WHERE s.skladiste_id = k.skladiste_id 
AND k.proizvod_id = p.proizvod_id
AND l.lokacija_id = s.lokacija_id
AND g.grad_id = l.grad_id
AND d.drzava_id = g.drzava_id
AND k.kolicina_proizvoda > 50
AND d.naziv NOT LIKE '%ss%'

-- 4
SELECT * FROM proizvod;
SELECT * FROM kolicina;
SELECT * FROM popust;
SELECT * FROM narudzba_proizvoda;

SELECT DISTINCT p.naziv, p.broj_mjeseci_garancije 
FROM proizvod p, narudzba_proizvoda np
WHERE np.proizvod_id = p.proizvod_id
AND MOD(p.broj_mjeseci_garancije,3)=0
AND np.popust_id IS NOT NULL

-- 5
SELECT fl.ime || ' ' || fl.prezime AS "ime i prezime", o.naziv AS "Naziv odjela", '19705' indeks
FROM fizicko_lice fl, odjel o, uposlenik u
WHERE fl.fizicko_lice_id = u.uposlenik_id 
AND u.odjel_id = o.odjel_id
AND u.uposlenik_id != o.sef_id

-- 6
SELECT np.narudzba_id, p.cijena cijena, nvl(pop.postotak, 0) postotak, nvl(pop.postotak,0)/100 postotakRealni 
FROM narudzba_proizvoda np
LEFT JOIN popust pop ON np.popust_id = pop.popust_id
INNER JOIN proizvod p ON np.proizvod_id = p.proizvod_id
WHERE ((nvl(pop.postotak,0)/100))*p.cijena < 200

-- 7
SELECT naziv "Kategorija", 
DECODE(nadkategorija_id, NULL, 'Nema kategorije', 1, 'Komp Oprema') AS "Nadkategorija"
FROM kategorija

-- 8
SELECT nvl(u.datum_raskidanja, ADD_MONTHS(u.datum_potpisivanja, 24)) AS raskid FROM ugovor_za_pravno_lice u
WHERE to_number(substr(u.ugovor_id, 1, 2)) <= 50

-- 9
SELECT * FROM uposlenik;
SELECT * FROM odjel;
SELECT * FROM fizicko_lice;

SELECT fl.ime ime, fl.prezime prezime, 
DECODE(o.naziv,
	'Management', 'MANAGER',
	'Human Resources', 'HUMAN',
	'OTHER'
) AS odjel,
o.odjel_id odjel_id
FROM fizicko_lice fl, uposlenik u, odjel o
WHERE u.odjel_id = o.odjel_id AND u.uposlenik_id = fl.fizicko_lice_id
ORDER BY fl.ime ASC, fl.prezime DESC

-- 10
SELECT * FROM kategorija;
SELECT * FROM proizvod p
LEFT JOIN kategorija k ON p.kategorija_id = k.kategorija_id ORDER BY k.kategorija_id, cijena;

SELECT p.kategorija_id, 
	(SELECT p2.naziv 
	FROM proizvod p2 
	WHERE p2.kategorija_id = p.kategorija_id 
	AND p2.cijena = (SELECT min(p3.cijena) 
					FROM proizvod p3 WHERE p3.kategorija_id = p.kategorija_id)) Najjeftiniji, 
(SELECT p2.naziv 
	FROM proizvod p2 
	WHERE p2.kategorija_id = p.kategorija_id 
	AND p2.cijena = (SELECT max(p3.cijena) 
					FROM proizvod p3 WHERE p3.kategorija_id = p.kategorija_id)) Najskuplji, 
sum(p.cijena) ZCijena
FROM proizvod p
GROUP BY p.kategorija_id
ORDER BY min(p.cijena)+max(p.cijena)

SELECT SUM(Length(Najjeftiniji)*3+ZCijena*3) FROM
(SELECT p.kategorija_id, 
	(SELECT p2.naziv 
	FROM proizvod p2 
	WHERE p2.kategorija_id = p.kategorija_id 
	AND p2.cijena = (SELECT min(p3.cijena) 
					FROM proizvod p3 WHERE p3.kategorija_id = p.kategorija_id)) Najjeftiniji, 
(SELECT p2.naziv 
	FROM proizvod p2 
	WHERE p2.kategorija_id = p.kategorija_id 
	AND p2.cijena = (SELECT max(p3.cijena) 
					FROM proizvod p3 WHERE p3.kategorija_id = p.kategorija_id)) Najskuplji, sum(p.cijena) ZCijena FROM proizvod p
GROUP BY p.kategorija_id
ORDER BY min(p.cijena)+max(p.cijena))
WHERE ROWNUM<4;













