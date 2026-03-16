DROP TABLE zaposleni;

CREATE TABLE zaposleni
  (sifra_zaposlenog NUMBER,
   naziv_zaposlenog VARCHAR2(35),
   naziv_odjela VARCHAR2(20),
   sifra_posla NUMBER,
   naziv_posla VARCHAR2(20),
   plata NUMBER,
   dodatak NUMBER,
   datum_zaposlenja DATE);

ALTER TABLE zaposleni
ADD (sifra NUMBER,
     CONSTRAINT sifra_zap_pk PRIMARY KEY (sifra));

CREATE OR REPLACE TRIGGER zaposleni_trigger
BEFORE UPDATE OR DELETE
ON zaposleni
FOR EACH ROW
BEGIN
  IF (To_Number(To_Char(sysdate, 'd')) BETWEEN 2 AND 6 
  AND to_number(to_char(sysdate, 'hh')) * 100 + to_number(to_char(sysdate, 'mm')) BETWEEN 1623 AND 2316)
  OR (to_number(to_char(sysdate, 'd')) NOT BETWEEN 2 AND 6
  AND to_number(to_char(sysdate, 'hh')) * 100 + to_number(to_char(sysdate, 'mm')) BETWEEN 634 AND 2356)
  THEN
    Raise_Application_Error(-20000, 'Nemoguce mijenjati podatke za ovog zaposlenika!');
  END IF; 
END;

CREATE TABLE arhiva
AS
SELECT *
FROM zaposleni;

ALTER TABLE arhiva
ADD (korisnik VARCHAR2(20),
     datum DATE);

CREATE OR REPLACE TRIGGER zaposleni_arhiva
BEFORE UPDATE OR DELETE
ON zaposleni
FOR EACH ROW
BEGIN
  INSERT INTO arhiva
  VALUES (:old.sifra_zaposlenog, :old.naziv_zaposlenog, :old.naziv_odjela, :old.sifra_posla, :old.naziv_posla, :old.plata, :old.dodatak, :old.datum_zaposlenja, :old.sifra, USER, SYSDATE);
END;


DROP TABLE odjeli;
DROP TABLE poslovi;

CREATE TABLE odjeli
AS
SELECT *
FROM departments;

CREATE TABLE poslovi
AS
SELECT *
FROM jobs;

CREATE SEQUENCE id_odjeli
MINVALUE 1
START WITH 1
INCREMENT BY 1
CACHE 20;

CREATE SEQUENCE id_poslovi
MINVALUE 1
START WITH 1
INCREMENT BY 1
CACHE 20;

ALTER TABLE odjeli
ADD (id NUMBER DEFAULT id_odjeli.NEXTVAL,
     korisnik VARCHAR2(20) DEFAULT '%%%%',
     datum DATE DEFAULT To_Date('01012000', 'DDMMYYYY'),
     CONSTRAINT id_odjeli_pk PRIMARY KEY (id));

ALTER TABLE poslovi
ADD (id NUMBER DEFAULT id_poslovi.NEXTVAL,
     korisnik VARCHAR2(20) DEFAULT '%%%%',
     datum DATE DEFAULT To_Date('01012000', 'DDMMYYYY'),
     CONSTRAINT id_poslovi_pk PRIMARY KEY (id));

ALTER TABLE zaposleni
ADD (id_odjela NUMBER,
     id_posla NUMBER,
     CONSTRAINT odjel_zap_fk FOREIGN KEY (id_odjela)
     REFERENCES odjeli (id),
     CONSTRAINT posao_zap_fk FOREIGN KEY (id_posla)
     REFERENCES poslovi (id));

CREATE OR REPLACE TRIGGER odjeli_dodjele
BEFORE INSERT
ON odjeli
FOR EACH ROW
BEGIN
  :new.korisnik := USER;
  :new.datum := SYSDATE;
END;

CREATE OR REPLACE TRIGGER poslovi_dodjele
BEFORE INSERT
ON poslovi
FOR EACH ROW
BEGIN
  :new.korisnik := USER;
  :new.datum := SYSDATE;
END;

CREATE OR REPLACE TRIGGER odjeli_dodjele
BEFORE INSERT
ON odjeli
FOR EACH ROW
DECLARE
    manager NUMBER;
