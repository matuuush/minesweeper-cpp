# Užívateľská dokumentácia projektu Míny + dynamický generátor

## Popis hry

Míny sú zábavná počítačová hra založená na postupnom odkrývaní hracieho poľa obsahujúceho míny a bezpečné políčka. Cieľom hry je odhaliť všetky bezpečné políčka bez kliknutia na mínu. Táto verzia hry obsahuje jedno vylepšene pôžitku z hry. Vďaka mechanizmu generovania poľa, ktoré zabezpečuje, že hra má vždy riešenie sa hráčovi nikdy nestane, že by klikol na mínu vo svojom prvom ťahu, keď nemá informáciu ani o jednom bezpečnom políčku. Táto verzia hry však obsahuje aj jednu vlastnosť pre odporcov tipovania. Je zaručené, že hráč klikne na mínu, ak sa pokusí odhaliť políčko, o ktorého susedoch nemá žiadnu informáciu.

## Ciele hry

Pozitívny cieľ: Hráč vyhráva hru, ak odhalí všetky bezpečné políčka.

Negatívny cieľ: Ak hráč odhalí mínu, vybuchne a prehráva hru.

Splnenie niektorého cieľa hry spozorujete zastavením časomiery.

Negatívny cieľ spoznáte navyše podľa toho, že na hracej ploche odhalíte mínu.

## Začiatok hry

Po spustení je hra nachystaná na hru s pôvodnými nastaveniami - 10 mín na poli s veľkosťou 10x10 políčok. Pre začatie hry stačí odhaliť ľubovoľné políčko. V prípade, že si hráč želá spustiť hru s inými nastaveniami, je možné kedykoľvek otvoriť okno s nastaveniami stlačením tlačidla `Settings`.

## Reštartovanie hry

Po ľubovoľnom konci hry (pozitívnom alebo negatívnom) je možné začať novú hru opätovným stlačením ľavého tlačidla na myši pokiaľ sa kurzor myši nachádza na hracej ploche.

## Ovládanie

Ľavé tlačidlo na myši - Odhaliť políčko / Spustiť hru odznova

Pravé tlačidlo na myši - Označiť políčko vlajočkou / Odstrániť vlajku

Čísla na klávesnici - Nastaviť veľkosť poľa a počet mín

Backspace - Vymazať hodnotu v nastaveniach

## Nastavenia

Sú prístupné po kliknutí na dolný panel okna s nápisom `Settings`. Po otvorení je možné kliknúť na pôvodné hodnoty nastavení a zmeniť ich pomocou čísel na klávesnici. Hodnotu je možné vymazať po kliknutí na tlačítko `Backspace`. Potvrdenie nových hodnôt nastavení a generovanie poľa s novými nastaveniami je možné vykonať stlačením tlačidla `Confirm`.