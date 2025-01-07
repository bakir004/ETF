ALTER SESSION SET current_schema = erd;

--ZADATAK 1
--1.
SELECT DISTINCT pl.naziv AS ResNaziv FROM pravno_lice pl
WHERE 0 < (SELECT Count(*) FROM fizicko_lice fl WHERE pl.lokacija_id = fl.lokacija_id)
 
--2.
SELECT DISTINCT to_char(uzpl.datum_potpisivanja, 'dd.MM.yyyy') "Datum Potpisivanja", pl.naziv ResNaziv FROM ugovor_za_pravno_lice uzpl
LEFT JOIN pravno_lice pl ON pl.pravno_lice_id = uzpl.pravno_lice_id
WHERE uzpl.datum_potpisivanja > (
SELECT DISTINCT f.datum_kupoprodaje FROM faktura f
LEFT JOIN narudzba_proizvoda np ON np.faktura_id = f.faktura_id
LEFT JOIN proizvod p ON np.proizvod_id = p.proizvod_id
WHERE p.broj_mjeseci_garancije IS NOT NULL AND f.datum_kupoprodaje = (
																	SELECT DISTINCT min(f.datum_kupoprodaje) FROM faktura f
																	LEFT JOIN narudzba_proizvoda np ON np.faktura_id = f.faktura_id
																	LEFT JOIN proizvod p ON np.proizvod_id = p.proizvod_id
																	WHERE p.broj_mjeseci_garancije IS NOT NULL
																	)
)

--3.
SELECT p.naziv FROM proizvod p, kategorija k
WHERE p.kategorija_id = k.kategorija_id
AND p.kategorija_id = (
	SELECT k.kategorija_id FROM proizvod p
	LEFT JOIN kategorija k ON p.kategorija_id = k.kategorija_id
	LEFT JOIN kolicina kol ON kol.proizvod_id = p.proizvod_id
	HAVING sum(kol.kolicina_proizvoda) = (SELECT Max(sum(k.kolicina_proizvoda)) FROM proizvod p, kolicina k
										 WHERE p.proizvod_id = k.proizvod_id
										 GROUP BY p.proizvod_id)
	GROUP BY k.kategorija_id
)

--4.
SELECT p.naziv "Proizvod", pl.naziv "Proizvodjac" FROM proizvod p, proizvodjac pz, pravno_lice pl
WHERE p.proizvodjac_id = pz.proizvodjac_id AND pz.proizvodjac_id = pl.pravno_lice_id
AND (SELECT avg(p.cijena) FROM proizvod p) <ANY (SELECT p2.cijena FROM proizvod p2 WHERE p2.proizvodjac_id = pz.proizvodjac_id)


--5.
SELECT fl.ime || ' ' || fl.prezime AS "Ime i prezime", sum(f.iznos) "iznos" FROM fizicko_lice fl, uposlenik u, kupac k, faktura f
WHERE u.uposlenik_id = k.kupac_id
AND u.uposlenik_id = fl.fizicko_lice_id
AND f.kupac_id = k.kupac_id
GROUP BY fl.ime, fl.prezime
HAVING sum(f.iznos) > (SELECT avg(t.sum) FROM (SELECT sum(f.iznos) sum FROM fizicko_lice fl, kupac k, faktura f
											WHERE fl.fizicko_lice_id = k.kupac_id
											AND f.kupac_id = k.kupac_id
											GROUP BY fl.ime, fl.prezime) t
					)

