Nazev   : Editor a simulator vysokourovnových Petriho siti
Autori  : Martin Knapovsky, Martin Cincala
Poznamka: Projekt do predmetu ICP, FIT VUT v Brne

Toto README obsahuje pouze nejdulezitejsi informace, zbytek informaci lze nalezt v dokumentaci k programu, ktera vznikne vygenerovanim pomoci make doxygen v adresari ./doc/html

#Pouziti
make           - Preklad serveru a klienta
make run       - Spusteni klienta
make runserver - Spusteni serveru
make doxygen   - Vygenerovani dokumentace do ./doc/html/
make pack      - Vytvoreni archivu s projektem
make clean     - Odstraneni docasnych souboru a vygenerovane dokumentace

#Login#
Uzivatelske jmeno: "testuser"
Heslo: "password"

#Priklady#
./examples/networks - Priklady siti
./examples/configs  - Konfiguracni soubory klienta
./examples/history  - Ukazka souboru s historii simulace
./examples/protocol - Ukazka zprav komunikacniho protokolu
                      a samotny komunikacni protokol

#Server#
Server2012 implementuje TCP server a metody pro simulaci vysokourovnovych petriho siti.

Implementace:
Komunikace s klientem probiha pomoci XML souboru. Krok simulace ma 3 hlavni faze: overeni dostupnosti tokenu, kontrola podminek a presun tokenu. Kazde misto v petriho siti obsahuje mnozinu tokenu, ktera je postupne filtrovana. Pokud je po vyfiltrovani mnozina prazdna, prechod nelze provest a klient tuto skutecnost oznami prislusnou hlaskou.

Spusteni:
Server se spousti bez parametru prikazem make runserver. Standardne nasloucha na portu 44444. Kdyz je port 44444 nedostupny, pouzije se nahodny volny port. 

#Klient#
Klient slouzi k tvorbe, prohlizeni a simulaci simulacnich souboru, ktere je mozne nahrat z mistniho uloziste, ci ze serveru, ke kteremu se klient pripojuje na zaklade zadane ip adresy a portu. Klient je nasledne autentizovan pomoci zadaneho jmena a hesla. Site, ktere byli ulozeny na server je mozne prochazet a filtrovat dle vyhledavaneho retezce v polich, ktere uzivatel vybere pro prohledavani. Pri prochazeni siti je mozne prochazet take simulacni historii dane site. Klientska aplikace uzivateli umoznuje vytvaret Petriho site volbou daneho modu mysi, ktery se prepina v toolbaru Mode. Mista a prechody ve scene lze kopirovat, vkladat a odstranovat s tim, ze se pri vlozeni vzdy vytvori novy prvek s unikatnim id. Pro upravu vlastnosti daneho prvku je k dispozici widget, jehoz zobrazeni lze zapnout ci vypnout pomoci tlacitka "Display item properties". Simulace je pak spoustena, ci krokovana pomoci tlacitek v toolbaru. Simulaci je mozne vratit do predchoziho stavu pomoci tlacitka se zpetnou sipkou. Aktualni nastaveni je mozne ulozit do souboru config.xml. V adresari ./examples/configs jsou pripraveny soubory config1.xml a config2.xml, ktere je mozne nacist a nastavit tak chovani klienta.

#Dalsi soubory#
./src/Doxyfile - Nastaveni pro vygenerovani dokumentace 
                 pomoci programu doxygen
./src/icon.png - Ikona programu pro doxygen
