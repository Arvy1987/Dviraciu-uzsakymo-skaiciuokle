#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;
int atidarytiFaila(string kategorijos[], string pavadinimai[],double kainos[], int kiekiai[]);
double skaiciuotiEilutesSuma(double kaina, int kiekis);
double skaiciuotiNuolaida(double eilutesSuma, int kiekis);
double skaiciuotiKategorijosSuma(const string kategorijos[],const double kainos[], const int kiekiai[],int prekiuKiekis, string kategorija);
void isvestiAtaskaita(ostream& isvestis,
                     const string kategorijos[], const string pavadinimai[],
                     const double kainos[], const int kiekiai[], int prekiuKiekis);
int main() {
    const int MAX_PREKIU=100;
    string kategorijos[MAX_PREKIU];
    string pavadinimai[MAX_PREKIU];
    double kainos[MAX_PREKIU];
    int kiekiai[MAX_PREKIU];

    int prekiuKiekis = atidarytiFaila(kategorijos, pavadinimai, kainos, kiekiai);
    if (prekiuKiekis > 0) {
        while (true) {
            cout << "\n1. Rodyti ataskaita\n"
                 << "2. Issaugoti ataskaita\n"
                 << "3. Rodyti prekes pagal kategorija\n"
                 << "0. Iseiti\n"
                 << "Pasirinkimas: ";

            string pasirinkimas;
            if (!getline(cin, pasirinkimas) || pasirinkimas == "0") {
                break;
            }

            if (pasirinkimas == "1") {
                isvestiAtaskaita(cout, kategorijos, pavadinimai,
                                 kainos, kiekiai, prekiuKiekis);
            } else if (pasirinkimas == "2") {
                ofstream failas("ataskaita.txt");
                if (!failas.is_open()) {
                    cout << "Nepavyko atidaryti ataskaita.txt.\n";
                    continue;
                }

                isvestiAtaskaita(failas, kategorijos, pavadinimai, kainos, kiekiai, prekiuKiekis);
                failas.close();

                if (failas.fail()) {
                    cout << "Nepavyko irasyti ataskaitos.\n";
                } else {
                    cout << "Ataskaita issaugota.\n";

                }
            } else if (pasirinkimas == "3") {
                cout << "Ivesk kategorija (Dviraciai, Dalys arba Aksesuarai): ";
                string kategorija;
                if (!getline(cin, kategorija)) {
                    break;
                }

                if (kategorija != "Dviraciai" && kategorija != "Dalys" &&
                    kategorija != "Aksesuarai") {
                    cout << "Neteisinga kategorija.\n";
                    continue;
                    }

                double suma = skaiciuotiKategorijosSuma(
                    kategorijos, kainos, kiekiai, prekiuKiekis, kategorija);

                if (suma == 0) {
                    cout << "Sioje kategorijoje prekiu nera.\n";
                    continue;
                }

                cout << fixed << setprecision(2);
                for (int i = 0; i < prekiuKiekis; i++) {
                    if (kategorijos[i] == kategorija) {
                        cout << pavadinimai[i] << " | " << kainos[i]
                             << " Eur/vnt. | Kiekis: " << kiekiai[i] << '\n';
                    }
                }
                cout << "Kategorijos moketina suma: " << suma << " Eur\n";
            } else {
                cout << "Neteisingas pasirinkimas.\n";
            }
        }

    }
    return 0;
}
int atidarytiFaila(string kategorijos[], string pavadinimai[],
                  double kainos[], int kiekiai[]) {
    ifstream failas("uzsakymas.txt");
    if (!failas.is_open()) {
        cout << "Failo atidaryti nepavyko. \n";
        return 0;
    }
    cout << "Failas atidarytas. \n";
    string eilute;
    getline(failas, eilute);

    istringstream antraste(eilute);
    int irasuSkaicius = 0;

    if (!(antraste >> irasuSkaicius) || irasuSkaicius < 1 || irasuSkaicius > 100) {
        cout << "Neteisingas irasu skaicius.\n";
        return 0;
    }

    antraste >> ws;
    if (!antraste.eof()) {
        cout << "Pirmoje eiluteje turi buti tik irasu skaicius.\n";
        return 0;
    }

    for (int i = 0; i < irasuSkaicius; i++) {
        getline(failas, eilute);
        istringstream duomenys(eilute);
        string kategorija;
        getline(duomenys, kategorija, ';');
        string pavadinimas;
        getline(duomenys, pavadinimas, ';');
        string kainosTekstas;
        getline(duomenys, kainosTekstas, ';');
        int kiekis = 0;
        duomenys >> kiekis;
        double kaina = stod(kainosTekstas);
        kategorijos[i] = kategorija;
        pavadinimai[i] = pavadinimas;
        kainos[i] = kaina;
        kiekiai[i] = kiekis;
    }

    return irasuSkaicius;
}
double skaiciuotiEilutesSuma(double kaina, int kiekis) {
    return kaina * kiekis;
}
double skaiciuotiNuolaida(double eilutesSuma, int kiekis) {
    if (kiekis >= 10) {
        return eilutesSuma * 0.10;
    }
    return 0.0;
}
    double skaiciuotiKategorijosSuma(const string kategorijos[],const double kainos[], const int kiekiai[],int prekiuKiekis, string kategorija) {
        double suma = 0.0;

        for (int i = 0; i < prekiuKiekis; i++) {
            if (kategorijos[i] == kategorija) {
                double eilutesSuma = skaiciuotiEilutesSuma(kainos[i], kiekiai[i]);
                double nuolaida = skaiciuotiNuolaida(eilutesSuma, kiekiai[i]);
                suma = suma + (eilutesSuma - nuolaida);
            }
        }

        return suma;
    }
