
DELIMITER $$

CREATE PROCEDURE `GetLogAll`()
BEGIN
	select * from DbLog;
END
$$
DELIMITER ;


