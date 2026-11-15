#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

struct Lugar {
    string nombre;
    double lat;
    double lon;
};

// Coordenadas de OpenStreetMap, consultadas el 12/11/2026. El punto 0 es el
// inicio y el fin de la visita.
const vector<Lugar> lugares = {
    { "Plaza de Armas", -16.3988333, -71.5369705 },
    { "Monasterio de Santa Catalina", -16.3951903, -71.5365593 },
    { "Casa del Moral", -16.3969616, -71.5377816 },
    { "Puente Bolognesi", -16.3976808, -71.5416618 },
    { "Mirador de Yanahuara", -16.3874749, -71.5417491 },
    { "Plaza de Cayma", -16.3814836, -71.5455830 },
    { "Mirador de Carmen Alto", -16.3699276, -71.5366260 },
    { "Parque Selva Alegre", -16.3896022, -71.5309591 },
    { "Mercado San Camilo", -16.4030888, -71.5350033 },
    { "Molino de Sabandia", -16.4567393, -71.4998552 },
    { "Mansion del Fundador", -16.4639531, -71.5564225 },
    { "Mirador de Sachaca", -16.4262711, -71.5674758 }
};

// Distancias por calles en auto, en km, obtenidas con OSRM sobre OpenStreetMap
// en la misma fecha. La fila es el origen y la columna el destino; por los
// sentidos de las calles, ir de i a j no mide lo mismo que volver de j a i.
const vector<vector<double>> calleKm = {
    { 0.00, 1.27, 1.11, 0.51, 2.13, 2.93, 4.42, 2.12, 0.83, 9.30, 8.78, 5.71 },
    { 1.50, 0.00, 0.69, 1.03, 1.44, 2.41, 3.73, 1.34, 1.62, 10.12, 9.60, 6.21 },
    { 1.05, 1.06, 0.00, 0.58, 1.83, 2.80, 4.12, 2.10, 1.17, 9.64, 9.15, 5.76 },
    { 2.21, 1.99, 1.80, 0.00, 1.61, 2.41, 3.91, 3.03, 2.33, 10.80, 10.70, 5.20 },
    { 2.57, 2.04, 1.85, 1.97, 0.00, 1.31, 2.55, 2.74, 2.69, 11.16, 10.54, 5.94 },
    { 3.64, 3.13, 2.94, 3.06, 1.03, 0.00, 2.56, 2.74, 3.76, 12.23, 11.32, 6.18 },
    { 5.05, 4.52, 4.33, 4.45, 2.56, 2.49, 0.00, 3.49, 5.17, 13.36, 13.02, 8.67 },
    { 2.10, 1.67, 1.75, 1.96, 2.23, 2.97, 3.65, 0.00, 2.44, 10.41, 10.33, 7.06 },
    { 1.11, 1.65, 1.51, 1.62, 3.09, 3.94, 5.38, 2.25, 0.00, 8.44, 8.36, 6.48 },
    { 10.03, 10.58, 10.43, 10.54, 12.08, 12.69, 14.32, 10.62, 8.84, 0.00, 8.40, 12.57 },
    { 8.87, 9.43, 9.27, 9.39, 10.92, 11.53, 13.17, 10.17, 8.39, 8.40, 0.00, 7.23 },
    { 6.24, 6.50, 6.30, 6.13, 6.10, 6.36, 8.40, 7.54, 6.17, 12.17, 5.89, 0.00 }
};

