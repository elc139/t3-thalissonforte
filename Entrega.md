Programação Paralela Multithread
======================================
<div align="right"> Programação Paralela - ELC139</div>

Nome: <b>Thalisson Forte</b><br>


Open MP
-----------------------------
#### Regras de execução:
Os testes foram executados usando 3 threads executando em um total de 60 iterações. Foram testadas diferentes combinações de `schedules` e `chunks`.

Compilando:
```c
    g++ -std=c++11 OpenMPDemoABC.cpp -fopenmp -o OpenMPDemoABC
```

Executando:
```c
    OMP_NUM_THREADS=3 ./OpenMPDemoABC
```

#### Schedule `static`:
##### Sem especificação de chunk
<div align="center"><img src="https://raw.githubusercontent.com/elc139/t3-thalissonforte/master/static.png"></div>

##### Com 4 chunks

<div align="center"><img src="https://raw.githubusercontent.com/elc139/t3-thalissonforte/master/static_4chunks.png"></div>

#### Schedule `dynamic`:
##### Sem especificação de chunk
<div align="center"><img src="https://raw.githubusercontent.com/elc139/t3-thalissonforte/master/dynamic.png"></div>

##### Com 4 chunks

<div align="center"><img src="https://raw.githubusercontent.com/elc139/t3-thalissonforte/master/dynamic_4chunks.png"></div>

#### Schedule `guided`:
##### Sem especificação de chunk
<div align="center"><img src="https://raw.githubusercontent.com/elc139/t3-thalissonforte/master/guided.png"></div>

##### Com 4 chunks

<div align="center"><img src="https://raw.githubusercontent.com/elc139/t3-thalissonforte/master/guided_4chunks.png"></div>

#### Schedule `runtime`:

<div align="center"><img src="https://raw.githubusercontent.com/elc139/t3-thalissonforte/master/runtime.png"></div>

#### Schedule `auto`:

<div align="center"><img src="https://raw.githubusercontent.com/elc139/t3-thalissonforte/master/auto.png"></div>


### Referências
- Computing. OpenMP. https://computing.llnl.gov/tutorials/openMP/.
- OpenMP: For & Scheduling. Jaka's Corner. http://jakascorner.com/blog/2016/06/omp-for-scheduling.html
