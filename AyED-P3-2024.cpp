#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <bitset>

#define INFI  999

using namespace std;

struct Coordenada {
    int x, y;
    Coordenada(int a, int b) : x(a), y(b) {}
};

struct Mancha {
    Coordenada coordenada_central;
    vector<Coordenada> coordenadas;
    Mancha() : coordenada_central(0,0) {}
};

struct Estado {
    vector<int> secuencia;
    int nodo_actual;
    int costo_acumulado;
};

int calcularDistancia(Coordenada c1, Coordenada c2) {
    return sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2));
}

vector<vector<int>> leerImagen(string nombre_archivo) {
    ifstream archivo(nombre_archivo);
    if (!archivo) {
        cerr << "Error al abrir el archivo." << endl;
        exit(1);
    }

    vector<vector<int>> imagen;
    string linea;
    while (getline(archivo, linea)) {
        vector<int> fila;
        for (char c : linea) {
            fila.push_back(c - '0'); //Convierte de caracter a entero -- ASCII '0' = 48; '1' = 49; 'x' = 120
        }
        imagen.push_back(fila);
    }
    return imagen;
}

bool esManchaDeMaleza(vector<bitset<3>> muestra) {
    bitset<3> filtro(111);
    bool a = false;
    for(int i = 0; i <3; i++){
        a = (filtro == (muestra[i]&filtro)); //Compara cada fila despues de realizar la operacion de conjunto
        if(!a) return false;
    }
    return a;
}

vector<Mancha> detectarManchas(vector<vector<int>> imagen) {
    vector<Mancha> manchas;
    int filas = imagen.size(), columnas = imagen[0].size();

    for (int i = 1; i < filas - 1; ++i) {
        for (int j = 1; j < columnas - 1; ++j) {
            vector<bitset<3>> muestra(3);
            for (int k = 0; k < 3; ++k) {
                for (int l = 0; l < 3; ++l) {
                    if(imagen[i - 1 + k][j - 1 + l] < 2){
                        muestra[k].set(l, imagen[i - 1 + k][j - 1 + l]); //Carga como muestra el cuadrante de 3x3 centrado en (i,j)
                    } 
                    else{
                        muestra[k].set(l,0); 
                    }
                }
            }

            if (esManchaDeMaleza(muestra)) {
                Mancha mancha;
                mancha.coordenada_central = Coordenada(i, j);
                manchas.push_back(mancha);
            }
        }
    }
    return manchas;
}

vector<vector<int>> crearGrafo(vector<Mancha> manchas) {
    int n = manchas.size();
    vector<vector<int>> grafo(n, vector<int>(n, 0)); //Primer parametro: tamaño; Segundo Parametro: valor a almacenar

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            grafo[i][j] = grafo[j][i] = calcularDistancia(manchas[i].coordenada_central, manchas[j].coordenada_central);
        }
    }
    return grafo;
}

void imprimirGrafo(vector<vector<int>> grafo) {
    int n = grafo.size();
    
    //Busca el ancho máximo de cualquier elemento en la matriz
    int max_width = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int width = to_string(grafo[i][j]).length();
            if (width > max_width) {
                max_width = width;
            }
        }
    }

    //Imprime la matriz con el ancho determinado
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << setw(max_width + 2) << grafo[i][j];  //Añade espacio extra para separar columnas
        }
        cout << endl;
    }
}