// Tiempo de manejo en minutos para los mismos pares.
const vector<vector<double>> calleMin = {
    { 0.0, 2.8, 2.6, 1.1, 4.3, 5.4, 8.6, 4.7, 2.0, 17.6, 14.8, 11.0 },
    { 3.9, 0.0, 1.9, 2.5, 3.2, 4.6, 7.2, 2.8, 4.1, 19.5, 16.9, 12.1 },
    { 2.7, 2.5, 0.0, 1.2, 4.1, 5.4, 8.1, 4.1, 2.9, 18.3, 15.7, 10.7 },
    { 4.8, 4.8, 4.3, 0.0, 3.2, 4.4, 7.6, 6.4, 5.0, 20.4, 17.6, 9.9 },
    { 5.9, 4.9, 4.3, 4.3, 0.0, 3.0, 5.2, 5.1, 6.1, 21.5, 18.7, 11.3 },
    { 6.8, 6.6, 6.0, 5.9, 2.5, 0.0, 5.3, 5.3, 7.0, 22.4, 18.8, 11.2 },
    { 10.4, 9.4, 8.8, 8.8, 5.5, 5.0, 0.0, 6.6, 10.6, 25.3, 23.2, 16.2 },
    { 4.5, 3.5, 3.7, 3.7, 4.5, 5.7, 7.0, 0.0, 5.1, 19.7, 18.0, 12.9 },
    { 2.6, 3.5, 3.2, 3.7, 6.6, 7.2, 10.6, 4.8, 0.0, 15.7, 14.1, 12.5 },
    { 17.5, 18.4, 18.2, 18.6, 21.2, 21.7, 25.5, 20.2, 16.6, 0.0, 13.9, 22.2 },
    { 14.9, 15.8, 15.6, 16.0, 18.6, 19.2, 22.9, 17.7, 14.1, 13.8, 0.0, 13.4 },
    { 11.5, 12.0, 11.4, 11.0, 11.0, 10.9, 15.4, 13.5, 11.5, 21.4, 10.8, 0.0 }
};

// Distancia en linea recta sobre la superficie terrestre, formula de haversine, en km.
double haversine(const Lugar& a, const Lugar& b) {
    const double radioTierra = 6371.0;
    const double rad = M_PI / 180.0;
    double dLat = (b.lat - a.lat) * rad;
    double dLon = (b.lon - a.lon) * rad;
    double h = sin(dLat / 2) * sin(dLat / 2) +
               cos(a.lat * rad) * cos(b.lat * rad) * sin(dLon / 2) * sin(dLon / 2);
    return 2 * radioTierra * asin(sqrt(h));
}

// Suma los tramos en el orden de la ruta, respetando el sentido de cada uno.
double costoRuta(const vector<vector<double>>& dist, const vector<int>& ruta) {
    double costo = 0;
    for (size_t i = 0; i + 1 < ruta.size(); i++) costo += dist[ruta[i]][ruta[i + 1]];
    costo += dist[ruta.back()][ruta[0]];
    return costo;
}

vector<int> nearestNeighbor(const vector<vector<double>>& dist, int inicio) {
    int n = dist.size();
    vector<bool> visitado(n, false);
    vector<int> ruta = { inicio };
    visitado[inicio] = true;
    for (int paso = 1; paso < n; paso++) {
        int actual = ruta.back();
        int masCercano = -1;
        double mejorDist = 1e18;
        for (int c = 0; c < n; c++) {
            if (!visitado[c] && dist[actual][c] < mejorDist) {
                mejorDist = dist[actual][c];
                masCercano = c;
            }
        }
        ruta.push_back(masCercano);
        visitado[masCercano] = true;
    }
    return ruta;
}

// Invertir un tramo cambia el sentido de todas sus aristas internas, y con una
// matriz asimetrica eso tambien cambia su costo. Por eso cada inversion se
// evalua recalculando la ruta completa, como en el ejemplo de la guia.
int dosOpt(const vector<vector<double>>& dist, vector<int>& ruta) {
    int n = ruta.size();
    int movimientos = 0;
    bool mejora = true;
    while (mejora) {
        mejora = false;
        for (int i = 1; i < n - 1; i++) {
            for (int j = i + 1; j < n; j++) {
                vector<int> nueva = ruta;
                reverse(nueva.begin() + i, nueva.begin() + j + 1);
                if (costoRuta(dist, nueva) < costoRuta(dist, ruta) - 1e-9) {
                    ruta = nueva;
                    movimientos++;
                    mejora = true;
                }
            }
        }
    }
    return movimientos;
}

// Rota el ciclo para que empiece en el punto de partida, la Plaza de Armas.
vector<int> desdeOrigen(const vector<int>& ruta) {
    int n = ruta.size();
    int p = find(ruta.begin(), ruta.end(), 0) - ruta.begin();
    vector<int> r(n);
    for (int k = 0; k < n; k++) r[k] = ruta[(p + k) % n];
    return r;
}

