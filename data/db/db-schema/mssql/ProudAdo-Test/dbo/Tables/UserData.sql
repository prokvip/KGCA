USE [ProudAdo-Test]

CREATE TABLE [dbo].[UserData] (
    [UserID]   NVARCHAR (50) NOT NULL,
    [Password] NVARCHAR (50) NULL,
    [Country]  INT           NULL,
    CONSTRAINT [PK_UserTable] PRIMARY KEY CLUSTERED ([UserID] ASC)
);

