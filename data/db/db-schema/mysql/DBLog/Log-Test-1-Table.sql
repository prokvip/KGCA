DELIMITER $$

CREATE TABLE `DbLog`(
	`Index` bigint NOT NULL auto_increment,
	`LoggerName` nvarchar(256) NULL,
	`LogText` nvarchar(256) NULL,
	`InsertDateTime` datetime NULL,
	`TestField` bigint NULL,
	PRIMARY KEY(`Index`)
)
$$
DELIMITER ;


