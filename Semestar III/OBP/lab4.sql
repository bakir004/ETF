SELECT first_name, last_name, employees.department_id, department_name
FROM employees
LEFT JOIN departments
ON employees.department_id = departments.department_id;

SELECT DISTINCT employees.job_id FROM employees
LEFT JOIN jobs
ON employees.job_id = jobs.job_id
WHERE employees.department_id = 30

SELECT first_name, last_name, department_name, locations.* FROM employees
LEFT JOIN departments ON employees.department_id = departments.department_id
LEFT JOIN locations ON departments.location_id = locations.location_id
WHERE employees.commission_pct IS NULL

SELECT first_name, department_name FROM employees
LEFT JOIN departments ON employees.department_id = departments.department_id
WHERE LOWER(first_name) LIKE '%a%'

SELECT first_name, job_title, employees.department_id, department_name, city FROM employees
LEFT JOIN jobs ON employees.job_id = jobs.job_id
LEFT JOIN departments ON employees.department_id = departments.department_id
LEFT JOIN locations ON departments.location_id = locations.location_id
WHERE locations.city = 'Dallas'

SELECT a.first_name || ' ' || a.last_name AS "Naziv zaposlenog", 
a.employee_id AS "Shifra zaposlenog", 
b.first_name || ' ' || b.last_name AS "Naziv shefa", 
b.employee_id AS "Shifra shefa",
city AS "Grad shefa"
FROM employees a 
LEFT JOIN employees b ON a.manager_id = b.employee_id
LEFT JOIN departments ON b.department_id = departments.department_id
LEFT JOIN locations ON departments.location_id = locations.location_id

SELECT a.first_name, a.department_id, b.first_name FROM employees a
LEFT JOIN departments d ON a.department_id = d.department_id
LEFT JOIN employees b ON d.department_id = b.department_id

SELECT first_name, job_title, department_name, salary, salary*(1+commission_pct), j.min_salary, j.max_salary
FROM employees e
LEFT JOIN departments d ON e.department_id = d.department_id
LEFT JOIN jobs j ON e.job_id = j.job_id
WHERE commission_pct IS NOT NULL AND salary*(1+commission_pct) BETWEEN j.min_salary AND j.max_salary

SELECT a.first_name, a.hire_date, b.first_name, b.hire_date FROM employees a
LEFT JOIN employees b ON b.first_name = 'Douglas'
WHERE a.hire_date > b.hire_date

SELECT a.first_name, a.hire_date, b.first_name AS "Shef name", b.hire_date AS "Shef Date" FROM employees a
LEFT JOIN employees b ON a.manager_id = b.employee_id
WHERE a.hire_date > b.hire_date

SELECT * FROM employees
WHERE hire_date > TO_DATE('01.01.2000', 'dd.mm.yyyy') 

SELECT decode(
	MOD(to_number(to_char(hire_date, 'yyyy')), 2),
	1, ADD_MONTHS(hire_date, -1),
	0, ADD_MONTHS(hire_date, 1) 
), hire_date FROM employees
WHERE TO_NUMBER(to_char(hire_date, 'yyyy')) > 2005

SELECT sum(salary) FROM employees WHERE employee_id IS NULL

SELECT Count(job_id), to_number(to_char(hire_date, 'DD')) FROM employees GROUP BY to_number(to_char(hire_date, 'DD')) HAVING count(*) > 11 ORDER BY count(job_id)














