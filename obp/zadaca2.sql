ALTER SESSION SET current_schema = erd;

SELECT * FROM fizicko_lice ORDER BY lokacija_id;
SELECT * FROM pravno_lice ORDER BY lokacija_id;

--1.
SELECT DISTINCT pl.naziv AS ResNaziv FROM pravno_lice pl
WHERE 0 < (SELECT Count(*) FROM fizicko_lice fl WHERE pl.lokacija_id = fl.lokacija_id);
 
--2.
SELECT * FROM ugovor_za_pravno_lice;
SELECT * FROM faktura;
SELECT * FROM kupac;
SELECT * FROM proizvod;
SELECT * FROM isporuka;
SELECT * FROM kurirska_sluzba;
SELECT * FROM narudzba_proizvoda;

SELECT * FROM ugovor_za_pravno_lice;

SELECT * FROM faktura f
LEFT JOIN narudzba_proizvoda np ON np.faktura_id = f.faktura_id
LEFT JOIN proizvod p ON np.proizvod_id = p.proizvod_id
WHERE p.broj_mjeseci_garanice IS NOT NULL
GROUP BY f.datum_kupoprodaje



--2. Rezultat: 402 
SELECT Sum(Length(ResNaziv)*3 + Length("Datum Potpisivanja")*3) FROM
(ZAMJENA);
 
--3. Rezultat: 51
SELECT Sum(Length(naziv)*3) FROM
(ZAMJENA);
 
--4. Rezultat: 504  
SELECT Sum(Length("Proizvod")*3 + Length("Proizvodjac")*3) FROM
(ZAMJENA);
 
--5. Rezultat: 6897 
SELECT Sum(Length("Ime i prezime")*3 + "iznos"*3) FROM
(ZAMJENA); 
 
--6. Rezultat: 18
SELECT Sum(Length("naziv")*3) FROM
(ZAMJENA);

--7. Rezultat 17709
SELECT Sum(Length("Kupac")*3 + Round("Usteda")*3) FROM
(ZAMJENA);
 
--8. Rezultat: 243
 SELECT Sum(idisporuke*3 + idkurirske*3) FROM
(ZAMJENA);
 
--9. Rezultat: 9210 
 SELECT Sum(Length(naziv)*3 + cijena*3) FROM
(ZAMJENA);

--10. Rezultat: 2448
SELECT Sum(Length(naziv)*3 + Round(cijena)*3) FROM
(ZAMJENA);
 