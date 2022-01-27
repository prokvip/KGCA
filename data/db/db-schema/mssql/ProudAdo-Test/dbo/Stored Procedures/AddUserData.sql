

-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
USE [ProudAdo-Test]

CREATE PROCEDURE [dbo].[AddUserData]
	-- Add the parameters for the stored procedure here
	@UserID varchar(50),
	@Password varchar(50),
	@Country int
		
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	
	if exists (select UserID from UserData where UserID=@UserID)	
	begin
		return -1
	end	
	insert into UserData (UserID,Password,Country) values (@UserID,@Password,@Country)

	return 0
END



