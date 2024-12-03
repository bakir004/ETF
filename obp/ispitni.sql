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

CREATE TABLE 








