# SPACE JUMP (C / OpenGL)

迫ってくるトゲトゲの敵をジャンプでかわし続ける、ワンボタンのアクションゲームです。C と OpenGL（GLUT）で書きました。

2024 年 6 月に制作したもので、のちに Three.js でブラウザ版（[space-jump](https://github.com/mimish0778/space-jump)）に移植しています。

<img width="865" height="618" alt="タイトル画面" src="https://github.com/user-attachments/assets/af1329af-7e5f-4305-9989-2cece3991c3a" />

<img width="867" height="617" alt="プレイ画面" src="https://github.com/user-attachments/assets/620bf2e4-a58a-424e-a604-f8ac60166b17" />

## 遊び方

タイトル画面で `Space` を押すとロケットが飛び立ち、ゲームが始まります。キャラクターは自動で転がって進むので、敵が来たらジャンプでよけてください。敵にぶつかるとタイトルに戻ります。

進んだ距離がスコア（m）になり、最高記録は左上の `MAX` に表示されます（ゲームを終了するとリセット）。

| キー | タイトル画面 | プレイ中 |
|---|---|---|
| `Space` / `↑` | スタート | ジャンプ |
| `Esc` | 終了 | 終了 |

## ビルド

Windows 向けです。GLUT のヘッダ・ライブラリ・DLL はリポジトリに同梱しています。

### Visual Studio

1. 空の C/C++ プロジェクトを作り、`src/report.c` を追加する
2. インクルードディレクトリに `include/`、ライブラリディレクトリに `lib/` を追加する
3. 追加の依存ファイルに `glut32.lib` `opengl32.lib` `glu32.lib` を指定する
4. ビルドしたら、`bin/glut32.dll` を `.exe` と同じフォルダにコピーして実行する

### MinGW

```sh
gcc src/report.c -Iinclude -Llib -lglut32 -lopengl32 -lglu32 -o space-jump.exe
```

こちらも実行前に `glut32.dll` を `space-jump.exe` と同じフォルダに置いてください。

## ファイル構成

```
src/report.c       ゲーム本体（描画・入力・当たり判定すべて）
include/myShape.h  円柱・円盤などの図形を描くユーティリティ
include/GL/        OpenGL / GLU / GLUT のヘッダ
lib/               glut32.lib, glut.def
bin/glut32.dll     実行時に必要な DLL
```

キャラクターやロケットはすべて球・円柱・立方体などの組み合わせで描いています。シーンの切り替えは、大きな壁が画面を横切るワイプで表現しています。

動きはフレームごとに一定量進める作りなので、PC の性能によってゲームの速さが変わります。

## クレジット

- `include/myShape.h` — Copyright (c) 2003 by T. HAYASHI and K. KATO
- `include/GL/`、`lib/`、`bin/glut32.dll` — GLUT（OpenGL Utility Toolkit）の配布物です。ライセンス・著作権は配布元に帰属します。
