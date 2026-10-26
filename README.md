# space-jump-opengl

C と OpenGL（GLUT）で作った、障害物をジャンプでかわして距離を伸ばす横スクロールのアクションゲームです。ロケットが発進するスタート画面から始まり、転がってくるトゲの球をよけながら進みます。

2024 年 6 月に制作したオリジナル版で、のちに Three.js へ移植したブラウザ版が [space-jump](https://github.com/mimish0778/space-jump) です。

| スタート画面 | ゲーム画面 |
| --- | --- |
| <img width="865" height="618" alt="スタート画面" src="https://github.com/user-attachments/assets/af1329af-7e5f-4305-9989-2cece3991c3a" /> | <img width="867" height="617" alt="ゲーム画面" src="https://github.com/user-attachments/assets/620bf2e4-a58a-424e-a604-f8ac60166b17" /> |

## 遊び方

1. スタート画面で `Space` か `↑` を押すと、ロケットが発進してゲームが始まります。
2. 右からトゲの球が転がってきます。`Space` か `↑` でジャンプして飛び越えてください。
3. トゲの球にぶつかると、スタート画面に戻ります。

進んだ距離（m）がスコアになり、画面右上に表示されます。最高記録は画面左上の `MAX` に表示されます。最高記録は起動している間だけ保持され、ゲームを終了するとリセットされます。

## 操作方法

| キー | 動作 |
| --- | --- |
| `Space` / `↑` | スタート画面ではロケットを発進させてゲームを開始、ゲーム中はジャンプ |
| `Esc` | ゲームを終了 |

ジャンプは地面にいるときだけできます。

## 必要なもの

- C コンパイラ
- OpenGL、GLU
- GLUT（Windows 用の GLUT は `include/`、`lib/`、`bin/` に同梱しています。Linux では freeglut が使えます）

## ビルドと実行

### Windows（Visual Studio）

1. C/C++ のコンソールアプリのプロジェクトを作ります。
2. `src/report.c` をプロジェクトに追加します。
3. インクルードディレクトリに `include/` を追加します。
4. ライブラリディレクトリに `lib/` を追加し、追加の依存ファイルに `glut32.lib`、`opengl32.lib`、`glu32.lib` を指定します。
5. ビルドし、`bin/glut32.dll` を実行ファイル（`.exe`）と同じフォルダにコピーしてから実行します。

### Windows（MinGW）

```sh
gcc src/report.c -Iinclude -Llib -lglut32 -lopengl32 -lglu32 -o space-jump.exe
```

実行する前に、`bin/glut32.dll` を `space-jump.exe` と同じフォルダに置いてください。

### Linux（freeglut）

```sh
# Debian / Ubuntu の場合
sudo apt install build-essential freeglut3-dev

gcc src/report.c -Iinclude -lglut -lGLU -lGL -lm -o space-jump
./space-jump
```

Linux（GCC 13、freeglut）では、このコマンドでビルドと実行ができることを確認しています。

### GCC 14 以降を使う場合

ソースコードには、戻り値の型を省略した関数の定義と、`<string.h>` を読み込まずに使っている `strlen` があります。GCC 14 以降ではこれらが既定でエラーになるため、次のオプションを追加してください（GCC 13 以前でも同じオプションを付けてビルドできます）。

```sh
-Wno-error=implicit-int -Wno-error=implicit-function-declaration
```

## 仕組み

- 画面は 700 x 500 のウィンドウで、スタート画面とゲーム画面の 2 つのシーンを切り替えます。切り替えのときは、大きな板が画面を横切る演出が入ります。
- キャラクター、トゲの球、ロケットは、GLUT の球、円錐、立方体と、`myShape.h` の円柱を組み合わせて描いています。
- 動きはすべて 1 フレームごとの移動量で決まっています（経過時間による補正はありません）。そのため、実行する環境の描画速度によってゲームの速さが変わります。

## プロジェクト構成

```
space-jump-opengl/
├── src/
│   └── report.c      ゲーム本体
├── include/
│   ├── myShape.h     円柱などの形状を描くユーティリティ
│   └── GL/           OpenGL / GLU / GLUT のヘッダー（Windows 用）
├── lib/
│   ├── glut32.lib    GLUT のインポートライブラリ（Windows 用）
│   └── glut.def
└── bin/
    └── glut32.dll    実行時に必要な GLUT の DLL（Windows 用）
```

## ライセンスについて

- `include/GL/`、`lib/glut32.lib`、`lib/glut.def`、`bin/glut32.dll` は GLUT（OpenGL Utility Toolkit）の配布物に含まれるファイルです。著作権とライセンスはそれぞれの配布元に帰属します。
- `include/myShape.h` は T. HAYASHI 氏と K. KATO 氏によるユーティリティです（Copyright (c) 2003）。
