# Ipc_graph

## 运行

- 使用FIFO

```bash
./gen_main.py
```

- 使用SHM

```bash
./gen_main.py --ipc_type Sample
```

- 编译

```bash
make clean
make all
```

- 运行

```bash
./start.sh
```

- 在`include/conf.h`文件中，将`#define DEBUG 0` 改为 `#define DEBUG 1` 可以输出一些运行信息，助于调试。
