int main(){
    std::list<Studentas> studentai;
    std::list<Studentas> vargsiukai;
    std::list<Studentas> galvociai;

    char pasirinkimas;
    std::cout << "Ar norite ivesti duomenis (R) ar skaityti is failo (F) ar generuoti (G)? ";
    std::cin >> pasirinkimas;

    if (pasirinkimas == 'G' || pasirinkimas == 'g') {
        int studentuSkaicius;
        std::cout << "Kiek studentu norite sugeneruoti? ";
        std::cin >> studentuSkaicius;

        auto startCreateFileTime = std::chrono::high_resolution_clock::now();

        std::string fileName = "studentai_" + std::to_string(studentuSkaicius) + ".txt";
        std::ofstream outFile(fileName);
        outFile << std::setw(15) << std::left << "Vardas" << std::setw(15) << std::left << "Pavarde";

        for (int i = 1; i <= 5; ++i) {
            outFile << std::setw(5) << std::right << "nd" << i;
        }
        outFile << std::setw(5) << std::right << "egz\n";
        outFile << "-----------------------------------------------------------------------\n";

        for (int i = 1; i <= studentuSkaicius; ++i) {
            outFile << std::setw(15) << std::left << "Vardas" + std::to_string(i)
                    << std::setw(15) << std::left << "Pavarde" + std::to_string(i);
            for (int j = 0; j < 5; ++j) {
                outFile << std::setw(5) << std::right << generuotiNdBala();
            }
            outFile << std::setw(5) << std::right << generuotiEgzBala() << "\n";
        }

        outFile.close();

        auto endCreateFileTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> CreateFileTime = endCreateFileTime - startCreateFileTime;
        std::cout << "Failo" << fileName << "kūrimas uztruko: " << CreateFileTime.count() << " sekundes" << std::endl;


        std::cout << "Failas '" << fileName << "' buvo sugeneruotas.\n";
        return 0;
    }





    if (pasirinkimas == 'R' || pasirinkimas == 'r') {
        Studentas naujasStudentas;
        naujasStudentas.address = &naujasStudentas;

        do {
            std::cout << "Iveskite studento varda: ";
            std::cin >> naujasStudentas.vardas;
            std::cout << "Iveskite studento pavarde: ";
            std::cin >> naujasStudentas.pavarde;

            char generuotiPazymius;
            std::cout << "Ar norite, kad namu darbu ir egzamino pazymiai butu generuojami? (T/N): ";
            std::cin >> generuotiPazymius;

            if (generuotiPazymius == 'T' || generuotiPazymius == 't') {
                int NdKiekis = rand() % 10 + 1;
                for (int i = 0; i < NdKiekis; ++i) {
                    naujasStudentas.Nd.push_back(generuotiNdBala());
                }
                naujasStudentas.egz = generuotiEgzBala();
            } else {
                try {
                    int pazymys;
                    std::cout << "Iveskite namu darbu pazymius (baigus parasyk -1): ";
                    while (std::cin >> pazymys) {
                        if (pazymys == -1) {
                            break;
                        }
                        if (pazymys < 1 || pazymys > 10) {
                            throw std::runtime_error("Invalid. ND pazymys turi buti tarp 1-10.");
                        }
                        naujasStudentas.Nd.push_back(pazymys);
                    }
                    std::cout << "Iveskite egzamino pazymi: ";
                    std::cin >> naujasStudentas.egz;
                    if (naujasStudentas.egz < 1 || naujasStudentas.egz > 10) {
                        throw std::runtime_error("Invalid. Egzamino pazymys turi buti tarp 1-10.");
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Klaida: " << e.what() << std::endl;
                    return 1;
                }
            }

            studentai.push_back(naujasStudentas);



            std::cout << "Ar norite ivesti dar vieno studento duomenis? (T/N): ";
            std::cin >> pasirinkimas;

        } while (pasirinkimas == 'T' || pasirinkimas == 't');

        char baloSkaiciavimoBudas;
        std::cout << "Kaip norite skaiciuoti galutini bala pagal vidurki (V) ar pagal mediana (M)? ";
        std::cin >> baloSkaiciavimoBudas;


        auto partitionPoint = std::stable_partition(studentai.begin(), studentai.end(), [&](const Studentas& studentas) {
            double galutinisBalas = skaiciuotiGalutiniBala(studentas, baloSkaiciavimoBudas);
            return galutinisBalas < 5.0;
        });

        vargsiukai.assign(studentai.begin(), partitionPoint);
        galvociai.assign(partitionPoint, studentai.end());

        char rikiavimas;
        std::cout << "Ar norite, kad galutiniai rezultatai butu surikiuoti (Y/N)? ";
        std::cin >> rikiavimas;

        if (rikiavimas == 'Y' || rikiavimas == 'y') {
        std::cout << "Kaip norite surikiuoti rezultatus:\n"
                     "1. Pagal vardà\n"
                     "2. Pagal pavardæ\n"
                     "3. Pagal galutiná balà\n"
                     "Jûsø pasirinkimas: ";
        int rikiavimoKriterijus;
        std::cin >> rikiavimoKriterijus;

        char rikiavimoTvarka;
        std::cout << "Kaip norite surikiuoti rezultatus - didžiausi pirma (D) ar mažiausi pirma (M)? ";
        std::cin >> rikiavimoTvarka;

        auto compareByCriterion = [rikiavimoKriterijus, rikiavimoTvarka, baloSkaiciavimoBudas](const Studentas& a, const Studentas& b) {
            switch (rikiavimoKriterijus) {
                case 1:
                    return (rikiavimoTvarka == 'D' || rikiavimoTvarka == 'd') ? a.vardas > b.vardas : a.vardas < b.vardas;
                case 2:
                    return (rikiavimoTvarka == 'D' || rikiavimoTvarka == 'd') ? a.pavarde > b.pavarde : a.pavarde < b.pavarde;
                case 3:
                    return (rikiavimoTvarka == 'D' || rikiavimoTvarka == 'd') ?
                        skaiciuotiGalutiniBala(a, baloSkaiciavimoBudas) > skaiciuotiGalutiniBala(b, baloSkaiciavimoBudas) :
                        skaiciuotiGalutiniBala(a, baloSkaiciavimoBudas) < skaiciuotiGalutiniBala(b, baloSkaiciavimoBudas);
                default:
                    // Jei pasirinktas neteisingas kriterijus, nekeičiame tvarkos.
                    return false;
            }
        };

        vargsiukai.sort(compareByCriterion);
        galvociai.sort(compareByCriterion);

        }

        std::cout << std::endl;
        int studentCount = studentai.size();
        std::cout <<"studentu sk:" << studentCount << std::endl;


        std::cout << std::endl;
        std::cout << "vargsiukai:" << std::endl;
        std::cout << std::endl;
        std::cout << std::setw(15) << std::left << "Vardas" << std::setw(15) << std::left << "Pavarde"
                  << std::setw(15) << std::right << "Galutinis" << std::setw(15) << std::right << "Adresas" << std::endl;
        std::cout << "-----------------------------------------------------------------------" << std::endl;

        for (const Studentas& studentas : vargsiukai) {
            double galutinisBalas = skaiciuotiGalutiniBala(studentas, baloSkaiciavimoBudas);
            std::cout << std::setw(15) << std::left << studentas.vardas << std::setw(15) << std::left << studentas.pavarde
                      << std::setw(15) << std::fixed << std::setprecision(2) << std::right << galutinisBalas
                      << std::setw(15) << std::right << &naujasStudentas << "\n";
        }

        std::cout << std::endl;
        std::cout << std::endl;


        std::cout << "galvociai:" << std::endl;
        std::cout << std::endl;
        std::cout << std::setw(15) << std::left << "Vardas" << std::setw(15) << std::left << "Pavarde"
                  << std::setw(15) << std::right << "Galutinis" << std::setw(15) << std::right << "Adresas" << std::endl;
        std::cout << "-----------------------------------------------------------------------" << std::endl;

        for (const Studentas& studentas : galvociai) {
            double galutinisBalas = skaiciuotiGalutiniBala(studentas, baloSkaiciavimoBudas);
            std::cout << std::setw(15) << std::left << studentas.vardas << std::setw(15) << std::left << studentas.pavarde
                      << std::setw(15) << std::fixed << std::setprecision(2) << std::right << galutinisBalas
                      << std::setw(15) << std::right << &naujasStudentas << "\n";
        }

        std::cout << std::endl;



    } else if (pasirinkimas == 'F' || pasirinkimas == 'f') {
        try {
            std::string failoPavadinimas;
            std::cout << "Iveskite failo pavadinima: ";
            std::cin >> failoPavadinimas;

            std::ifstream inFile(failoPavadinimas);
            if (!inFile) {
                throw std::runtime_error("Nepavyko atidaryti failo");
            }

            char baloSkaiciavimoBudas;
            std::cout << "Kaip norite skaiciuoti galutini bala pagal vidurki (V) ar pagal mediana (M)? ";
            std::cin >> baloSkaiciavimoBudas;





            auto startReadFileTime = std::chrono::high_resolution_clock::now();

            std::string eilute;
            while (std::getline(inFile, eilute)) {
                Studentas naujasStudentas;
                std::istringstream iss(eilute);
                iss >> naujasStudentas.vardas >> naujasStudentas.pavarde;

                int pazymys;
                while (iss >> pazymys) {
                    naujasStudentas.Nd.push_back(pazymys);
                }

                if (!naujasStudentas.Nd.empty()) {
                    naujasStudentas.egz = naujasStudentas.Nd.back();
                    naujasStudentas.Nd.pop_back();

                    studentai.push_back(naujasStudentas);
                }
            }

            inFile.close();
            int studentCount = studentai.size();
            auto endReadFileTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> readFileTime = endReadFileTime - startReadFileTime;



            auto startSortTime = std::chrono::high_resolution_clock::now();

            auto partitionPoint = std::stable_partition(studentai.begin(), studentai.end(), [&](const Studentas& studentas) {
            double galutinisBalas = skaiciuotiGalutiniBala(studentas, baloSkaiciavimoBudas);
                return galutinisBalas < 5.0;
            });

            vargsiukai.assign(studentai.begin(), partitionPoint);
            galvociai.assign(partitionPoint, studentai.end());
            auto endSortTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> sortTime = endSortTime - startSortTime;


            char rikiavimas;
            std::cout << "Ar norite, kad galutiniai rezultatai butu surikiuoti (Y/N)? ";
            std::cin >> rikiavimas;

            if (rikiavimas == 'Y' || rikiavimas == 'y') {
            std::cout << "Kaip norite surikiuoti rezultatus:\n"
                         "1. Pagal vardà\n"
                         "2. Pagal pavardæ\n"
                         "3. Pagal galutiná balà\n"
                         "Jûsø pasirinkimas: ";
            int rikiavimoKriterijus;
            std::cin >> rikiavimoKriterijus;

            char rikiavimoTvarka;
            std::cout << "Kaip norite surikiuoti rezultatus - didžiausi pirma (D) ar mažiausi pirma (M)? ";
            std::cin >> rikiavimoTvarka;

            auto compareByCriterion = [rikiavimoKriterijus, rikiavimoTvarka, baloSkaiciavimoBudas](const Studentas& a, const Studentas& b) {
                switch (rikiavimoKriterijus) {
                    case 1:{
                        std::string numStrA = a.vardas.substr(6); // Assuming "vardas" is followed by a number
                        std::string numStrB = b.vardas.substr(6);
                        int numA = std::stoi(numStrA);
                        int numB = std::stoi(numStrB);
                        return (rikiavimoTvarka == 'D' || rikiavimoTvarka == 'd') ? numA > numB : numA < numB;
                    }

                    case 2:{
                        std::string numStrA = a.pavarde.substr(7); // Assuming "vardas" is followed by a number
                        std::string numStrB = b.pavarde.substr(7);
                        int numA = std::stoi(numStrA);
                        int numB = std::stoi(numStrB);
                        return (rikiavimoTvarka == 'D' || rikiavimoTvarka == 'd') ? numA > numB : numA < numB;
                    }

                    case 3:
                        return (rikiavimoTvarka == 'D' || rikiavimoTvarka == 'd') ?
                            skaiciuotiGalutiniBala(a, baloSkaiciavimoBudas) > skaiciuotiGalutiniBala(b, baloSkaiciavimoBudas) :
                            skaiciuotiGalutiniBala(a, baloSkaiciavimoBudas) < skaiciuotiGalutiniBala(b, baloSkaiciavimoBudas);
                    default:
                        // Jei pasirinktas neteisingas kriterijus, nekeičiame tvarkos.
                        return false;
                }
            };

            vargsiukai.sort(compareByCriterion);
            galvociai.sort(compareByCriterion);

            }


            std::cout << std::endl;
            std::cout << "Studentu skaicius: " << studentCount << std::endl;
            std::cout << "Failo nuskaitymas su " << studentCount << " studentu uztruko: " << readFileTime.count() << " sekundes" << std::endl;


            std::cout << "Failo su " << studentCount << " studentu dalijimas i 2 grupes uztruko: " << sortTime.count() << " sekundes" << std::endl;



            auto startWriteVargsiukaiTime = std::chrono::high_resolution_clock::now();

            std::ofstream vargsiukaiFile("vargsiukai.txt");
            vargsiukaiFile << std::setw(15) << std::left << "Vardas" << std::setw(15) << std::left << "Pavarde" << std::setw(15) << std::right << "Galutinis" << std::endl;
            vargsiukaiFile << "---------------------------------------------" << std::endl;
            for (const Studentas& studentas : vargsiukai) {
                double galutinisBalas = skaiciuotiGalutiniBala(studentas, baloSkaiciavimoBudas);
                vargsiukaiFile << std::setw(15) << std::left << studentas.vardas << std::setw(15) << std::left << studentas.pavarde << std::setw(15) << std::fixed << std::setprecision(2) << std::right << galutinisBalas << "\n";
            }
            vargsiukaiFile.close();
            auto endWriteVargsiukaiTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> writeVargsiukaiTime = endWriteVargsiukaiTime - startWriteVargsiukaiTime;
            std::cout << "Failo isvedimas su " << studentCount << " studentu i vargsiukai uztruko: " << writeVargsiukaiTime.count() << " sekundes" << std::endl;

            auto startWriteGalvociaiTime = std::chrono::high_resolution_clock::now();

            std::ofstream galvociaiFile("galvociai.txt");
            galvociaiFile << std::setw(15) << std::left << "Vardas" << std::setw(15) << std::left << "Pavarde" << std::setw(15) << std::right << "Galutinis" << std::endl;
            galvociaiFile << "---------------------------------------------" << std::endl;
            for (const Studentas& studentas : galvociai) {
                double galutinisBalas = skaiciuotiGalutiniBala(studentas, baloSkaiciavimoBudas);
                galvociaiFile << std::setw(15) << std::left << studentas.vardas << std::setw(15) << std::left << studentas.pavarde << std::setw(15) << std::fixed << std::setprecision(2) << std::right << galutinisBalas << "\n";
            }
            galvociaiFile.close();
            auto endWriteGalvociaiTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> writeGalvociaiTime = endWriteGalvociaiTime - startWriteGalvociaiTime;
            std::cout << "Failo isvedimas su " << studentCount << " studentu i galvociai uztruko: " << writeGalvociaiTime.count() << " sekundes" << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Klaida: " << e.what() << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Neteisingas pasirinkimas." << std::endl;
        return 1;
    }

    return 0;
}


