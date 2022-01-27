USE [ProudAdo-Test]

SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[XMLData](
	[id] [int] NULL,
	[xmldat] [xml] NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO

insert into XMLData select 1,'
<?Names of Cat?>
<Root>
<!-- How to name your cat -->
<Cats>
<AmericanShorthair color="Grey" Friendly="No">Tiger</AmericanShorthair>
<PersianHimalayan>MeowMeow</PersianHimalayan>
<JapaneseBobtail>Rainbow River</JapaneseBobtail>
</Cats>
</Root>'
GO
