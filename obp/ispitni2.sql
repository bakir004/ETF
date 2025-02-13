SELECT first_name, manager_id, LEVEL
FROM employees START WITH manager_id > 101
CONNECT BY PRIOR employee_id = manager_id
ORDER BY LEVEL


CREATE TABLE tablica (
	id NUMBER PRIMARY KEY,
	naziv VARCHAR2(50),
	datum DATE,
	cbr NUMBER,
	rbr NUMBER,
	fx_t NUMBER REFERENCES tablica(id)
);

SELECT * FROM tablica;

INSERT INTO tablica VALUES (1, 'Boracko jezero', to_date('21-NOV-2009', 'dd-MM-yyyy'), 1, 2.5, NULL);
INSERT INTO tablica VALUES (2, 'Jablanicko jezero', to_date('25-DEC-2010', 'dd-MM-yyyy'), 1, 3.5, 1);
INSERT INTO tablica VALUES (3, 'Mostarsko jezero', to_date('17-MAY-2011', 'dd-MM-yyyy'), 1, 1.5, 1);
INSERT INTO tablica VALUES (4, 'Busko jezero', to_date('03-MAR-2014', 'dd-MM-yyyy'), 1, 6.6, 2);
INSERT INTO tablica VALUES (5, 'Malo plivsko jezero', to_date('27-AUG-2017', 'dd-MM-yyyy'), 1, 6.7, 3);
INSERT INTO tablica VALUES (6, 'Ramsko jezero', to_date('28-JUL-2018', 'dd-MM-yyyy'), 1, 4.4, 4);
INSERT INTO tablica VALUES (7, 'Panonsko jezero', to_date('29-OCT-2019', 'dd-MM-yyyy'), 1, 3.3, 2);
INSERT INTO tablica VALUES (8, 'Satorsko jezero', to_date('28-AUG-2021', 'dd-MM-yyyy'), 1, 6.7, 3);
INSERT INTO tablica VALUES (9, 'Salakovacko jezero', to_date('19-APR-2022', 'dd-MM-yyyy'), 1, 2.2, 2);
INSERT INTO tablica VALUES (10, 'Panonsko jezero', to_date('30-JAN-2023', 'dd-MM-yyyy'), 1, 7.6, 5);

SELECT id, fx_t
FROM tablica
START WITH mod(id, fx_t) > MOD(InStr(naziv, 'jez'),fx_t)
CONNECT BY PRIOR fx_t = id
ORDER BY id

CREATE VIEW dvogled AS
SELECT l.naziv, l.adresa, l.drzava, p.pid, pd.pdv, sum(sr.kolicina), sum(sr.kolicina*sr.cijena)
FROM proizvodi p, lice l, pdv pd, stavke_racuna sr, racun r
WHERE p.firmaProizvodjac = l.lid
AND pd.id = p.pdv
AND sr.proizvod = p.pid
AND sr.racun = r.rid
AND r.skladiste = ANY (
	SELECT * FROM (
		SELECT r.skladiste
		FROM racun r
		WHERE r.rid NOT IN (SELECT racun FROM arhiva)
		GROUP BY r.skladiste
		ORDER BY count(*) DESC 
	) WHERE ROWNUM <= 5
) GROUP BY p.pid

SELECT * FROM (
    SELECT count(*), manager_id
    FROM employees
    GROUP BY manager_id
    ORDER BY count(*) DESC
)
WHERE ROWNUM <= 5;

SELECT * FROM v$version;



CREATE VIEW wcomp AS
SELECT s.naziv, o.naziv, a.naziv, a.tipArtikla, st.kolicina, st.kolicina * st.cijena
FROM izlazi i, artikli a, osobe o, stavke st, skladista s
WHERE st.izlaz = i.zid
AND st.artikal = a.mid,
AND s.odgovornoLice = o.id
AND i.skladiste = s.sid
GROUP BY ROLLUP(s.naziv, a.naziv)

