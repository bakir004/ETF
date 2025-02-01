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
