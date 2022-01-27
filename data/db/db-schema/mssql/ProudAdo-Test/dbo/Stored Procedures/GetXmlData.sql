USE [ProudAdo-Test]

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[GetXmlData]
@id int
AS
BEGIN
SET NOCOUNT ON;
select id,xmldat.query('//Cats') as xmldat from XMLData where id=@id;

return 0;
END
GO