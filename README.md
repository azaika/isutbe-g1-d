# Neutreeko (ニュートリーコ)

## リンク
- [課題サイト](https://sites.google.com/site/isutbe2020/%E3%82%B0%E3%83%AB%E3%83%BC%E3%83%97%E8%AA%B2%E9%A1%8C/%E3%82%B0%E3%83%AB%E3%83%BC%E3%83%97%E8%AA%B2%E9%A1%8C?authuser=0) (課題の説明)
- [Neutreeko 公式サイト](https://www.neutreeko.net/neutreeko.htm) (参考になるものが色々おいてある)
- [ルール説明](http://djangorec.blog.fc2.com/blog-entry-1.html) (日本語)
- [BGA](https://ja.boardgamearena.com/gamepanel?game=neutreeko) (遊べる)

## 各ファイルの説明
- README.md : 今表示されているこのファイル
- .gitignore : git で無視してほしいファイルを列挙する
- source/
  - main.c  : 最初に起動される場所
  - board.h : 盤面 (型は int\[7\]\[7\]) の取り扱いを容易にするための色々
  - ai.h : CPU の思考はここに置いていこう
- build/
  - 開発が便利になる諸々を入れられたら嬉しい (今は空)