// Held y Karp con reconstruccion de la ruta. Usa dist[j][k] en el sentido de
// j hacia k, de modo que tambien resuelve el caso asimetrico.
double optimoExacto(const vector<vector<double>>& dist, vector<int>& rutaOptima) {
    int n = dist.size();
    int total = 1 << n;
    vector<vector<double>> mejor(total, vector<double>(n, 1e18));
    vector<vector<int>> previo(total, vector<int>(n, -1));
    mejor[1][0] = 0;
    for (int S = 1; S < total; S++) {
        if (!(S & 1)) continue;
        for (int j = 0; j < n; j++) {
            if (!(S & (1 << j)) || mejor[S][j] >= 1e18) continue;
            for (int k = 0; k < n; k++) {
                if (S & (1 << k)) continue;
                int T = S | (1 << k);
                if (mejor[S][j] + dist[j][k] < mejor[T][k]) {
                    mejor[T][k] = mejor[S][j] + dist[j][k];
                    previo[T][k] = j;
                }
            }
        }
    }
    double resultado = 1e18;
    int ultimo = -1;
    for (int j = 1; j < n; j++) {
        if (mejor[total - 1][j] + dist[j][0] < resultado) {
            resultado = mejor[total - 1][j] + dist[j][0];
            ultimo = j;
        }
    }
    rutaOptima = { 0 };
    vector<int> resto;
    int S = total - 1;
    while (ultimo != 0) {
        resto.push_back(ultimo);
        int antes = previo[S][ultimo];
        S ^= (1 << ultimo);
        ultimo = antes;
    }
    reverse(resto.begin(), resto.end());
    rutaOptima.insert(rutaOptima.end(), resto.begin(), resto.end());
    return resultado;
}

vector<int> invertida(const vector<int>& ruta) {
    vector<int> r = ruta;
    reverse(r.begin() + 1, r.end());
    return r;
}

void mostrarRuta(const vector<int>& ruta) {
    for (int c : ruta) cout << c << "-";
    cout << ruta[0];
}

void mostrarItinerario(const vector<int>& ruta) {
    double km = 0, minutos = 0;
    int n = ruta.size();
    for (int i = 0; i < n; i++) {
        int a = ruta[i], b = ruta[(i + 1) % n];
        km += calleKm[a][b];
        minutos += calleMin[a][b];
        cout << setw(3) << i + 1 << ". " << left << setw(29) << lugares[a].nombre << "a "
             << setw(29) << lugares[b].nombre << right << setw(6) << calleKm[a][b] << " km"
             << setw(6) << setprecision(1) << calleMin[a][b] << " min" << setprecision(2)
             << setw(7) << km << " km\n";
    }
}

// Resuelve con una matriz: NN desde la plaza, NN + 2-opt y NN + 2-opt desde los 12 lugares.
// Devuelve la mejor ruta de los 12 inicios, escrita desde la plaza.
vector<int> resolver(const vector<vector<double>>& dist, const string& titulo, double& optimo) {
    int n = dist.size();
    vector<int> rutaOptima;
    optimo = optimoExacto(dist, rutaOptima);

    vector<int> nn = nearestNeighbor(dist, 0);
    vector<int> mejorada = nn;
    int movimientos = dosOpt(dist, mejorada);
    double cNN = costoRuta(dist, nn), c2 = costoRuta(dist, mejorada);

    double mejorTodos = 1e18;
    vector<int> rutaMejor;
    int aciertos = 0;
    for (int inicio = 0; inicio < n; inicio++) {
        vector<int> r = nearestNeighbor(dist, inicio);
        dosOpt(dist, r);
        double c = costoRuta(dist, r);
        if (c < optimo + 1e-9) aciertos++;
        if (c < mejorTodos - 1e-9) {
            mejorTodos = c;
            rutaMejor = desdeOrigen(r);
        }
    }

    cout << "\n" << titulo << "\n";
    cout << "Nearest Neighbor desde la plaza: " << cNN << " km, ";
    mostrarRuta(nn);
    cout << "\nNN + 2-opt: " << c2 << " km, movimientos aceptados " << movimientos << ", ";
    mostrarRuta(mejorada);
    cout << "\nNN + 2-opt desde los 12 lugares: mejor " << mejorTodos << " km, llegan al optimo "
         << aciertos << " de " << n << "\nOptimo exacto (Held-Karp): " << optimo << " km, ";
    mostrarRuta(rutaOptima);
    cout << "\nDiferencia con el optimo: NN " << 100.0 * (cNN - optimo) / optimo << " %, NN + 2-opt "
         << 100.0 * (c2 - optimo) / optimo << " %\n";
    return rutaMejor;
}

