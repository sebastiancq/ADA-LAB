#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <random>

using namespace std;

void insertionSort(vector<string> &arr, long long &comparaciones, long long &movimientos)
{
    int n = arr.size();

    for (int i = 1; i < n; i++)
    {
        string key = arr[i];
        int j = i - 1;

        while (j >= 0)
        {
            comparaciones++;

            if (arr[j] > key)
            {
                arr[j + 1] = arr[j];
                movimientos++;
                j--;
            }
            else
            {
                break;
            }
        }

        arr[j + 1] = key;
    }
}

int main()
{
    vector<string> ordenado = {
    "Aaron", "Abel", "Abigail", "Abraham", "Ada",
    "Adan", "Adela", "Adelaida", "Adelina", "Adolfo",
    "Adrian", "Adriana", "Agustin", "Agustina", "Aida",
    "Aileen", "Ainhoa", "Alan", "Alba", "Alberto",
    "Alejandra", "Alejandro", "Alex", "Alexandra", "Alexis",
    "Alfonso", "Alfredo", "Alicia", "Alma", "Alvaro",
    "Barbara", "Beatriz", "Belen", "Belinda", "Benjamin",
    "Berenice", "Bernardo", "Bianca", "Blanca", "Bruno",
    "Camila", "Camilo", "Candela", "Candelaria", "Carla",
    "Carlos", "Carlota", "Carolina", "Carmen", "Casandra",
    "Catalina", "Cecilia", "Celeste", "Cesar", "Clara",
    "Claudia", "Claudio", "Clemente", "Constanza", "Cristian",
    "Dafne", "Dalia", "Damian", "Dana", "Daniel",
    "Daniela", "Dario", "David", "Debora", "Delfina",
    "Demetrio", "Diana", "Diego", "Dolores", "Domingo",
    "Edgar", "Edgardo", "Edith", "Edmundo", "Eduardo",
    "Efrain", "Elena", "Eleonora", "Elias", "Eliana",
    "Elisa", "Elizabeth", "Eloisa", "Elsa", "Elvira",
    "Emanuel", "Emilia", "Emiliano", "Emilio", "Enrique",
    "Fabian", "Fabiola", "Fatima", "Federico", "Felipe",
    "Fernanda", "Fernando", "Fiorella", "Florencia", "Francisco",
    "Gabriel", "Gabriela", "Gael", "Gala", "Gaspar",
    "Genaro", "Genesis", "Gerardo", "German", "Gervasio",
    "Gilda", "Gina", "Gloria", "Gonzalo", "Gustavo",
    "Hadrian", "Haidee", "Harold", "Hector", "Helena",
    "Helga", "Hernan", "Hilario", "Homero", "Hugo",
    "Ian", "Ignacio", "Iker", "Iliana", "Ines",
    "Ingrid", "Irene", "Iris", "Isabel", "Ivan",
    "Jacinto", "Jacob", "Jacqueline", "Jaime", "Jana",
    "Javier", "Jazmin", "Jennifer", "Jeremias", "Jeronimo",
    "Jesica", "Jesus", "Jimena", "Joaquin", "Joel",
    "Jonathan", "Jorge", "Jose", "Josefina", "Juan",
    "Karen", "Karina", "Karla", "Katherine", "Kevin",
    "Lara", "Laura", "Lautaro", "Leandro", "Leia",
    "Leonardo", "Leonel", "Leticia", "Lia", "Liam",
    "Lidia", "Liliana", "Lina", "Lionel", "Lisandro",
    "Lola", "Lorena", "Lourdes", "Lucas", "Lucia",
    "Magdalena", "Maia", "Maite", "Malena", "Manuel",
    "Manuela", "Mara", "Marcela", "Marcelo", "Marco",
    "Marcos", "Margarita", "Maria", "Mariana", "Mariano",
    "Maribel", "Marina", "Mario", "Marisol", "Marta",
    "Martin", "Martina", "Mateo", "Matias", "Mauricio",
    "Nadia", "Nahuel", "Nancy", "Naomi", "Natalia",
    "Nerea", "Nicolas", "Noelia", "Nora", "Nuria",
    "Octavio", "Ofelia", "Olga", "Omar", "Oscar",
    "Pablo", "Paloma", "Pamela", "Paola", "Patricia",
    "Paula", "Paulina", "Pedro", "Penelope", "Pilar",
    "Rafael", "Rafaela", "Ramiro", "Ramon", "Raquel",
    "Raul", "Rebeca", "Regina", "Renata", "Renato",
    "Rene", "Ricardo", "Rita", "Roberto", "Rocio",
    "Rodolfo", "Rodrigo", "Romina", "Rosa", "Ruben",
    "Sabrina", "Salvador", "Samantha", "Samuel", "Sandra",
    "Santiago", "Sara", "Sebastian", "Selena", "Sergio",
    "Silvana", "Silvia", "Simon", "Sofia", "Susana",
    "Tamara", "Tania", "Tatiana", "Teo", "Teodoro",
    "Teresa", "Thiago", "Tomas", "Trinidad", "Tristan",
    "Ulises", "Uma", "Ursula",
    "Valentin", "Valentina", "Valeria", "Vanessa",
    "Vera", "Veronica", "Victor", "Victoria",
    "Walter", "William",
    "Xander", "Ximena",
    "Yago", "Yamila", "Yolanda",
    "Zaira", "Zoe"
};



    vector<string> inverso = ordenado;
    reverse(inverso.begin(), inverso.end());

    vector<string> aleatorio = ordenado;

    random_device rd;
    mt19937 generador(rd());

    shuffle(aleatorio.begin(), aleatorio.end(), generador);

    // CASO 1: ORDENADO
    long long comparacionesOrdenado = 0;
    long long movimientosOrdenado = 0;

    // chrono::high_resolution_clock::time_point inicioOrdenado = chrono::high_resolution_clock::now();
    auto inicioOrdenado = chrono::high_resolution_clock::now();

    insertionSort(
        ordenado,
        comparacionesOrdenado,
        movimientosOrdenado);

    // chrono::high_resolution_clock::time_point finOrdenado = chrono::high_resolution_clock::now();
    auto finOrdenado = chrono::high_resolution_clock::now();

    // chrono::microseconds tiempoOrdenado = chrono::duration_cast<chrono::microseconds>(finOrdenado - inicioOrdenado);
    auto tiempoOrdenado = chrono::duration_cast<chrono::microseconds>(finOrdenado - inicioOrdenado);

    // CASO 2: INVERSO
    long long comparacionesInverso = 0;
    long long movimientosInverso = 0;

    auto inicioInverso = chrono::high_resolution_clock::now();

    insertionSort(inverso, comparacionesInverso, movimientosInverso);

    auto finInverso = chrono::high_resolution_clock::now();

    auto tiempoInverso = chrono::duration_cast<chrono::microseconds>(finInverso - inicioInverso);

    // CASO 3: ALEATORIO
    long long comparacionesAleatorio = 0;
    long long movimientosAleatorio = 0;

    auto inicioAleatorio = chrono::high_resolution_clock::now();

    insertionSort(aleatorio, comparacionesAleatorio, movimientosAleatorio);

    auto finAleatorio = chrono::high_resolution_clock::now();

    auto tiempoAleatorio = chrono::duration_cast<chrono::microseconds>(finAleatorio - inicioAleatorio);



    cout << "LISTA YA ORDENADA" << endl;
    cout << "Tiempo: " << tiempoOrdenado.count()
         << " microsegundos" << endl;
    cout << "Comparaciones: " << comparacionesOrdenado << endl;
    cout << "Movimientos: " << movimientosOrdenado << endl;

    cout << "\nLISTA EN ORDEN INVERSO" << endl;
    cout << "Tiempo: " << tiempoInverso.count()
         << " microsegundos" << endl;
    cout << "Comparaciones: " << comparacionesInverso << endl;
    cout << "Movimientos: " << movimientosInverso << endl;

    cout << "\nLISTA ALEATORIA" << endl;
    cout << "Tiempo: " << tiempoAleatorio.count()
         << " microsegundos" << endl;
    cout << "Comparaciones: " << comparacionesAleatorio << endl;
    cout << "Movimientos: " << movimientosAleatorio << endl;

    return 0;
}