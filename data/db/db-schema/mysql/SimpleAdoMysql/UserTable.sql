
DELIMITER $$

CREATE DATABASE `proudado_test`$$

USE `proudado_test`$$

CREATE TABLE `UserTable`(
	`UserID` VARCHAR(50) NOT NULL,
	`Password` VARCHAR(50) NULL,
	`Country` INT NULL,
	PRIMARY KEY (`UserID`)
)$$

DELIMITER ;