void isvestiAtaskaita(ostream& isvestis,
                     const string kategorijos[], const string pavadinimai[],
                     const double kainos[], const int kiekiai[], int prekiuKiekis){
    double visoMoketi = 0.0;
    double visoPriesNuolaidas = 0.0;
    isvestis << fixed << setprecision(2);

    isvestis << "Kategorija | Pavadinimas | Kaina (Eur) | Kiekis | "
         << "Pries nuolaida (Eur) | Nuolaida (Eur) | Moketi (Eur)\n";

    for (int i = 0; i < prekiuKiekis; i++) {
        string kategorija = kategorijos[i];
        string pavadinimas = pavadinimai[i];
        double kaina = kainos[i];
        int kiekis = kiekiai[i];

        double eilutesSuma = skaiciuotiEilutesSuma(kaina, kiekis);
        visoPriesNuolaidas = visoPriesNuolaidas + eilutesSuma;
        double nuolaida = skaiciuotiNuolaida(eilutesSuma, kiekis);
        double moketinaSuma = eilutesSuma - nuolaida;
        visoMoketi = visoMoketi + moketinaSuma;

        isvestis << kategorija << " | " << pavadinimas << " | "
         << kaina << " | " << kiekis << " | " << eilutesSuma
         << " | " << nuolaida << " | " << moketinaSuma << '\n';
    }

    isvestis << ""<< '\n';
    isvestis << "Is viso moketi-> " << visoMoketi << " Eur\n";
    isvestis << "Is viso pries nuolaidas-> " << visoPriesNuolaidas << " Eur\n";
    double visoNuolaidu = visoPriesNuolaidas - visoMoketi;
    isvestis << "Is viso nuolaidu-> " << visoNuolaidu << " Eur\n";

    isvestis <<"\n Suma pagal prekių kategoriją" << "\n->\n";
    isvestis << "Dviraciai: "
         << skaiciuotiKategorijosSuma(kategorijos, kainos, kiekiai, prekiuKiekis, "Dviraciai")
         << " Eur\n";

    isvestis<< "Dalys: "
         << skaiciuotiKategorijosSuma(kategorijos, kainos, kiekiai, prekiuKiekis, "Dalys")
         << " Eur\n";

    isvestis<< "Aksesuarai: "
         << skaiciuotiKategorijosSuma(kategorijos, kainos, kiekiai, prekiuKiekis, "Aksesuarai")
         << " Eur\n";
}

