DELIMITER $$

USE `proudado_test`$$

DROP PROCEDURE IF EXISTS `AddUserData`$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `AddUserData`(
	IN InUserID VARCHAR(50),
	IN InPassword VARCHAR(50),
	IN InCountry INT
	)
BEGIN
	IF (SELECT COUNT(UserID) FROM UserTable WHERE UserID=InUserID <> 0) THEN
		SELECT -1;
	ELSE
	    	INSERT INTO UserTable(UserID,PASSWORD,Country) VALUES(InUserID,InPassword,InCountry);
		SELECT 0;
	END IF;
END$$

DROP PROCEDURE IF EXISTS `GetUserData`$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `GetUserData`(
	IN InUserID varchar(50)
)
BEGIN
	SELECT UserID, Password, Country FROM UserTable WHERE UserID = InUserID;
END$$

DELIMITER ;
