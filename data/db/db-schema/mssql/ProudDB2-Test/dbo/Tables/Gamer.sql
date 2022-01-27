CREATE TABLE [dbo].[Gamer] (
    [UUID]             UNIQUEIDENTIFIER NOT NULL,
    [GamerID]          VARCHAR (50)     NOT NULL,
    [OwnerUUID]        UNIQUEIDENTIFIER NULL,
    [RootUUID]         UNIQUEIDENTIFIER NULL,
	[Password]         VARCHAR(50) NOT NULL,
    [SelectedHeroGuid] UNIQUEIDENTIFIER NULL,
    CONSTRAINT [PK_Gamer] PRIMARY KEY CLUSTERED ([UUID] ASC)
);


GO
CREATE NONCLUSTERED INDEX [IX_Gamer_RootUUID]
    ON [dbo].[Gamer]([RootUUID] ASC);


GO
CREATE NONCLUSTERED INDEX [IX_Gamer_OwnerUUID]
    ON [dbo].[Gamer]([OwnerUUID] ASC);

