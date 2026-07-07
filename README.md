# space-jump-opengl

C / OpenGL (GLUT) で実装したジャンプアクションゲームです。ロケット発射のスタート画面から始まり、障害物（敵）を避けながらスコアを稼いでいく2Dライクな3D描画のミニゲームです。

> 2024年6月制作。後に Three.js 版へ移植する前の、オリジナルの C/OpenGL 実装です。

## 概要

- 言語: C
- グラフィックス: OpenGL + GLU + GLUT（Windows / freeglut 互換）
- 画面構成:
  1. **スタート画面** — ロケットを発射してゲームを開始
  
  <img width="865" height="618" alt="title" src="https://github.com/user-attachments/assets/af1329af-7e5f-4305-9989-2cece3991c3a" />
  
  2. **ゲーム画面** — プレイヤーが自動で走り続け、ジャンプで障害物・敵を回避してスコアを稼ぐ
  
  <img width="867" height="617" alt="play" src="https://github.com/user-attachments/assets/620bf2e4-a58a-424e-a604-f8ac60166b17" />

## 操作方法

| キー | 動作 |
|------|------|
| `Space` または `↑（矢印キー上）` | スタート画面ではロケット発射 / ゲーム中はジャンプ |
| `Esc` | ゲーム終了 |

## ディレクトリ構成

```
space-jump-opengl/
├── src/
│   └── report.c        # メインプログラム
├── include/
│   ├── myShape.h        # 円・円柱などの基本形状描画ユーティリティ
│   └── GL/               # OpenGL / GLU / GLUT ヘッダ
├── lib/
│   ├── glut32.lib
│   └── glut.def
└── bin/
    └── glut32.dll        # 実行時に必要（実行ファイルと同じディレクトリに配置）
```

## ビルド方法（Windows / Visual Studio）

1. 新しい C/C++ コンソール（または Windows デスクトップ）プロジェクトを作成
2. `src/report.c` と `include/myShape.h` をプロジェクトに追加
3. プロジェクトのインクルードディレクトリに `include/` を追加
4. プロジェクトのライブラリディレクトリに `lib/` を追加し、追加の依存ファイルに `glut32.lib`（および `opengl32.lib`, `glu32.lib`）を指定
5. 実行時に `bin/glut32.dll` を実行ファイル（.exe）と同じフォルダにコピー
6. ビルドして実行

### MinGW でのビルド例

```sh
gcc src/report.c -Iinclude -Llib -lglut32 -lopengl32 -lglu32 -o space-jump.exe
```

実行前に `glut32.dll` を `space-jump.exe` と同じフォルダに置いてください。

## 注意事項

- `GL/`, `glut32.dll`, `glut32.lib`, `glut.def` は GLUT（OpenGL Utility Toolkit）配布物に含まれるファイルです。ライセンス・著作権は各配布元に帰属します。
- `myShape.h` は Copyright (c) 2003 by T. HAYASHI and K. KATO によるユーティリティです。