int main() {
    int n = lugares.size();
    vector<vector<double>> recta(n, vector<double>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) recta[i][j] = haversine(lugares[i], lugares[j]);

    cout << fixed << setprecision(2);
    cout << "Lugares de la visita\n";
    for (int i = 0; i < n; i++) {
        cout << setw(3) << i << "  " << left << setw(30) << lugares[i].nombre << right
             << setprecision(4) << setw(10) << lugares[i].lat << setw(10) << lugares[i].lon << "\n";
    }
    cout << setprecision(2);

    // Cuanto mas largo es ir por calles que en linea recta, y que tan distinta es la ida de la vuelta.
    double sumaFactor = 0, minFactor = 1e18, maxFactor = 0, maxAsim = 0;
    int pares = 0, asimetricos = 0, iMax = 0, jMax = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            double f = calleKm[i][j] / recta[i][j];
            sumaFactor += f;
            minFactor = min(minFactor, f);
            maxFactor = max(maxFactor, f);
            pares++;
            if (i < j) {
                double dif = fabs(calleKm[i][j] - calleKm[j][i]);
                if (dif > 0.005) asimetricos++;
                if (dif > maxAsim) { maxAsim = dif; iMax = i; jMax = j; }
            }
        }
    }
    cout << "\nFactor calles / linea recta en los " << pares << " pares: promedio " << sumaFactor / pares
         << ", minimo " << minFactor << ", maximo " << maxFactor << "\n";
    // Medido solo desde la plaza hacia cada lugar, para separar trayectos cortos y largos.
    double sumaCorto = 0, sumaLargo = 0, rectaCorto = 0, rectaLargo = 0;
    for (int j = 1; j < n; j++) {
        if (recta[0][j] < 2) { sumaCorto += calleKm[0][j]; rectaCorto += recta[0][j]; }
        else { sumaLargo += calleKm[0][j]; rectaLargo += recta[0][j]; }
    }
    cout << "Desde la plaza: factor " << sumaCorto / rectaCorto << " hasta los lugares a menos de 2 km\n"
         << "Desde la plaza: factor " << sumaLargo / rectaLargo << " hasta los lugares a 2 km o mas\n";
    cout << "Pares con ida y vuelta distintas: " << asimetricos << " de " << n * (n - 1) / 2 << "\n"
         << "Mayor diferencia: " << maxAsim << " km entre " << iMax << " y " << jMax
         << ", " << calleKm[iMax][jMax] << " km de ida y " << calleKm[jMax][iMax] << " km de vuelta\n";

    double optRecta = 0, optCalle = 0;
    vector<int> rutaRecta = resolver(recta, "Matriz en linea recta", optRecta);
    vector<int> rutaCalle = resolver(calleKm, "Matriz por calles", optCalle);

    cout << "\nItinerario por calles de la mejor ruta de NN + 2-opt\n";
    mostrarItinerario(rutaCalle);
    cout << "Tiempo total de manejo: " << setprecision(1) << costoRuta(calleMin, rutaCalle)
         << " min" << setprecision(2) << "\n";

    // La ruta planificada en linea recta se recorre en realidad por calles.
    double recorrida = costoRuta(calleKm, rutaRecta);
    double alReves = costoRuta(calleKm, invertida(rutaRecta));
    cout << "\nRuta de linea recta recorrida por calles: " << recorrida << " km en su sentido, "
         << alReves << " km en sentido contrario\n";
    cout << "Frente al optimo por calles: " << 100.0 * (min(recorrida, alReves) - optCalle) / optCalle
         << " % mas larga en el mejor sentido\n";
    cout << "Optimo por calles en sentido contrario: " << costoRuta(calleKm, invertida(rutaCalle)) << " km\n";

    return 0;
}
