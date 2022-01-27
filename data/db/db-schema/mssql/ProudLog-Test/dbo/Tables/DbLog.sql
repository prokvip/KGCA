CREATE TABLE [dbo].[DbLog] (
    [Index]          BIGINT         IDENTITY (1, 1) NOT NULL,
    [LoggerName]     NVARCHAR (256) NULL,
    [LogText]        NVARCHAR (256) NULL,
    [InsertDateTime] DATETIME       CONSTRAINT [DF_DbLog_InsertDateTime] DEFAULT (getdate()) NULL,
    [TestField]      BIGINT         NULL,
    CONSTRAINT [PK_DbLog] PRIMARY KEY CLUSTERED ([Index] ASC)
);