BEGIN
    :new.korisnik := USER;
    :new.datum := SYSDATE;
    SELECT e.employee_id
    INTO manager
    FROM employees e 
    WHERE e.employee_id = (SELECT e2.employee_id
                        FROM employees e2 
                        WHERE e2.employee_id IN (SELECT DISTINCT manager_id
                                                FROM employees)
                        AND (SELECT Count(*)
                             FROM employees e3
                             WHERE e3.manager_id = e2.employee_id) = (SELECT Max(broj)
                                                                      FROM (SELECT Count(*) AS broj, manager_id
                                                                            FROM employees
                                                                            GROUP BY manager_id) zaposlenici_po_sefovima));
    :new.manager_id := manager;
END;


CREATE OR REPLACE TRIGGER zaposleni_trigger
BEFORE INSERT
ON zaposleni
FOR EACH ROW
DECLARE
    odjel NUMBER;
BEGIN
    SELECT d.department_id
    INTO odjel
    FROM departments d, employees e
    WHERE d.manager_id = e.employee_id AND e.employee_id IN (SELECT e2.employee_id
                                                           FROM employees e2, (SELECT manager_id, Count(department_id) AS broj
                                                                               FROM departments
                                                                               GROUP BY manager_id) nadlezni
                                                           WHERE nadlezni.broj >= 2 AND e2.employee_id = nadlezni.manager_id);
    
    :new.id_odjela := odjel;
END;


CREATE SEQUENCE id_arhiva
MINVALUE 1
START WITH 1
INCREMENT BY 1
CACHE 20;

CREATE TABLE arhiva_brisanja
AS
SELECT *
FROM zaposleni;

ALTER TABLE arhiva_brisanja
ADD (id NUMBER,
     CONSTRAINT id_arh_pk PRIMARY KEY (id));

CREATE OR REPLACE TRIGGER zaposleni_brisanje
BEFORE DELETE
ON zaposleni
FOR EACH ROW
DECLARE
    odjel NUMBER;
BEGIN
    INSERT INTO arhiva_brisanja
    VALUES
    (:old.sifra_zaposlenog, :old.naziv_zaposlenog, :old.naziv_odjela, :old.sifra_posla, :old.naziv_posla, :old.plata, :old.dodatak, :old.datum_zaposlenja, :old.sifra, id_arhiva.NEXTVAL);
END;


CREATE OR REPLACE TRIGGER zaposleni_plata
BEFORE INSERT OR UPDATE
ON zaposleni
FOR EACH ROW
DECLARE
    iznos_plate NUMBER;
    minimum NUMBER;
    maksimum NUMBER;
    smanjivanje NUMBER;
    povecavanje NUMBER;
BEGIN
    minimum := 1000;
    maksimum := 10000;
    smanjivanje := 0.2;
    povecavanje := 0.2;
    iznos_plate := :new.plata * (1 + :new.dodatak);
    WHILE iznos_plate > maksimum AND smanjivanje <= 1
    LOOP
        :new.plata := (1 - smanjivanje) * :new.plata;
        smanjivanje := smanjivanje + 0.1;
    END LOOP;
    WHILE iznos_plate < minimum AND povecavanje <= 1
    LOOP
        :new.plata := (1 + povecavanje) * :new.plata;
        povecavanje := povecavanje + 0.1;
    END LOOP;
END;



CREATE TABLE logovi_odjeli
    (korisnik VARCHAR2(20),
     datum DATE,
     odjel NUMBER,
     stara_lokacija VARCHAR2(30),
     nova_lokacija VARCHAR2(30));

CREATE OR REPLACE TRIGGER odjeli_drzava
BEFORE UPDATE
ON odjeli
FOR EACH ROW
DECLARE
    stara_drzava VARCHAR2(30);
    nova_drzava VARCHAR2(30);
    stara_lokacija NUMBER;
    nova_lokacija NUMBER;
BEGIN
    stara_lokacija := :old.location_id;
    nova_lokacija := :new.location_id;
    SELECT c.country_name
    INTO stara_drzava
    FROM locations l, countries c
    WHERE l.location_id = stara_lokacija AND l.country_id = c.country_id;
    SELECT c.country_name
    INTO nova_drzava
    FROM locations l, countries c
    WHERE l.location_id = nova_lokacija AND l.country_id = c.country_id;
    IF stara_lokacija = 'United States of America' AND nova_lokacija = 'United Kingdom' THEN
        INSERT INTO logovi_odjeli
        VALUES
        (USER, SYSDATE, :new.department_id, stara_lokacija, nova_lokacija);
        Raise_Application_Error(-20003, 'Zabranjen izmjena iz SAD u UK!');
    END IF;
END;

