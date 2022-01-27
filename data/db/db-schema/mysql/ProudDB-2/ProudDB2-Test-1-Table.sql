/*
MYSQL에는 MSSQL과 달리 프로시저내에서 리턴값을 가질 수 없습니다. 따라서 MSSQL에 있는 checkPassword.sql에
해당하는 부분은 Table을 읽어들여 서버에서 하심을 권장해 드립니다.
*/


CREATE DATABASE `ProudDB2-Test`;



USE `ProudDB2-Test`;


CREATE TABLE `Friend`(
	`UUID` VARCHAR(38) NOT NULL,
	`OwnerUUID` VARCHAR(38) NOT NULL,
	`RootUUID` VARCHAR(38) NOT NULL,
	`charLevel` INT NOT NULL,
	`Name` VARCHAR(50) NOT NULL,
	
	PRIMARY KEY(`UUID`)
	);
	
CREATE TABLE `Gamer`(
		`UUID` VARCHAR(38) NOT NULL,
		`GamerID` VARCHAR(50) NOT NULL UNIQUE,
		`OwnerUUID` VARCHAR(38) NOT NULL,
		`RootUUID` VARCHAR(38) NOT NULL,
		`Password` VARCHAR(50) NOT NULL,
		`SelectedHeroGuid` VARCHAR(38) NULL,
		
		PRIMARY KEY(`UUID`)
		);
		
CREATE TABLE `Hero`(
		`UUID` VARCHAR(38) NOT NULL,
		`OwnerUUID` VARCHAR(38) NOT NULL,
		`RootUUID` VARCHAR(38) NOT NULL,
		`Name` VARCHAR(50) NOT NULL,
		`Removed` INT NULL,
		`RemovedTime` DateTime NULL,
		`HeroType` INT NULL,
		`Sample_Score` INT NULL,
		
		PRIMARY KEY(`UUID`)
		);

CREATE TABLE `Item`(
		`UUID` VARCHAR(38) NOT NULL,
		`OwnerUUID` VARCHAR(38) NOT NULL,
		`RootUUID` VARCHAR(38) NOT NULL,
		`Name` VARCHAR(50) NOT NULL,
		`Type` INT NOT NULL,
		
		PRIMARY KEY(`UUID`)
		);

		
		
		
		
		
		
		
		
		
		
		
		
		