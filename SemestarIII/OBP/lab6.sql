-- 1
SELECT sum(salary*commission_pct), count(commission_pct), count(*) FROM employees

SELECT * FROM employees WHERE department_id = 50;
-- 2
SELECT count(*), e.department_id, e.job_id, d.department_name
FROM employees e, departments d
WHERE e.department_id = d.department_id
GROUP BY e.department_id, e.job_id, d.department_name;

-- 3
SELECT min(salary), max(salary), round(sum(salary),6), round(avg(salary), 6) FROM employees;

-- 4
SELECT min(salary), max(salary), round(sum(salary),6), round(avg(salary), 6), job_id, count(*) FROM employees
GROUP BY job_id

-- 5
SELECT count(*), job_id FROM employees
GROUP BY job_id

-- 6 

SELECT count(DISTINCT m.employee_id) FROM employees e, employees m
WHERE e.manager_id = m.employee_id

SELECT * FROM employees

-- 7
SELECT DISTINCT count(m.department_id), m.employee_id, m.first_name, m.department_id FROM employees e
LEFT JOIN employees m ON m.employee_id = e.manager_id
WHERE e.manager_id IS NOT NULL
GROUP BY m.department_id,m.employee_id, m.first_name

SELECT DISTINCT m.employee_id, m.first_name, m.department_id, s.first_name FROM employees e, employees m, employees s
WHERE e.manager_id = m.employee_id 
AND e.manager_id IS NOT NULL

SELECT DISTINCT m.employee_id, m.first_name, m.department_id FROM employees e, employees m, employees s
WHERE e.manager_id = m.employee_id 
AND e.manager_id IS NOT NULL

-- 9
SELECT count(*), TO_CHAR(e.hire_date, 'yyyy') FROM employees e WHERE TO_CHAR(e.hire_date, 'yyyy') IN ('2003', '2007', '2008', '2004')
GROUP BY TO_CHAR(e.hire_date, 'yyyy')

-- 10
SELECT job_id, department_id, sum(salary) FROM employees GROUP BY job_id, department_id

SELECT e.salary, s.alijas FROM employees e, (SELECT max(salary) alijas FROM employees) s
