USE kainska_db;  /*тут ставиш назву своєї схеми яку створида щойно*/

DROP TABLE IF EXISTS LogData;
DROP TABLE IF EXISTS Settings;

CREATE TABLE LogData(
	ID int AUTO_INCREMENT PRIMARY KEY NOT NULL,
    time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    log varchar(50) NOT NULL
);

CREATE TABLE Settings(
	ID int AUTO_INCREMENT PRIMARY KEY NOT NULL,
    Comport varchar(5) NOT NULL,
    Speed int NOT NULL
);

/*----------------------------------------------------------------------------------------*/

INSERT INTO Settings (Comport, Speed) VALUES ("COM2", 9600);

select time as data, log from logdata;