CREATE TABLE [dbo].[Hero] (
    [UUID]         UNIQUEIDENTIFIER NOT NULL,
    [OwnerUUID]    UNIQUEIDENTIFIER NOT NULL,
    [RootUUID]     UNIQUEIDENTIFIER NOT NULL,
    [Name]         VARCHAR (50)     NOT NULL,
    [Removed]      INT              NULL,
    [RemovedTime]  DATETIME         NULL,
    [HeroType]     INT              NULL,
    [Sample_Score] INT              NULL,
    CONSTRAINT [PK_Hero] PRIMARY KEY CLUSTERED ([UUID] ASC)
);


GO
CREATE NONCLUSTERED INDEX [IX_Hero_RootUUID]
    ON [dbo].[Hero]([RootUUID] ASC);

