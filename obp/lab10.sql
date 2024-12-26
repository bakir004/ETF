CREATE TABLE zaposleni AS SELECT * FROM employees;

ALTER TABLE zaposleni ADD id NUMBER;

SELECT * FROM zaposleni;

UPDATE zaposleni SET id = employee_id WHERE 1=1;

ALTER TABLE zaposleni ADD CONSTRAINT id_primary_key PRIMARY KEY (id);

DROP TABLE odjeli;

CREATE TABLE odjeli AS SELECT * FROM departments;

ALTER TABLE odjeli ADD id NUMBER;

UPDATE odjeli SET id = department_id WHERE 1=1;

ALTER TABLE odjeli ADD CONSTRAINT odjeli_id_primary_key PRIMARY KEY(id);

ALTER TABLE odjeli DROP CONSTRAINT odjeli_id_primary_key;

ALTER TABLE odjeli ADD datum DATE;
UPDATE odjeli SET datum = sysdate WHERE 1=1;
ALTER TABLE odjeli ADD CONSTRAINT odjeli_datum_i_id_primary_key PRIMARY KEY(id, datum);

ALTER TABLE zaposleni ADD CONSTRAINT zaposleni_FK_za_odjele FOREIGN KEY(department_id) REFERENCES odjeli (id);
SELECT * FROM zaposleni;

ALTER TABLE odjeli DROP CONSTRAINT odjeli_datum_i_id_primary_key;

ALTER TABLE odjeli ADD CONSTRAINT odjeli_id_primary_key PRIMARY KEY (id);

SELECT * FROM zaposleni LEFT JOIN odjeli ON zaposleni.department_id = odjeli.id;

ALTER SESSION SET current_schema = bc19705;

SELECT * FROM all_constraints WHERE owner = 'BC19705';

ALTER SESSION SET current_schema = hr;

SELECT * FROM all_objects WHERE owner = 'HR';
SELECT * FROM all_constraints WHERE owner = 'HR' AND r_constraint_name IS NOT NULL AND table_name = 'EMPLOYEES';

SELECT * FROM countries;

ALTER TABLE ZAPOSLENI ADD plata_dodatak NUMBER;
UPDATE zaposleni SET plata_dodatak = salary * (1+commission_pct) WHERE 1=1;

SELECT * FROM zaposleni;

ALTER TABLE zaposleni ADD CONSTRAINT plata_dodatak_nije_prevelika CHECK (plata_dodatak BETWEEN salary AND 100000);

UPDATE zaposleni SET salary = 50000 WHERE id = 174;

-- Kreirajte pogled sa nazivom zap_pog sa sljedećim kolonama: šifra zaposlenog,
-- naziv zaposlenog i naziv odjela, za sve zaposlene koji primaju platu veću od
-- prosječne plate odjela u kojem rade.

CREATE VIEW zap_pog (emp_id, emp_name, dept_name) 
AS SELECT e.employee_id, e.first_name || ' ' || e.last_name, d.department_name
FROM employees e, departments d WHERE e.department_id = d.department_id;

SELECT * FROM zap_pog, departments;

CREATE VIEW pogled3 (job, dept, avg_salary, commission)
AS SELECT j.job_title, d.department_name, avg(e.salary), avg(e.commission_pct)
FROM employees e, departments d, jobs j
WHERE e.department_id = d.department_id
AND e.job_id = j.job_id
AND (d.department_name LIKE '%a%' OR d.department_name LIKE '%b%' OR d.department_name LIKE '%c%')
GROUP BY j.job_title, d.department_name

DROP VIEW pogled3;

SELECT * FROM pogled3;





















