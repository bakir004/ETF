CREATE TABLE odjeli(
	id varchar2(25) PRIMARY KEY NOT NULL,
	naziv varchar2(10) NOT NULL,
	opis char(15),
	datum DATE NOT NULL,
	korisnik varchar2(30) NOT NULL,
	napomena varchar2(10)
)

INSERT INTO odjeli (id, naziv, opis, datum, korisnik, napomena)
SELECT d.department_id, SUBSTR(d.department_name, 1, 10), NULL, sysdate, 
       e.first_name || ' ' || e.last_name, NULL
FROM employees e, departments d
WHERE d.manager_id = e.employee_id;

SELECT * FROM odjeli;

ALTER TABLE odjeli ADD (location_id NUMBER)

SELECT * FROM departments;

CREATE TABLE zaposleni (
    Id NUMBER(4) NOT NULL,
    Sifra_zaposlenog VARCHAR2(5) NOT NULL,
    Naziv_zaposlenog CHAR(50),
    Godina_zaposlenja NUMBER(4),
    Mjesec_zaposlenja CHAR(2) NOT NULL,
    Sifra_odjela VARCHAR2(5) NOT NULL,
    Naziv_odjela VARCHAR2(15),
    Grad CHAR(10) NOT NULL,
    Sifra_posla VARCHAR2(25) NOT NULL,
    Naziv_posla CHAR(30),
    Iznos_dodatak_na_platu NUMBER(5),
    Plata NUMBER(6) NOT NULL,
    Kontinent VARCHAR2(20) NOT NULL,
    Datum_unosa DATE NOT NULL,
    Korisnik_unio CHAR(20) NOT NULL,
    PRIMARY KEY (Id)
);

DROP TABLE zaposleni

SELECT * FROM employees;
SELECT * FROM locations;
SELECT * FROM jobs;

INSERT INTO zaposleni (id, Sifra_zaposlenog, Naziv_zaposlenog, Godina_zaposlenja, Mjesec_zaposlenja, 
Sifra_odjela, Naziv_odjela, Grad, Sifra_posla, Naziv_posla, Iznos_dodatak_na_platu, Plata,
Kontinent, Datum_unosa, Korisnik_unio)
SELECT e.employee_id, e.employee_id, e.first_name || ' ' || e.last_name, to_char(e.hire_date, 'yyyy'), to_char(e.hire_date, 'mm'), e.department_id,
	substr(d.department_name, 1, 15), substr(l.city, 1, 10), j.job_id, substr(j.job_title, 1, 30), e.commission_pct * e.salary, e.salary, r.region_name, sysdate, 'IDK'
	FROM employees e, departments d, jobs j, locations l, countries c, regions r
	WHERE e.department_id = d.department_id
	AND j.job_id = e.job_id
	AND d.location_id = l.location_id
	AND c.country_id = l.country_id
	AND r.region_id = c.region_id
	
SELECT * FROM zaposleni;

CREATE TABLE zaposleni2 AS SELECT * FROM zaposleni

RENAME zaposleni2 TO zap_backup

COMMENT ON COLUMN zaposleni.Grad IS 'grad';

ALTER TABLE zaposleni SET unused (korisnik_unio)

ALTER TABLE zaposleni DROP unused columns;