--6.
SELECT pl.naziv "naziv" FROM narudzba_proizvoda np
JOIN proizvod p ON np.proizvod_id = p.proizvod_id
JOIN faktura f ON np.faktura_id = f.faktura_id
JOIN isporuka i ON f.isporuka_id = i.isporuka_id
JOIN kurirska_sluzba ks ON ks.kurirska_sluzba_id = i.kurirska_sluzba_id
JOIN pravno_lice pl ON pl.pravno_lice_id = ks.kurirska_sluzba_id
WHERE np.popust_id IS NOT NULL
GROUP BY ks.kurirska_sluzba_id, pl.naziv
HAVING sum(np.kolicina_jednog_proizvoda) = (SELECT max(t.sum) FROM (SELECT sum(np.kolicina_jednog_proizvoda) sum FROM narudzba_proizvoda np
											JOIN proizvod p ON np.proizvod_id = p.proizvod_id
											JOIN faktura f ON np.faktura_id = f.faktura_id
											JOIN isporuka i ON f.isporuka_id = i.isporuka_id
											JOIN kurirska_sluzba ks ON ks.kurirska_sluzba_id = i.kurirska_sluzba_id
											WHERE np.popust_id IS NOT NULL
											GROUP BY ks.kurirska_sluzba_id) t)

											
--7.										
SELECT fl.ime || ' ' || fl.prezime "Kupac", sum(pop.postotak/100 * p.cijena * np.kolicina_jednog_proizvoda) "Usteda", '19705' "Indeks" FROM narudzba_proizvoda np
JOIN proizvod p ON np.proizvod_id = p.proizvod_id
JOIN faktura f ON np.faktura_id = f.faktura_id
JOIN kupac k ON f.kupac_id = k.kupac_id
JOIN popust pop ON np.popust_id = pop.popust_id
JOIN fizicko_lice fl ON fl.fizicko_lice_id = k.kupac_id
GROUP BY fl.ime, fl.prezime


--8.
SELECT DISTINCT i.isporuka_id idisporuke, i.kurirska_sluzba_id idkurirske FROM isporuka i
JOIN faktura f ON f.isporuka_id = i.isporuka_id
JOIN narudzba_proizvoda np ON np.faktura_id = f.faktura_id
JOIN proizvod p ON p.proizvod_id = np.proizvod_id
WHERE p.broj_mjeseci_garancije IS NOT NULL
AND np.popust_id IS NOT NULL

--9.
SELECT naziv, cijena FROM proizvod 
WHERE cijena > (SELECT avg(t.sum) FROM (SELECT max(p.cijena) sum FROM proizvod p, kategorija k
				WHERE p.kategorija_id = k.kategorija_id
				GROUP BY k.kategorija_id) t)
				

--10.
SELECT p.naziv, p.cijena FROM proizvod p
WHERE cijena <ALL (SELECT avg(p2.cijena) FROM proizvod p2, kategorija k
				   WHERE p2.kategorija_id = k.kategorija_id
				   AND k.nadkategorija_id != p.kategorija_id
				   GROUP BY k.kategorija_id)
				   
				   
				   
				   
				   
--ZADATAK 2
ALTER SESSION SET current_schema = bc19705;	   

DROP TABLE TabelaA;
CREATE TABLE TabelaA (
	id NUMBER PRIMARY KEY,
	naziv VARCHAR2(5),
	datum DATE,
	cijelibroj NUMBER,
	realnibroj NUMBER,
	CONSTRAINT realni_veci_od_5 CHECK (realnibroj>5),
	CONSTRAINT cijeli_izmedju_5_i_15 CHECK (cijelibroj NOT BETWEEN 5 AND 15)
)

INSERT INTO TabelaA VALUES (1, 'tekst', NULL, NULL, 6.2);
INSERT INTO TabelaA VALUES (2, NULL, NULL, 3, 5.26);
INSERT INTO TabelaA VALUES (3, 'tekst', NULL, 1, NULL);
INSERT INTO TabelaA VALUES (4, NULL, NULL, NULL, NULL);
INSERT INTO TabelaA VALUES (5, 'tekst', NULL, 16, 6.78);

--SELECT * FROM TabelaA;

DROP TABLE TabelaB;
CREATE TABLE TabelaB (
	id NUMBER PRIMARY KEY,
	naziv VARCHAR2(5),
	datum DATE,
	cijelibroj NUMBER,
	realnibroj NUMBER,
	fktabelaA NUMBER,
	CONSTRAINT fk_tabela_A_iz_tabele_B FOREIGN KEY (fktabelaA) REFERENCES TabelaA(id),
	CONSTRAINT cijeli_broj_unikatan UNIQUE (cijelibroj)
)