vector<vector<Coordenada>> encontrarBarreras(vector<vector<int>> imagen) {
    vector<vector<Coordenada>> barreras;
    int filas = imagen.size();
    int columnas = imagen[0].size();

    //Busca barreras horizontales
    for (int i = 0; i < filas; ++i) {
        int inicio = -1;
        for (int j = 0; j < columnas; ++j) {
            if (imagen[i][j] == 72) { //Busca 'x', resulta de hacer 'x' - '0' = 120 - 48 = 72
                if (inicio == -1) {
                    inicio = j;
                }
            } 
            else {
                if (inicio != -1 && j - inicio >= 4) {
                    vector<Coordenada> barrera;
                    barrera.push_back(Coordenada(i, inicio)); //Guarda el inicio de la barrera
                    barrera.push_back(Coordenada(i, j - 1)); //Guarda el final de la barrera
                    barreras.push_back(barrera); //Guarda la barrera
                }
                inicio = -1;
            }
        }
        if (inicio != -1 && columnas - inicio >= 4) { //Agrega barreras que se extiendan hasta el ultimo pixel
            vector<Coordenada> barrera;
            barrera.push_back(Coordenada(i, inicio)); 
            barrera.push_back(Coordenada(i, columnas - 1));
            barreras.push_back(barrera); 
        }
    }

    //Busca barreras verticales
    for (int j = 0; j < columnas; ++j) {
        int inicio = -1;
        for (int i = 0; i < filas; ++i) {
            if (imagen[i][j] == 72) {
                if (inicio == -1) {
                    inicio = i;
                }
            } else {
                if (inicio != -1 && i - inicio >= 4) {
                    vector<Coordenada> barrera;
                    barrera.push_back(Coordenada(inicio, j)); 
                    barrera.push_back(Coordenada(i - 1, j)); 
                    barreras.push_back(barrera); 
                }
                inicio = -1;
            }
        }
        if (inicio != -1 && filas - inicio >= 4) {
            vector<Coordenada> barrera;
            barrera.push_back(Coordenada(inicio, j));
            barrera.push_back(Coordenada(filas - 1, j));
            barreras.push_back(barrera);
        }
    }

    return barreras;
}

bool intersecta(Coordenada p1, Coordenada p2, Coordenada p3, Coordenada p4) {
    int dx12 = p2.x - p1.x;
    int dy12 = p2.y - p1.y;
    int dx34 = p4.x - p3.x;
    int dy34 = p4.y - p3.y;

    int den = dx12 * dy34 - dy12 * dx34;
    if (den == 0) {
        return false; //Los segmentos son paralelos o colineales
    }

    int vx = p3.x - p1.x;
    int vy = p3.y - p1.y;

    double k1 = (vx * dy34 - vy * dx34) / (double)den;
    double k2 = (vx * dy12 - vy * dx12) / (double)den;

    return (k1 >= 0 && k1 <= 1 && k2 >= 0 && k2 <= 1);
}

void actualizarGrafoPorBarreras(vector<vector<int>>& grafo, vector<Mancha> manchas, vector<vector<Coordenada>> barreras) {
    int n = manchas.size();

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            for (auto barrera : barreras) {
                Coordenada p1 = manchas[i].coordenada_central;
                Coordenada p2 = manchas[j].coordenada_central;
                Coordenada p3 = barrera[0];
                Coordenada p4 = barrera[1];

                if (intersecta(p1, p2, p3, p4)) {
                    grafo[i][j] = grafo[j][i] = INFI;
                    break; //No es necesario verificar con otras barreras si ya se encontró una intersección
                }
            }
        }
    }
}

void imprimirCoordenadasManchas(vector<Mancha> manchas) {
    for (int i = 0; i < manchas.size(); ++i) {
        cout << "Mancha " << i + 1 << ": (" << manchas[i].coordenada_central.x + 1  << ", " << manchas[i].coordenada_central.y + 1 << ")" << endl;
    }
}

int calcularCostoCiclo(vector<vector<int>> grafo, vector<int> ciclo) {
    int costo_total = 0;
    for (int i = 0; i < ciclo.size() - 1; ++i) {
        costo_total += grafo[ciclo[i]][ciclo[i+1]];
    }
    return costo_total;
}

