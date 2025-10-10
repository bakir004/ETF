SELECT * FROM employees

-- 1
SELECT e.first_name, e.last_name, e.department_id, e.job_id
FROM employees e
WHERE e.department_id = (SELECT s.department_id FROM employees s WHERE s.first_name = 'Susan')

-- 2
SELECT e.employee_id, e.first_name, e.salary
FROM employees e
WHERE e.salary > (SELECT avg(s.salary) FROM employees s WHERE s.department_id IN (30,90))

-- 3
SELECT * FROM employees e
WHERE e.department_id =ANY (SELECT s.department_id FROM employees s WHERE s.first_name LIKE '%c%')

SELECT * FROM locations

-- 4
SELECT * FROM employees e
WHERE e.department_id =ANY (SELECT s.department_id FROM employees s, departments d, locations l
							WHERE s.department_id = d.department_id
							AND d.location_id = l.location_id
							AND l.city = 'Toronto')
							
-- 5
SELECT * FROM employees e
WHERE e.manager_id = (SELECT s.employee_id FROM employees s WHERE s.last_name = 'King' AND s.first_name = 'Steven')

-- 6
SELECT * FROM employees e
WHERE e.salary > (SELECT avg(s.salary) FROM employees s WHERE s.department_id = e.department_id)

-- 7
SELECT * FROM employees e
WHERE e.commission_pct IS NULL AND e.department_id =ANY (SELECT s.department_id 
													  FROM employees s 
													  WHERE s.salary = e.salary
													  AND s.commission_pct IS NOT NULL)
													  
-- 8
SELECT * FROM employees e
WHERE (e.salary, e.commission_pct) =ANY (SELECT s.salary, s.commission_pct FROM employees s, departments d, locations l
						  WHERE s.department_id = d.department_id 
						  AND d.location_id = l.location_id
						  AND l.city = 'Rome')
						  
-- 9
SELECT * FROM employees e
WHERE (e.salary, e.commission_pct) =ANY (SELECT s.salary, s.commission_pct FROM employees s WHERE s.first_name = 'Scott')

SELECT * FROM departments

-- 10
SELECT * FROM employees e
WHERE e.salary > (SELECT max(s.salary) FROM employees s WHERE s.department_id = 80)
ORDER BY e.salary

-- 11
SELECT * FROM employees e
WHERE e.salary > (SELECT DISTINCT avg(r.salary) FROM employees s
				  LEFT JOIN employees r ON s.manager_id = r.employee_id
				  WHERE s.department_id = e.department_id)
				  
				  
SELECT DISTINCT avg(r.salary) FROM employees s
  LEFT JOIN employees r ON s.manager_id = r.employee_id
  WHERE s.department_id = 80
  
-- 12
SELECT e.employee_id, e.first_name, e.department_id, e.salary,
	(SELECT avg(s.salary) 
	 FROM employees s
	 WHERE s.department_id = e.department_id) AS "Prosjek odjela",
	(SELECT min(s.salary) 
	 FROM employees s
	 WHERE s.department_id = e.department_id) AS "Min odjela",
	(SELECT max(s.salary) 
	 FROM employees s
	 WHERE s.department_id = e.department_id) AS "Max odjela",
	 (SELECT min(k.salary) FROM employees k WHERE e.employee_id = k.employee_id),
	 (SELECT max(k.salary) FROM employees k WHERE e.employee_id = k.employee_id),
	 (SELECT avg(k.salary) FROM employees k WHERE e.employee_id = k.employee_id)
FROM employees e
WHERE e.salary > (SELECT DISTINCT avg(r.salary) FROM employees s
				  LEFT JOIN employees r ON s.manager_id = r.employee_id
				  WHERE s.department_id = e.department_id)			  

































