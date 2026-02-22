# Scanner de puertos multi-threaded

Es un scanner multi-threaded para scanear los puertos abiertos de cualquier
ip.

---

En las files se encuentra un server.cpp simple 
que se utilizo como testeo del scanner.

# Para buildearlo

Para buildear se requiere gcc y se puede simplemente compilar con g++.

```
g++ -std=c++17 main.cpp -o main
g++ -std=c++17 server.cpp -o server
```
