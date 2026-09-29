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




# Thu Sep 24 05:47:40 PM CEST 2026
- Activated venv convenient folder `~/Projects/zephyrproject/.venv`
- Change directory to the root of this repo
- Changed version to v4.4.2 in `west.yml`
- Modified and commented original delay of `blinky` to see it blink at 500ms


```bash
west build -p always -b nucleo_f439zi app
west flash -r openocd
west debug
```



# Mon Sep 28 05:16:13 PM CEST 2026

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





