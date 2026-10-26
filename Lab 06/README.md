# Laboratorio 06 - Análisis y Diseño de Algoritmos

Se aplica programación dinámica para resolver caminos en grafos por etapas y comparar formas de calcular Fibonacci. Cada ejercicio utiliza sus propios datos definidos en el código y se ejecuta de manera independiente.

## Ejercicios

- **01. Diligencia de costo mínimo.** Representa las conexiones mediante listas de adyacencia. Recorre las etapas desde el destino hacia el origen, guarda el mejor costo en `dp` y reconstruye una ruta mediante `siguiente`. La recurrencia toma el mínimo de `costo(u,v) + dp[v]`, con `dp[H] = 0`.
- **02. Fibonacci.** Compara recursión ingenua, memorización y tabulación para `n = 10, 20 y 30`. Las dos últimas guardan subproblemas en vectores y evitan repetir cálculos. Se mide el promedio de 30 ejecuciones por método con `steady_clock`, sin incluir la impresión; los tiempos dependen del equipo y de la compilación.
- **03. Todos los caminos óptimos.** Conserva una lista de sucesores cuando varias decisiones producen el mismo costo mínimo. Después recorre esas decisiones con una búsqueda en profundidad para imprimir todas las rutas óptimas. Enumerarlas añade un costo proporcional a las rutas generadas.
- **04. Diligencia de costo máximo.** Utiliza el grafo por etapas del ejercicio 01, pero selecciona el máximo de `costo(u,v) + dp[v]`. Los estados inalcanzables comienzan con un centinela negativo y `siguiente` permite reconstruir una ruta de costo máximo.

Los ejercicios 01 y 04 procesan cada nodo y arista una vez: su tiempo es `O(V + E)`. El orden inverso funciona porque las conexiones avanzan entre etapas de un grafo acíclico.

## Ejercicio 05: Aplicación a inventarios

Es un ejercicio de investigación y formulación, sin ejecutable. Una distribuidora decide cuántas unidades pedir en cada periodo para cubrir la demanda y reducir los costos de pedido y almacenamiento.

El estado `(t, i)` representa el periodo y el inventario inicial; la decisión `q` es la cantidad pedida. El inventario siguiente es `j = i + q - demanda[t]`. Para cada decisión factible se suma el costo del pedido, la compra, el almacenamiento y el mejor costo futuro `F(t+1, j)`. Se exige inventario final cero y se calcula desde el último periodo hacia el primero. Cada transición equivale a una arista de un grafo por etapas.

## Compilación y ejecución

Desde la carpeta `Lab 06`:

```sh
g++ Ejercicio01/main.cpp -o Ejercicio01/main.exe
g++ Ejercicio02/main.cpp -o Ejercicio02/main.exe
g++ Ejercicio03/main.cpp -o Ejercicio03/main.exe
g++ Ejercicio04/main.cpp -o Ejercicio04/main.exe

./Ejercicio01/main.exe
./Ejercicio02/main.exe
./Ejercicio03/main.exe
./Ejercicio04/main.exe
```
