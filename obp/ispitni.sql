CREATE TABLE parkiranje (
	id NUMBER PRIMARY KEY,
	datum DATE DEFAULT sysdate,
	trajanje NUMBER,
	t_fk NUMBER REFERENCES tarifa(id),
	a_fk NUMBER REFERENCES auta(id)
)

CREATE TABLE auta (
	id NUMBER PRIMARY KEY,
	registracija varchar2(7),
	boja varchar(32)
)

CREATE TABLE tarifa (
	id NUMBER PRIMARY KEY,
	cijena NUMBER,
	u_fk NUMBER REFERENCES ugovor(id)
)

CREATE TABLE ugovor (
	id NUMBER PRIMARY KEY,
	bonus NUMBER,
	u_fk NUMBER REFERENCES ugovor(id)
) 

SELECT * FROM ugovor

INSERT INTO ugovor VALUES (1,2,NULL);
INSERT INTO ugovor VALUES (2,1,NULL);
INSERT INTO ugovor VALUES (3,4,2);
INSERT INTO ugovor VALUES (4,7,3);

SELECT * FROM auta;

INSERT INTO auta VALUES (1,'786A678','Crvena');
INSERT INTO auta VALUES (2,'133B565','Plava');
INSERT INTO auta VALUES (3,'123C678','plava');
INSERT INTO auta VALUES (4,'978D768',NULL);
INSERT INTO auta VALUES (5,'157E890','zelena');
INSERT INTO auta VALUES (6,'056F873','Crvena');
INSERT INTO auta VALUES (7,'283G657','Plava');

SELECT * FROM parkiranje;

INSERT INTO Parkiranje (id, datum, trajanje, t_fk, a_fk) VALUES (1, TO_DATE('21.11.2008', 'DD.MM.YYYY'), 5, 1, 1);
INSERT INTO Parkiranje (id, datum, trajanje, t_fk, a_fk) VALUES (2, TO_DATE('26.12.2008', 'DD.MM.YYYY'), 2, 1, 2);
INSERT INTO Parkiranje (id, datum, trajanje, t_fk, a_fk) VALUES (3, TO_DATE('28.03.2009', 'DD.MM.YYYY'), 3, 2, 3);
INSERT INTO Parkiranje (id, datum, trajanje, t_fk, a_fk) VALUES (4, TO_DATE('14.10.2010', 'DD.MM.YYYY'), 1, 3, 2);
INSERT INTO Parkiranje (id, datum, trajanje, t_fk, a_fk) VALUES (5, TO_DATE('18.08.2011', 'DD.MM.YYYY'), 4, 1, 4);
INSERT INTO Parkiranje (id, datum, trajanje, t_fk, a_fk) VALUES (6, TO_DATE('19.05.2012', 'DD.MM.YYYY'), 2, 1, 6);
INSERT INTO Parkiranje (id, datum, trajanje, t_fk, a_fk) VALUES (7, TO_DATE('26.02.2013', 'DD.MM.YYYY'), 3, 2, 7);
INSERT INTO Parkiranje (id, datum, trajanje, t_fk, a_fk) VALUES (8, TO_DATE('27.03.2014', 'DD.MM.YYYY'), 3, 4, 2);
INSERT INTO Parkiranje (id, datum, trajanje, t_fk, a_fk) VALUES (9, TO_DATE('19.12.2015', 'DD.MM.YYYY'), 2, 4, 5);
INSERT INTO Parkiranje (id, datum, trajanje, t_fk, a_fk) VALUES (10, TO_DATE('15.11.2016', 'DD.MM.YYYY'), 6, 2, 1);

SELECT * FROM tarifa

INSERT INTO Tarifa (id, cijena, u_fk) VALUES (1, 1.5, 1);
INSERT INTO Tarifa (id, cijena, u_fk) VALUES (2, 3.5, 1);
INSERT INTO Tarifa (id, cijena, u_fk) VALUES (3, 6.5, 1);
INSERT INTO Tarifa (id, cijena, u_fk) VALUES (4, 6.5, 2);
INSERT INTO Tarifa (id, cijena, u_fk) VALUES (5, 7.5, 2);

SELECT (SELECT max(id) FROM parkiranje p),park.id
FROM (SELECT p.id AS id
FROM auta a, parkiranje p
WHERE p.a_fk = a.id AND a.boja = 'Crvena') park
GROUP BY park.id
ORDER BY park.id;

UPDATE parkiranje SET datum = to_date('21.11.2008', 'dd.mm.yyyy'),trajanje=5,t_fk=1,a_fk=1 WHERE id = 1 

SELECT substr(to_char(datum, 'yyyy'), 3) FROM parkiranje

SELECT DISTINCT a.registracija
FROM auta a, parkiranje p
WHERE p.a_fk = a.id AND
(Substr(a.registracija, 2, 1), to_number(Substr(a.registracija, 7)) ) IN
(SELECT Substr(To_char(datum, 'yyyy'), 4), trajanje + 3
FROM parkiranje);

SELECT Substr(To_char(datum, 'yyyy'), 4) a, trajanje + 3 b
FROM parkiranje;

SELECT * FROM auta ORDER BY id

DELETE FROM auta WHERE id =1;
DELETE FROM parkiranje WHERE id=10;

SELECT Substr(a.registracija, 2, 1), Substr(a.registracija, 7) FROM auta a

SELECT * FROM parkiranje ORDER BY id

SELECT p.id
FROM parkiranje p, tarifa t
WHERE p.t_fk = t.id
AND t.cijena > ANY (SELECT u.bonus
FROM ugovor u
WHERE u.id = t.u_fk)
ORDER BY p.id;


SELECT *
FROM ugovor u, ugovor nu, tarifa t
WHERE u.u_fk = nu.id
AND t.u_fk = nu.id

