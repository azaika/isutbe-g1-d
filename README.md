# Neutreeko (ニュートリーコ)

## リンク
- [Neutreeko 公式サイト](https://www.neutreeko.net/neutreeko.htm) (参考になるものが色々おいてある)
- [ルール説明](http://djangorec.blog.fc2.com/blog-entry-1.html) (日本語)
- [BGA](https://ja.boardgamearena.com/gamepanel?game=neutreeko) (遊べる)

## 各ファイルの説明
- README.md : 今表示されているこのファイル
- .gitignore : git で無視してほしいファイルを列挙する
- source/
  - main.c  : 最初に起動される場所
  - game_controller.h : ゲームの主な処理を行う場所
  - ai.h : CPU の思考はここに置いていこう
  - board.h : 盤面の取り扱いを容易にするための色々
- build/
  - 開発が便利になる諸々を入れられたら嬉しい (今は空)

## 諸注意
- **座標は (y, x)** で扱うことにしましょう
- 盤面は判定を簡単にするため, 外側の壁を左右上下に一マス分取ってあります (「番兵 (sentinel)」と呼ばれる処理)
