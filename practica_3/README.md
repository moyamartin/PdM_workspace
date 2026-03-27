# Practica 3

Este ejercicio implementa un modulo de software para trabajar con retardos no bloqueantes
definido por la estructura `delay_t`, utilizando como Tick de referencia la HAL correspondiente
(`HAL_GetTick` que devuelve el valor de `uwTick`) dentro de la carpeta
Drivers/API:

```
struct {
    tick_t startTime;
    tick_t duration;
    bool_t running;
} delay_t;
```

y las funciones

- `void delayInit(delay_t* delay, tick_t duration)`: inicializa el objeto delay pasado como argumento con el valor de `duration`.
- `bool_t delayRead(delay_t* delay)`: Verifica si el delay ya llego al timeout en caso de que este corriendo, si el delay no esta corriendo, esta funcion lo pone a correr.
- `void delayWrite(delay_t* delay, tick_t duration)`: Modifica la propiedad `duration` con el valor pasado por argumento al objeto.
- `void isDelayRunning(delay_t* delay)`: Devuelve el valor de la variable
  `running`

## Ejemplo

Dentro de `main.c` se muestra un ejemplo de como implementar `delay_t`. Este ejemplo, hace
parpadear un LED con el siguiente patron:

1. 5 veces con periodo de 500ms y ciclo de trabajo 50%.
2. 5 veces con periodo de 100ms y ciclo de trabajo 50%
3. 5 veces con periodo de 100ms y ciclo de trabajo 50%.
4. 5 veces con periodo de 1000ms y ciclo de trabajo 50%.

## Compilacion

El proceso de compilacion se puede realizar de dos formas.

1. Usando la terminal a traves de CMake
(necesario tener instalado toolchain de [ARM](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain) y [ninja](https://ninja-build.org/)):

```
$ cmake --preset Debug
$ cd build/Debug
$ ninja
Memory region         Used Size  Region Size  %age Used
             RAM:        1680 B       128 KB      1.28%
           FLASH:        9196 B       512 KB      1.75%
```

2. Usando STM32CubeIDE, importar este proyecto y hacer click en el martillo

## Ejecucion

Para cargar el binario final en la placa se puede hacer de dos formas.

1. A traves de la terminal usando [stlink](https://github.com/stlink-org/stlink)
   y `gdb-multiarch` en simulation.

en una ventana ejecutar st-util:

```
$ st-util
st-util 1.8.0
2026-03-13T11:25:23 INFO common.c: STM32F446: 128 KiB SRAM, 512 KiB flash in at least 128 KiB pages.
2026-03-13T11:25:23 INFO gdb-server.c: Listening at *:4242...
2026-03-13T11:26:34 INFO common.c: STM32F446: 128 KiB SRAM, 512 KiB flash in at least 128 KiB pages.
2026-03-13T11:26:34 INFO gdb-server.c: Found 6 hw breakpoint registers
2026-03-13T11:26:34 INFO gdb-server.c: GDB connected.
2026-03-13T11:26:37 INFO common.c: STM32F446: 128 KiB SRAM, 512 KiB flash in at least 128 KiB pages.
2026-03-13T11:26:37 INFO gdb-server.c: flash_erase: block 08000000 -> 4000
2026-03-13T11:26:37 INFO gdb-server.c: flash_erase: page 08000000
EraseFlash - Sector:0x0 Size:0x4000 2026-03-13T11:26:37 INFO flash_loader.c: Starting Flash write for F2/F4/F7/L4
2026-03-13T11:26:37 INFO flash_loader.c: Successfully loaded flash loader in sram
2026-03-13T11:26:37 INFO flash_loader.c: Clear DFSR
2026-03-13T11:26:37 INFO flash_loader.c: enabling 32-bit flash writes
2026-03-13T11:26:37 INFO gdb-server.c: flash_do: block 08000000 -> 4000
2026-03-13T11:26:37 INFO gdb-server.c: flash_do: page 08000000
2026-03-13T11:26:40 INFO gdb-server.c: Found 6 hw breakpoint registers
```

y en otra terminal usar gdb-multiarch con el binario compilado:

```
$ gdb-multiarch -f build/Debug/practica_3.elf
GNU gdb (Ubuntu 15.0.50.20240403-0ubuntu1) 15.0.50.20240403-git
Copyright (C) 2024 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from practica_2.elf...
(gdb) target extended localhost:4242
Remote debugging using localhost:4242
0x08000564 in __libc_init_array () at ../../../../../../newlib/libc/misc/init.c:44
warning: 44     ../../../../../../newlib/libc/misc/init.c: No such file or directory
(gdb) load
Loading section .isr_vector, size 0x1c4 lma 0x8000000
Loading section .text, size 0x21ec lma 0x80001c4
Loading section .rodata, size 0x18 lma 0x80023b0
Loading section .ARM, size 0x8 lma 0x80023c8
Loading section .init_array, size 0x4 lma 0x80023d0
Loading section .fini_array, size 0x4 lma 0x80023d4
Loading section .data, size 0x14 lma 0x80023d8
Start address 0x08002344, load size 9196
Transfer rate: 9 KB/sec, 1313 bytes/write.
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/mmoya/projects/cese/PdM_workspace/practica_2/build/Debug/practica_2.elf
```

2. A traves del STM32CubeIDE haciendo click en el boton play
