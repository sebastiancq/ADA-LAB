#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

// Se trabaja con log10 de cada cantidad: la grafica usa escala logaritmica
// y asi no hay desborde aunque se amplie n por encima de 20, mientras que 21! no cabe en long long.
double log10Factorial(int n) {
    double s = 0;
    for (int i = 2; i <= n; i++) s += log10((double)i);
    return s;
}

// Asignacion de n tareas a n puestos: n! soluciones.
double log10Asignacion(int n) { return log10Factorial(n); }

// TSP simetrico con origen fijo: (n - 1)! / 2 rutas distintas, con n >= 3.
double log10Tsp(int n) { return log10Factorial(n - 1) - log10(2.0); }

string notacion(double lg) {
    double exponente = floor(lg);
    double mantisa = pow(10.0, lg - exponente);
    ostringstream os;
    if (lg < 9) {
        os << fixed << setprecision(0) << pow(10.0, lg);
    } else {
        os << fixed << setprecision(2) << mantisa << "e" << (int)exponente;
    }
    return os.str();
}

void escribirSvg(const string& archivo, int nMin, int nMax) {
    const double ancho = 760, alto = 480;
    const double izq = 80, der = 30, arriba = 50, abajo = 60;
    const double maxLog = 20;
    auto px = [&](int n) { return izq + (n - nMin) * (ancho - izq - der) / (nMax - nMin); };
    auto py = [&](double lg) { return alto - abajo - lg * (alto - arriba - abajo) / maxLog; };

    ofstream svg(archivo);
    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << ancho << "\" height=\"" << alto
        << "\" font-family=\"Arial\" font-size=\"12\">\n";
    svg << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
    svg << "<text x=\"" << ancho / 2 << "\" y=\"28\" text-anchor=\"middle\" font-size=\"15\">"
        << "Crecimiento del espacio de soluciones (escala logaritmica)</text>\n";

    for (int e = 0; e <= maxLog; e += 2) {
        svg << "<line x1=\"" << izq << "\" y1=\"" << py(e) << "\" x2=\"" << ancho - der << "\" y2=\"" << py(e)
            << "\" stroke=\"#dddddd\"/>\n";
        svg << "<text x=\"" << izq - 8 << "\" y=\"" << py(e) + 4 << "\" text-anchor=\"end\">10^" << e << "</text>\n";
    }
    for (int n = nMin; n <= nMax; n += 2) {
        svg << "<text x=\"" << px(n) << "\" y=\"" << alto - abajo + 18 << "\" text-anchor=\"middle\">" << n << "</text>\n";
    }
    svg << "<line x1=\"" << izq << "\" y1=\"" << py(0) << "\" x2=\"" << ancho - der << "\" y2=\"" << py(0)
        << "\" stroke=\"black\"/>\n";
    svg << "<line x1=\"" << izq << "\" y1=\"" << py(0) << "\" x2=\"" << izq << "\" y2=\"" << py(maxLog)
        << "\" stroke=\"black\"/>\n";
    svg << "<text x=\"" << (izq + ancho - der) / 2 << "\" y=\"" << alto - 18
        << "\" text-anchor=\"middle\">n (tareas o ciudades)</text>\n";
    svg << "<text x=\"20\" y=\"" << (arriba + alto - abajo) / 2 << "\" text-anchor=\"middle\" transform=\"rotate(-90 20 "
        << (arriba + alto - abajo) / 2 << ")\">Numero de soluciones</text>\n";

    const string colores[2] = { "#c0392b", "#2471a3" };
    for (int serie = 0; serie < 2; serie++) {
        svg << "<polyline fill=\"none\" stroke-width=\"2.5\" stroke=\"" << colores[serie] << "\" points=\"";
        for (int n = nMin; n <= nMax; n++) {
            double lg = serie == 0 ? log10Asignacion(n) : log10Tsp(n);
            svg << px(n) << "," << py(lg) << " ";
        }
        svg << "\"/>\n";
        for (int n = nMin; n <= nMax; n++) {
            double lg = serie == 0 ? log10Asignacion(n) : log10Tsp(n);
            svg << "<circle cx=\"" << px(n) << "\" cy=\"" << py(lg) << "\" r=\"3\" fill=\"" << colores[serie] << "\"/>\n";
        }
    }
    svg << "<rect x=\"" << izq + 20 << "\" y=\"" << arriba + 10 << "\" width=\"250\" height=\"50\" fill=\"white\" stroke=\"#999999\"/>\n";
    svg << "<line x1=\"" << izq + 30 << "\" y1=\"" << arriba + 27 << "\" x2=\"" << izq + 55 << "\" y2=\"" << arriba + 27
        << "\" stroke=\"" << colores[0] << "\" stroke-width=\"2.5\"/>\n";
    svg << "<text x=\"" << izq + 62 << "\" y=\"" << arriba + 31 << "\">Asignacion de tareas: n!</text>\n";
    svg << "<line x1=\"" << izq + 30 << "\" y1=\"" << arriba + 47 << "\" x2=\"" << izq + 55 << "\" y2=\"" << arriba + 47
        << "\" stroke=\"" << colores[1] << "\" stroke-width=\"2.5\"/>\n";
    svg << "<text x=\"" << izq + 62 << "\" y=\"" << arriba + 51 << "\">TSP: (n - 1)! / 2</text>\n";
    svg << "</svg>\n";
}

int main() {
    const int nMin = 3, nMax = 20;

    cout << setw(4) << "n" << setw(22) << "Asignacion n!" << setw(22) << "TSP (n-1)!/2" << setw(12) << "Cociente\n";
    for (int n = nMin; n <= nMax; n++) {
        double a = log10Asignacion(n);
        double t = log10Tsp(n);
        cout << setw(4) << n << setw(22) << notacion(a) << setw(22) << notacion(t)
             << setw(11) << (int)lround(pow(10.0, a - t)) << "\n";
    }

    escribirSvg("grafica_crecimiento.svg", nMin, nMax);
    return 0;
}
