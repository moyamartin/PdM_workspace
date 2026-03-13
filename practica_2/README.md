# Practica 2

Este ejercicio implementa un modulo de software para trabajar con retardos no bloqueantes
definido por la estructura `delay_t`, utilizando como Tick de referencia la HAL correspondiente
(`HAL_GetTick` que devuelve el valor de `uwTick`):

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

# Ejemplo

Dentro de `main.c` se muestra un ejemplo de como implementar `delay_t`. Este ejemplo, hace
parpadear un LED con el siguiente patron:

1. 5 veces con periodo de 1000ms y ciclo de trabajo 50%.
2. 5 veces con periodo de 200ms y ciclo de trabajo 50%
3. 5 veces con periodo de 100ms y ciclo de trabajo 50%.