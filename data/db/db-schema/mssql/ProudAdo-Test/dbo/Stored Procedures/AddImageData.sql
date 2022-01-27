USE [ProudAdo-Test]

SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[AddImageData]
	-- Add the parameters for the stored procedure here
	@FileName varchar(200),
	@TestImage image
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	
	if exists (select ImageName from ImageData where ImageName=@FileName)	
	begin
		return -1
	end	
	insert into ImageData (ImageName,TestImage) values (@FileName,@TestImage)

	return 0
END