INSERT INTO TabelaB VALUES (1, NULL, NULL, 1, NULL, 1);
INSERT INTO TabelaB VALUES (2, NULL, NULL, 3, NULL, 1);
INSERT INTO TabelaB VALUES (3, NULL, NULL, 6, NULL, 2);
INSERT INTO TabelaB VALUES (4, NULL, NULL, 11, NULL, 2);
INSERT INTO TabelaB VALUES (5, NULL, NULL, 22, NULL, 3);

--SELECT * FROM TabelaB;

DROP TABLE TabelaC;
CREATE TABLE TabelaC (
	id NUMBER PRIMARY KEY,
	naziv VARCHAR2(5) NOT NULL,
	datum DATE,
	cijelibroj NUMBER NOT NULL,
	realnibroj NUMBER,
	fktabelaB NUMBER,
	CONSTRAINT FkCnst FOREIGN KEY (fktabelaB) REFERENCES TabelaB(id)
)	

INSERT INTO TabelaC VALUES (1, 'YES', NULL, 33, NULL, 4);
INSERT INTO TabelaC VALUES (2, 'NO', NULL, 33, NULL, 2);
INSERT INTO TabelaC VALUES (3, 'NO', NULL, 55, NULL, 1);

--SELECT * FROM TabelaC;

--1. OK, ne krsi ogranicenja
INSERT INTO TabelaA (id, naziv, datum, cijeliBroj, realniBroj) VALUES (6, 'tekst', null, null,6.20);
--2. GRESKA: cijeli broj nije jedinstven
INSERT INTO TabelaB (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaA) VALUES (6, null,null, 1, null, 1);
--3. OK, ne krsi ogranicenja
INSERT INTO TabelaB (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaA) VALUES (7, null,null, 123, null, 6);
--4. OK, ne krsi ogranicenja
INSERT INTO TabelaC (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaB) VALUES (4, 'NO',null, 55, null, null);
--5. OK, ne krsi ogranicenja
UPDATE TabelaA SET naziv = 'tekst' WHERE naziv IS NULL AND cijeliBroj IS NOT NULL;
--6. GRESKA: id-evi ove tabele su referencirani od strane TabelaC
DROP TABLE TabelaB;
--7. GRESKA: id-evi ove tabele su referencirani od strane TabelaB (constraint: FK_TABELA_A_IZ_TABELE_B)
DELETE FROM TabelaA WHERE realniBroj IS NULL;
--8. OK, ne ovisi niko od ovog reda
DELETE FROM TabelaA WHERE id = 5;
--9. OK, postoje kljucevi
UPDATE TabelaB SET fktabelaA = 4 WHERE fktabelaA = 2;
--10. OK, svi vec postuju ovo ogranicenje
ALTER TABLE TabelaA ADD CONSTRAINT cst CHECK (naziv LIKE 'tekst'); 

SELECT SUM(id) FROM TabelaA; --Rezultat: 16
SELECT SUM(id) FROM TabelaB; --Rezultat: 22
SELECT SUM(id) FROM TabelaC; --Rezultat: 10
-- SVE OK


--ZADATAK 3
CREATE OR REPLACE TRIGGER t1
AFTER INSERT ON TabelaB
FOR EACH ROW
BEGIN
    IF :NEW.cijelibroj < 50 THEN
        UPDATE TabelaA
        SET realnibroj = realnibroj * 1.25
        WHERE id = :NEW.fktabelaA;
    ELSE
        UPDATE TabelaA
        SET realnibroj = realnibroj * 0.75
        WHERE id = :NEW.fktabelaA;
    END IF;
END;
/

DROP SEQUENCE brojacB;
CREATE SEQUENCE brojacB 
START WITH 6
INCREMENT BY 1;

