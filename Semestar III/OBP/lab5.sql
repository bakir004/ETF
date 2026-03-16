--1
SELECT USER, sysdate AS "DATE"
FROM dual

--2
SELECT employee_id, first_name, last_name, salary, round(salary * 1.25) AS "Plata uvecana za 25%" FROM employees e

--3
SELECT employee_id, first_name, last_name, salary, MOD(round(salary * 1.25), 100) AS "Ostatak plate" FROM employees e

--4
SELECT first_name, last_name, hire_date, to_char(NEXT_DAY(ADD_MONTHS(hire_date, 6), 'MONDAY'), 'DAY, dd - MON, yyyy') FROM employees e

--5
SELECT e.first_name, d.department_name, r.region_name, FLOOR(MONTHS_BETWEEN(sysdate,e.hire_date)) AS "Broj mjeseci zaposlenja" FROM employees e, departments d, locations l, countries c, regions r
WHERE e.department_id = d.department_id
AND d.location_id = l.location_id
AND l.country_id = c.country_id
AND c.region_id = r.region_id

--6
SELECT concat(concat(concat(concat(e.first_name, ' prima platu '), e.salary), ', ali on bi zelio platu '), e.salary*(1+nvl(e.commission_pct, 0))*4.5) 
AS "Plata o kojoj mozes sanjat"
FROM employees e, departments d
WHERE e.department_id = d.department_id 
AND e.department_id IN (10, 30, 50)

--7
SELECT lpad(first_name || ' + ' || salary, 50, '$') FROM employees

--8
SELECT concat(lower(substr(first_name, 1, 1)), substr(upper(concat(concat(first_name, ' '), last_name)), 2, 999)) AS full_name, LENGTH(concat(concat(first_name, ' '), last_name))-1 AS "Duzina"
FROM employees
WHERE first_name LIKE 'A%'
OR first_name LIKE 'J%'
OR first_name LIKE 'M%'
OR first_name LIKE 'S%'

--9
SELECT first_name, hire_date, to_char(hire_date, 'DAY') 
FROM employees
ORDER BY 
CASE to_number(to_char(hire_date, 'D'))
	WHEN 2 THEN 1
	WHEN 3 THEN 2
	WHEN 4 THEN 3
	WHEN 5 THEN 4
	WHEN 6 THEN 5
	WHEN 7 THEN 6
	WHEN 1 THEN 7
END;

--10
SELECT e.first_name, l.city, nvl(to_char(e.commission_pct), 'nema na platu')
FROM employees e, departments d, locations l
WHERE e.department_id = d.department_id
AND d.location_id = l.location_id

--11
SELECT first_name, salary, substr('************************************************', 1, floor(salary/1000)) AS "Sifrovane hiljade" FROM employees;

--12
SELECT e.first_name, e.last_name, j.job_title, 
	DECODE(j.job_title,
		'President', 'A',
		'Manager', 'B',
		'Analyst', 'C',
		'Sales manager', 'D',
		'Programmer', 'E',
		'X') FROM employees e, jobs j
WHERE e.job_id = j.job_id





