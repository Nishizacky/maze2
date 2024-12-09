# コンパイラとフラグ
CC = gcc
CFLAGS = -g -Wall -O3 -fPIE -fPIC -lm -lpthread
LIBS = -lX11

# ターゲット名
TARGET = bin/maze2

# ソースファイルとオブジェクトファイル
SRCS = src/MAZE2.c src/drawfunc.c src/enemy.c src/globals.c src/kbhit.c src/mazemaker.c src/menu.c src/moving.c src/x11winlib_db.c
OBJS = $(SRCS:src/%.c=obj/%.o)

# ディレクトリ
OBJDIR = obj
BINDIR = bin
INCLUDEDIR = include

# ヘッダファイルのインクルードパス
INCLUDES = -I$(INCLUDEDIR)

# デフォルトターゲット
all: $(TARGET)

# ターゲットの生成ルール
$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)  # binディレクトリがなければ作成
	$(CC) $(OBJS) $(CFLAGS) $(INCLUDES) $(LIBS) -o $(TARGET)

# 個別のオブジェクトファイル生成
obj/%.o: src/%.c
	@mkdir -p $(OBJDIR)  # objディレクトリがなければ作成
	$(CC) -c $< $(CFLAGS) $(INCLUDES) -o $@

# runターゲット
run: $(TARGET)
	./$(TARGET)  # ビルド後に実行ファイルを実行

# クリーンアップ
clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(OBJDIR) $(BINDIR)