CREATE OR REPLACE TRIGGER t2
AFTER INSERT OR UPDATE ON TabelaC
FOR EACH ROW
BEGIN
    INSERT INTO TabelaB (
        id, naziv, datum, cijelibroj, realnibroj, fktabelaA
    )
    VALUES (
        brojacB.NEXTVAL,
        NULL,
        SYSDATE,
        2 * (SELECT MAX(cijelibroj) FROM TabelaB),
        NULL,
        (SELECT fktabelaA FROM TabelaB WHERE id = :NEW.fktabelaB)
    );
END;
/

DROP TABLE TabelaABekap;

CREATE TABLE TabelaABekap (
	id NUMBER PRIMARY KEY,
	naziv VARCHAR2(5),
	datum DATE,
	cijelibroj NUMBER,
	realnibroj NUMBER,
	cijeliBrojB INTEGER,
	sekvenca INTEGER,
	CONSTRAINT bekapA_realni_veci_od_5 CHECK (realnibroj>5),
	CONSTRAINT bekapA_cijeli_izmedju_5_i_15 CHECK (cijelibroj NOT BETWEEN 5 AND 15)
);

DROP SEQUENCE seq;
CREATE SEQUENCE seq
START WITH 1
INCREMENT BY 1
MINVALUE 1
MAXVALUE 10
CYCLE
CACHE 5;

CREATE OR REPLACE TRIGGER t3
AFTER INSERT ON TabelaB
FOR EACH ROW
DECLARE 
    broj NUMBER;
BEGIN
    SELECT Count(*) INTO broj FROM TabelaABekap WHERE id = :NEW.fktabelaA;

    IF broj != 0 THEN
        UPDATE TabelaABekap
        SET cijeliBrojB = cijeliBrojB + :NEW.cijelibroj
        WHERE id = :NEW.fktabelaA;
    ELSE
        INSERT INTO TabelaABekap (
            id, naziv, datum, cijelibroj, realnibroj, cijeliBrojB, sekvenca
        )
        SELECT 
            a.id, 
            a.naziv, 
            a.datum, 
            a.cijelibroj, 
            a.realnibroj, 
            :NEW.cijelibroj, 
            seq.NEXTVAL
        FROM 
            TabelaA a
        WHERE 
            a.id = :NEW.fktabelaA;
    END IF;
END;
/

INSERT INTO TabelaB (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaA) VALUES (brojacB.nextval, null,null, 2, null, 1);
INSERT INTO TabelaB (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaA) VALUES (brojacB.nextval, null,null, 4, null, 2);
INSERT INTO TabelaB (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaA) VALUES (brojacB.nextval, null,null, 8, null, 1);
INSERT INTO TabelaC (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaB) VALUES (4, 'NO', null, 5, null, 3);
INSERT INTO TabelaC (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaB) VALUES (5, 'YES', null, 7, null,3);
INSERT INTO TabelaC (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaB) VALUES (6, 'NO', null, 9, null, 2);
UPDATE TabelaC SET cijeliBroj = 10 WHERE id = 2;
DELETE FROM TabelaB WHERE id NOT IN (SELECT FkTabelaB FROM TabelaC);
DELETE FROM TabelaA WHERE id IN (3, 4, 6);

SELECT SUM(id*3 + cijeliBrojB*3) FROM TabelaABekap; --Rezultat: 2031
SELECT SUM(id*3 + cijeliBroj*3) FROM TabelaC; --Rezultat: 420
SELECT SUM(MOD(id,10)*3) FROM TabelaB; --Rezultat: 30
SELECT SUM(id + realniBroj)*10 FROM TabelaA; --Rezultat: 264
-- SVE OK

SELECT SUM(id*7 + cijeliBrojB*7) FROM TabelaABekap; --4739
SELECT SUM(id*7 + cijeliBroj*7) FROM TabelaC; --980
SELECT SUM(MOD(id,10)*7) FROM TabelaB; --70
SELECT SUM(id*5 + realniBroj)*10 FROM TabelaA; --584