CREATE TABLE koristenje (
	id NUMBER PRIMARY KEY,
	datum DATE DEFAULT sysdate,
	brojgodina NUMBER,
	s_fk NUMBER REFERENCES sef(id),
	k_fk NUMBER REFERENCES korisnik(id)
);

CREATE TABLE sef (
	id NUMBER PRIMARY KEY,
	sifra varchar2(6),
	tip varchar2(30)
)

CREATE TABLE pretplata (
	id NUMBER PRIMARY KEY,
	cijena NUMBER,
	p_fk NUMBER REFERENCES pretplata(id)
)

CREATE TABLE korisnik (
	id NUMBER PRIMARY KEY,
	prioritet NUMBER,
	p_fk NUMBER REFERENCES pretplata(id)
)

SELECT * FROM sef

INSERT INTO sef VALUES (1, 'GTH773', 'Aluminij');
INSERT INTO sef VALUES (2, 'ATX823', 'Aluminij');
INSERT INTO sef VALUES (3, 'FTL256', 'Titanij');
INSERT INTO sef VALUES (4, 'GRO456', 'Adamantij');
INSERT INTO sef VALUES (5, 'TEL511', 'Kriptonit');
INSERT INTO sef VALUES (6, 'RAP734', 'Lego');
INSERT INTO sef VALUES (7, 'GTT744', 'titanij');

SELECT * FROM pretplata;

INSERT INTO pretplata VALUES (1,30,NULL);
INSERT INTO pretplata VALUES (2,50,NULL);
INSERT INTO pretplata VALUES (3,70,2);
INSERT INTO pretplata VALUES (4,20,2);

SELECT * FROM korisnik;

INSERT INTO korisnik VALUES (1,2,1);
INSERT INTO korisnik VALUES (2,2,2);
INSERT INTO korisnik VALUES (3,3,2);
INSERT INTO korisnik VALUES (4,5,4);
INSERT INTO korisnik VALUES (5,7,4);

SELECT * FROM koristenje;

INSERT INTO koristenje VALUES (1,to_date('21-nov-08', 'dd-mon-yy'),1,1,1);
INSERT INTO koristenje VALUES (2,to_date('23-dec-09', 'dd-mon-yy'),1,2,2);
INSERT INTO koristenje VALUES (3,to_date('19-may-10', 'dd-mon-yy'),1,3,2);
INSERT INTO koristenje VALUES (4,to_date('27-jun-11', 'dd-mon-yy'),2,4,3);
INSERT INTO koristenje VALUES (5,to_date('28-jul-12', 'dd-mon-yy'),2,5,3);
INSERT INTO koristenje VALUES (6,to_date('29-aug-13', 'dd-mon-yy'),3,6,1);
INSERT INTO koristenje VALUES (7,to_date('30-sep-14', 'dd-mon-yy'),3,7,3);
INSERT INTO koristenje VALUES (8,to_date('22-oct-15', 'dd-mon-yy'),4,1,4);
INSERT INTO koristenje VALUES (9,to_date('21-nov-16', 'dd-mon-yy'),5,2,5);
INSERT INTO koristenje VALUES (10,to_date('21-dec-17', 'dd-mon-yy'),5,3,1);

SELECT * FROM koristenje;

SELECT DISTINCT substr(s.sifra,3,3) AS O
FROM sef s, koristenje k
WHERE s.id=k.s_fk
AND k.brojgodina<s.id
ORDER BY O ASC;

SELECT p1.id AS id1,
	   p2.id AS id2,
	   p3.id AS id3
FROM pretplata p1 
INNER JOIN pretplata p2 ON p1.p_fk=p2.id
LEFT OUTER JOIN pretplata p3 ON p2.p_fk=p3.id
ORDER BY id1 ASC; 

SELECT * FROM pretplata;

SELECT count(brojgodina)
FROM koristenje
GROUP BY brojgodina
HAVING count(*) > 1
ORDER BY count(brojgodina);

SELECT SUM(k.prioritet), sum(p.cijena)
FROM korisnik k, pretplata p
WHERE p.id=k.p_fk
AND k.p_fk IS NULL;

SELECT sifra FROM sef WHERE sifra NOT LIKE 'T'
AND tip LIKE 't%'
ORDER BY sifra;

SELECT RPAD(p.id, p.id, 'A'), LPAD(k.id, k.id, 'B')
FROM korisnik k, pretplata p WHERE k.p_fk = p.p_fk

SELECT * FROM koristenje WHERE datum < to_date('01.01.2013', 'dd.mm.yyyy');

SELECT datum, decode(
	MOD(to_number(to_char(datum, 'yyyy')),2),
	1, ADD_MONTHS(datum, -1),
	0, ADD_MONTHS(datum, 1) 
) FROM koristenje
WHERE datum < to_date('01.01.2013', 'dd.mm.yyyy');

SELECT s.sifra, p.cijena 
FROM sef s, koristenje k, korisnik ko, pretplata p
WHERE k.S_FK = s.ID AND k.K_FK = ko.ID AND p.ID = ko.P_FK 

SELECT * FROM korisnik

SELECT substr(s.sifra, 4) || LENGTH(s.tip) FROM sef s
WHERE lower(substr(s.sifra, 2, 1)) = lower(substr(s.tip, 1, 1))

SELECT sum(k.brojgodina), avg(korisnik.prioritet)
FROM koristenje k, korisnik korisnik, sef sef
WHERE k.s_fk = sef.id AND k.k_fk = korisnik.id AND sef.tip LIKE ('_itanij') OR sef.tip LIKE ('Aluminij')
GROUP BY sef.tip
HAVING avg(korisnik.prioritet) > 1
ORDER BY avg(korisnik.prioritet) DESC

SELECT id + p_fk FROM PRETPLATA p