vector<int> buscarCicloHamiltoniano(vector<vector<int>> grafo, int indice_coordenada_inicial) {
    int n = grafo.size();
    vector<int> mejor_ciclo;
    int costo_mejor_ciclo = INFI;

    //Define el estado inicial de la cola
    queue<Estado> cola;
    Estado estado_inicial;
    estado_inicial.nodo_actual = indice_coordenada_inicial; //Inicia desde el nodo correspondiente a la coordenada inicial (1,1)
    estado_inicial.costo_acumulado = 0;
    estado_inicial.secuencia.push_back(indice_coordenada_inicial);
    cola.push(estado_inicial); //Encolo ese estado

    while (!cola.empty()) {
        Estado estado_actual = cola.front();
        cola.pop();

        if (estado_actual.secuencia.size() == n) { //Verifica si se ha visitado todos los nodos
            estado_actual.secuencia.push_back(indice_coordenada_inicial); //Vuelve al nodo inicial
            int costo_ciclo = calcularCostoCiclo(grafo, estado_actual.secuencia);
            if (costo_ciclo < costo_mejor_ciclo) {
                mejor_ciclo = estado_actual.secuencia;
                costo_mejor_ciclo = costo_ciclo;
            }
        } 
        else {
            for (int i = 0; i < n; ++i) {
                if (i != estado_actual.nodo_actual) { //Saltea el nodo donde se inicia el ciclo
                    if (find(estado_actual.secuencia.begin(), estado_actual.secuencia.end(), i) == estado_actual.secuencia.end()) { //Verifica si el nodo no ha sido visitado antes
                        Estado nuevo_estado = estado_actual;
                        nuevo_estado.nodo_actual = i;
                        nuevo_estado.costo_acumulado += grafo[estado_actual.nodo_actual][i];
                        nuevo_estado.secuencia.push_back(i);
                        cola.push(nuevo_estado); //Lo añado a los visitados
                    }
                }
            }
        }
    }

    if (mejor_ciclo.size() != n + 1) { //+1 porque el ciclo debe volver al nodo inicial
        return vector<int>();
    } 
    else {
        return mejor_ciclo;
    }
}

void agregarCoordenadaInicial(vector<Mancha>& manchas, Coordenada coordenada_inicial) {
    Mancha coordenada_inicial_mancha;
    coordenada_inicial_mancha.coordenada_central = coordenada_inicial;
    manchas.insert(manchas.begin(), coordenada_inicial_mancha);
}

void imprimirCicloHamiltoniano(vector<int> ciclo_hamiltoniano, vector<Mancha> manchas, vector<vector<int>> grafo) {
    if (ciclo_hamiltoniano.empty()) {
        cout << "No se encontro un ciclo Hamiltoniano." << endl;
        return;
    }

    cout << "Ciclo Hamiltoniano: ";
    for (int i = 0; i < ciclo_hamiltoniano.size(); ++i) {
        cout << "(" << manchas[ciclo_hamiltoniano[i]].coordenada_central.x + 1 << ", " << manchas[ciclo_hamiltoniano[i]].coordenada_central.y + 1 << ") ";
    }
    cout << endl;
    int distancia_total = 0;
    for (int i = 0; i < ciclo_hamiltoniano.size() - 1; ++i) {
        distancia_total += grafo[ciclo_hamiltoniano[i]][ciclo_hamiltoniano[i + 1]];
    }
    distancia_total += grafo[ciclo_hamiltoniano.back()][ciclo_hamiltoniano.front()]; //Añade la distancia desde el último nodo al primer nodo para cerrar el ciclo
    cout << "Distancia total recorrida: " << distancia_total << endl;
}

int main() {
    string nombre_archivo = "Prueba1.txt";
    vector<vector<int>> imagen = leerImagen(nombre_archivo);
    vector<Mancha> manchas = detectarManchas(imagen);
    vector<vector<int>> grafo = crearGrafo(manchas);
    vector<vector<Coordenada>> barreras = encontrarBarreras(imagen);
    
    cout << "Orden y Coordenadas de las Manchas:" << endl;
    imprimirCoordenadasManchas(manchas);
    cout << endl;
    cout << endl;

    cout << "Grafo sin considerar las barreras" << endl;
    imprimirGrafo(grafo);
    cout << endl;
    cout << endl;

    actualizarGrafoPorBarreras(grafo, manchas, barreras);
    
    cout << "Grafo considerando las barreras" << endl;
    imprimirGrafo(grafo);
    cout << endl;
    cout << endl;

    Coordenada coordenada_inicial(0, 0);
    agregarCoordenadaInicial(manchas, coordenada_inicial);

    grafo = crearGrafo(manchas); //Se modifica el vector de manchas para agregar la coordenada inicial
    actualizarGrafoPorBarreras(grafo, manchas, barreras);
    vector<int> ciclo_hamiltoniano = buscarCicloHamiltoniano(grafo, 0);

    imprimirCicloHamiltoniano(ciclo_hamiltoniano, manchas, grafo);
    cout << endl;
    cout << endl;
    return 0;
}
