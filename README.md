# Productor-Consumidor



## Referencias

1. Basado en el tutorial de [chuidiang.org](http://www.chuidiang.org/clinux/ipcs/mem_comp.php)

2. Basado en el tutorial de [C.U. Elena y S.R Ranquel](https://www2.infor.uva.es/~cllamas/concurr/pract97/rsantos/index.html)

## Requerimientos

1. Instalar gcc y make

    ```bash
    sudo apt install gcc make
    ```

## Compilación 

1. En la raiz del proyecto ejecutar el comando make para crear los ejecutables


## Ejecución

1. Para ejecutar el inicializador:
    
    ```bash
    cd src
    ./inicializador <Buffer> <Tamaño>
    ```
2. Para ejecutar el productor:

    ```bash
    cd src
    ./productor <Buffer> <media>
    ```
3. Para ejecutar el consumidor:

    ```bash
    cd src
    ./consumidor <Buffer> <media> <mode>
    ```
3. Para ejecutar el productor:

    ```bash
    cd src
    ./finalizador <Buffer>
    ```


## Contribuyendo
[Juan Esquivel Rojas](https://github.com/JDesq)

[David Román Muñoz](https://github.com/romux09)