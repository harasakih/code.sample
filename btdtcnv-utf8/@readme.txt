test3
BUG-2005/10/11-001      レコードＩＤ判定外のレコードがあった時、レコード長のみのゴミレコードをだしてしまう。
        10/11改修

BUG-2005/10/18-001      UNPACK(btpd2hs)で、４ビットが数値例外（０〜９以外）でも、そのままＵＮＰＡＣＫ化している。
                        そのため、ＵＮＰＡＣＫキャラが数字以外となりＰＯＳＴＧＲＥＳに入れるとき、ＩＮＴで受けると
                        データ例外となる。
        10/18改修       データ例外の４ビットは、無条件にゼロとする。
