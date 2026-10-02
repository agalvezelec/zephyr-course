# Zephyr Training Environment

Welcome to the Zephyr RTOS training! This repository includes a ready-to-use
development environment based on Zephyr 4.3.0, which you can set up in one of
three ways:

---

## Manual Zephyr Setup

Follow the following guide:
- [Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#).

Make sure to select appropriate OS and to perform all steps till
[Build the Blinky Sample](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#build-the-blinky-sample).




# Module 02
Thu Sep 24 05:47:40 PM CEST 2026
- Activated venv convenient folder `~/Projects/zephyrproject/.venv`
- Change directory to the root of this repo
- Changed version to v4.4.2 in `west.yml`
- Modified and commented original delay of `blinky` to see it blink at 500ms


```bash
west build -p always -b nucleo_f439zi app
west flash -r openocd
west debug
```



# Module 03
Mon Sep 28 05:16:13 PM CEST 2026

- `Kconfig`: `zephyr-course/app` is a TUI template for Kconfig system. Defines what will be shown in `west build -t menuconfig`

Actual compilation cascade:

- `prj.conf`: `zephyr-course/app` 
- `.config`:  `zephyr-course/build/zephyr`
- `autoconf.h`: `zephyr-course/build/zephyr/include/generated/zephyr/`

## Demo 1: 

- `Kconfig` is created at the app level with custom definition: `BLINK_SLEEP_TIME_MS`
- `main.cpp` is modified to include the new `Kconfig` definition. Get rid of:

```C
#define SLEEP_TIME_MS 1000
```

Change the old definition in the argument:


```C
k_msleep(SLEEP_TIME_MS);
//becomes
k_msleep(CONFIG_BLINK_SLEEP_TIME_MS);
```

Note that `BLINK_SLEEP_TIME_MS` should include the `CONFIG_` prefix.

Build:

```bash
west build -p always -b nucleo_f439zi app
```

TUI: 


```bash
west build -t menuconfig
```

## Demo 2
No need to modify `main.cpp`. In `choice` we declare booleans. Then in the `config` block (no prompt means hidden in `menuconfig`) the conditional statments will check which boolean is active and asign that particular definition to the global definition. 

Functionaly Kconfig works as expected, but the options are not shown exactly as in the Lecture pdf (pg. 34).





# Module 04
Wed Sep 30 05:45:27 PM CEST 2026

- Identify on-board LED's **node label** in `nucleo_f429zi.dts`: `green_led_1`
- Create `app.overlay` and add alias `app-led`
- Blink period is defined in `Kconfig`
- Modify old `main.cpp`:

 ```C
// Careful with the dash symbol: the compiler uses underscores
#define LED_NODE DT_ALIAS(app_led)
...
k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
```



Check build and flash:

```bash
west build -p always -b nucleo_f439zi app
west build -t menuconfig
west flash -r openocd
```


# Module 05 
Thu Oct  1 05:15:19 PM CEST 2026

## Assignment 1:

- Create a custom board using the "Copy/Rename" method
- Build the hello world sample for said board
- Place the board directory in <project_root>/boards/
- Push and tag it as l5-task1

Set `ZEPHYR_BASE=$HOME/Projects/zephyrproject/zephyr`

- Copy and rename board files to root dir.

Hello world. This works:

```bash
west build -p -b custom_nucleo $ZEPHYR_BASE/samples/hello_world -- -DBOARD_ROOT=$PWD
```

This also works:

```bash
export BOARD_ROOT=$PWD
west build -p -b custom_nucleo $ZEPHYR_BASE/samples/hello_world
```


[x] CMake error: without enviroment defined,  _No board named 'custom_nucleo' found_





app:

```bash
west build -p -b custom_nucleo app/
```


For me it did not work the `--board-dir` flag. Instead I set `export BOARD_ROOT=$PWD` as environment variable


Flash: 

Add line to Cmake: `board_runner_args(openocd --cmd-pre-init "source [find board/st_nucleo_f4.cfg]")`


```bash
west flash -r openocd
```

Minicom shows message correctly






## Assignment 2:
Scratch method:

- Kconfig files and yaml created from scratch
- Devicetree is a cropped version of the original
- `CMakeLists.txt` should include `board.c` for `printk()` function


```bash
boards/scratch_nucleo/
├── board.c
├── board.yml
├── CMakeLists.txt
├── Kconfig.defconfig
├── Kconfig.scratch_nucleo
└── scratch_nucleo.dts

1 directory, 6 files
```


[x] CMake error  related to `scratch_nucleo.dts`:

Clocks should be included as nodes


Build:

```bash
west build -p -b scratch_nucleo $ZEPHYR_BASE/samples/hello_world
```


[x] Flash error:

Missing `board.cmake`

```bash
-- west flash: rebuilding
ninja: no work to do.
FATAL ERROR: no runners.yaml found in /home/gsa/Projects/zephyr-course/build/zephyr. Either board scratch_nucleo/stm32f429xx doesn't support west flash/debug/simulate, or a pristine build is needed.

```


Missing `scratch_nucleo_defconfig` . Now minicom shows the message

Confusion between purpose of:  `scratch_nucleo_defconfig`, `Kconfig.defconfig` and `Kconfig.scratch_nucleo`


Check command for debug:   

```bash
grep -E 'CONFIG_(SERIAL|CONSOLE|UART_CONSOLE|PRINTK|SHELL)'     build/zephyr/.config
```

All is well set.