CREATE TABLE employeesc AS SELECT * FROM employees WHERE 1=0;
SELECT * FROM employeesc;

DROP TABLE employeesc;

CREATE TABLE employeesc(
	id NUMBER PRIMARY KEY,
	emp_name VARCHAR2(30),
	job_title VARCHAR2(30),
	salary NUMBER
);

CREATE SEQUENCE empId
START WITH 1
INCREMENT BY 1
MAXVALUE 1000
NOCYCLE
CACHE 10;

CREATE OR REPLACE PROCEDURE ADD_EMPLOYEE(
    p_emp_name  IN VARCHAR2,
    p_job_title IN VARCHAR2,
    p_salary    IN NUMBER
)
IS
BEGIN
    INSERT INTO employeesc (id, emp_name, job_title, salary)
    VALUES (empId.nextVal, p_emp_name, p_job_title, p_salary);

    COMMIT;
END;
/

SELECT * FROM employeesc;

BEGIN
    ADD_EMPLOYEE('Alice Smith', 'Data Analyst', 85000);
END;
/

SELECT OBJECT_NAME, STATUS 
FROM USER_OBJECTS 
WHERE OBJECT_TYPE = 'PROCEDURE' AND OBJECT_NAME = 'ADD_EMPLOYEE';

DROP PROCEDURE add_employee





CREATE OR REPLACE FUNCTION update_employees(
    p_job_title IN VARCHAR2,  -- Job title to filter employees
    p_salary IN NUMBER         -- New salary to update
) RETURN NUMBER -- Return the number of rows affected
IS
    v_updated_count NUMBER;  -- Variable to store the count of updated rows
BEGIN
    -- Perform a bulk update for all employees with the specified job title
    UPDATE employeesc
    SET salary = p_salary
    WHERE job_title = p_job_title;

    -- Get the number of rows affected by the update
    v_updated_count := SQL%ROWCOUNT;

    -- Commit the changes to the database
    COMMIT;

    -- Return the number of rows affected
    RETURN v_updated_count;
EXCEPTION
    WHEN OTHERS THEN
        -- Handle any exceptions by rolling back the transaction and raising an error
        ROLLBACK;
        RAISE;  -- Re-raise the exception to propagate it
END update_employees;
/

SELECT * FROM USER_ERRORS WHERE NAME = 'UPDATE_EMPLOYEES';

DROP FUNCTION update_employees;

DECLARE
    v_rows_updated NUMBER;
BEGIN
    -- Call the function to update salaries for a specific job title
    v_rows_updated := update_employees('Data Analyst', 75000);

    -- Output the number of rows updated
    DBMS_OUTPUT.PUT_LINE('Rows updated: ' || v_rows_updated);
END;
/












SELECT 
(SELECT naziv FROM skladista2 s2 WHERE s2.sid=sk.sid ) skladistenaziv,
(SELECT ODGOVORNOLICE FROM SKLADISTa2 s2 WHERE s2.sid=sk.sid) odgovornoLice,
(SELECT naziv FROM  artikli2 a2 WHERE a2.mid=a.mid) artikalnaziv,
Sum(s.kolicina) kolicina, Sum(s.kolicina*s.cijena) iznos
FROM  IZLAZI2 i, Stavke2 s, Artikli2 a, Skladista2 sk
WHERE i.zid=s.izlaz AND s.artikal=a.mid  AND sk.sid = i.skladiste
GROUP BY rollup(sk.sid,a.mid)
UNION
SELECT
NULL,NULL,
(SELECT naziv FROM  artikli2 a2 WHERE a2.mid=a.mid) artikalnaziv,
Sum(s.kolicina) kolicina, Sum(s.kolicina*s.cijena) iznos
FROM  IZLAZI2 i, Stavke2 s, Artikli2 a
WHERE i.zid=s.izlaz AND s.artikal=a.mid

GROUP BY a.mid







































