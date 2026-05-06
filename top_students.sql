-- Step 1:Creating the db,,db schema
CREATE TABLE IF NOT EXISTS STUDENT (
    ID INTEGER PRIMARY KEY,
    NAME VARCHAR(50),
    SCORE FLOAT
);

--Step 2: Sample data insertion given in the assessment
INSERT INTO STUDENT (ID, NAME, SCORE) VALUES
(1,  'Bob',   50.0),
(2,  'John',  65.5),
(3,  'Harry', 45.0),
(4,  'Dick',  85.0),
(5,  'Dev',   25.0),
(6,  'Sid',   98.0),
(7,  'Tom',   90.0),
(8,  'Julia', 70.5),
(9,  'Erica', 81.0),
(10, 'Jerry', 85.0);

-- Step 3: The actual solution query
SELECT ID, NAME
FROM STUDENT
ORDER BY SCORE DESC, ID ASC
LIMIT 3;