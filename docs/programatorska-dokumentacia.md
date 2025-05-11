# Programátorská dokumentácia projektu Míny + dynamický generátor

## Presné zadanie úlohy

Odkaz na [špecifikáciu](specification.md)

## Slovník
Nasledujúce pojmy sú použité inak v dokumentácii, ktorá je napísaná v slovenskom jazyku, a v kóde aplikácie, ktorá je napísaná v anglickom jazyku a v programátorskom jazyku C++.

|   Dokumentácia    	|   Kód 				|
|	---					|	---					|
|   bunka, políčko  	|   Cell				|
|	(herné) pole		|	Field				|
|	okno				|	sf::RenderWindow	|
|	vlajka, vlajočka	|	CellStatus::flagged	|

## Fungovanie aplikácie

### Príprava na beh

Na začiatku programu dochádza k vytvoreniu herného poľa buniek, ktoré predstavujú herné políčka s mínou alebo `číslom`.<sup>[1]</sup>. V programe nemôže dôjsť k tomu, aby mín bolo príliš veľa. Je zabezpečené, aby mín nebolo viac ako je buniek v poli. Inak by dochádzalo k undefined behaviour. Taktiež je zabezpečené, aby míny nezaberali viac ako 2 desatiny celkovej plochy poľa. Toto obmedzenie vychádza z rýchlostnej požiadavky špecifikácie:

```
Dynamický generátor a kontrolný mechanizmus budú dostatočne rýchle na to, aby bolo zabezpečené, že samotná hra nebude viditeľne spomalená a skúsenosť hráča tak nebude ovplyvnená negatívne.
```

Moje testy ukázali, že v prípade, keď míny zaberajú viac ako daný pomer políčok, dynamický generátor nefunguje dostatočne rýchlo.

Následne dochádza k vytvoreniu okna, do ktorého je vykreslené herné pole so všetkými bunkami, aktuálny čas hry, počet zostávajúcich mín a tlačítko `Nastavenia`.

[1]: Prípadne bez čísla, čo vlastne znamená číslo 0.

### Opakovanie základnej hernej funkcie

Programu dominuje `"nekonečný"`<sup>[2]</sup> while cyklus, ktorý v každej iterácii vymaže a znovu nakreslí každý objekt v okne. Toto je praktika knižnice SFML, ktorú využívam, a podľa ktorej je toto odporúčaný postup, prebiehajúci najrýchlejšie, a na ktorý sú moderné obrazovky prispôsobené.

Po kliknutí myšou do okna tento cyklus zachytí event kliknutia myši a obsluhujúca funkcia zabezpečí správne vyhodnotenie aktivity. Kliknutím ľavým tlačidlom myši na bunku je bunka odhalená. V prípade, že bunka obsahuje mínu, hra končí prehrou hráča. Inak môže hráč kliknúť na inú bunku, až pokiaľ neodhalené nezostanú len bunky s mínami. Vtedy hra končí tiež, tentokrát však výhrou. Koniec hry spozná hráč zastavením časomiery. Ak však hráč `kedykoľvek`<sup>[3]</sup> klikne na políčko, ktoré nesusedí so žiadnym odhaleným políčkom, tak do tohto políčka sa vygeneruje mína, ktorá ukončí hru. Dôvodom je, že hráč nemá ako vedieť, čo sa na danom políčku nachádza a teda sa snaží o tipovanie, ktoré je v tejto verzii hry zakázané.

Po konci hry nasledujúce kliknutie ľavým tlačidlom na myši hru reštartuje s novým herným poľom vygenerovaným nanovo.

Okrem ľavého tlačidla na myši je možné kliknúť aj pravým tlačidlom. Vtedy je na bunku vykreslená vlajočka, indikujúca hráčovu domnienku o míne nachádzajúcej sa v danej bunke. Na vlajkou označenú mínu nie je možné kliknúť ľavým tlačidlom myši a nechcene ju tak odhaliť. V prípade, že sa užívateľ pomýlil a chcel by vlajku z bunky odstrániť, je to možné urobiť opakovaným stlačením pravého tlačidla na myši na danú bunku.

[2]: Prebieha až do zavretia programu tlačítkom X v pravom hornom rohu okna

[3]: Okrem prvého kliknutia ľavým tlačidlom myši na pole

### Generovanie herného poľa

Generovanie herného poľa je spustené po prvom kliknutí ľavým tlačidlom myši na pole. Týmto je zabezpečené, že hráč nemôže prehrať hneď po prvom kliknutí, kedy je ešte celkom bez informácie o stave poľa, lebo program si zapamätá, kam hráč klikol a tak vie, kde mínu nemôže vygenerovať.

Následne dochádza k opakovanému generovaniu rôznych herných polí, až kým nie je vygenerované také pole, ktoré je možné vyriešiť bez tipovania. Overenie existencie riešenia zabezpečuje časť programu s názvom Solver. Solver funguje len na základe 4 jednoduchých pravidiel, medzi ktoré nepatrí tipovanie. Ak teda Solver dokáže vyriešiť dané pole, je zaručené, že dané pole je možné vyriešiť bez tipovania. Spomenuté pravidlá obsahujú:

1. Ak okolo bunky B nie je očakávaná žiadna ďalšia bunka M s mínou, tak všetky neodhalené bunky okolo bunky B sú bezpečné (neobsahujú mínu).

2. Ak sa okolo bunky B nachádza práve toľko neodhalených mín ako je okolo bunky B neodhalených buniek tak všetky neodhalené bunky okolo bunky B sú míny.

3. Máme bunku B1 a bunku B2. Ak všetky neodhalené bunky susediace s bunkou B1 patria medzi neodhalené bunky susediace s bunkou B2 a počet neodhalených susedných buniek je vyšší u bunky B2 ako u B1 a zároveň v susedstve B2 ešte očakávame `rovnaký` počet mín ako v susedstve B1, tak všetky neodhalené bunky okolo B2, ktoré nesusedia s B1 `sú bezpečné`.

4. Máme bunku B1 a bunku B2. Ak všetky neodhalené bunky susediace s bunkou B1 patria medzi neodhalené bunky susediace s bunkou B2 a počet neodhalených susedných buniek je `o X` vyšší u bunky B2 ako u B1 a zároveň v susedstve B2 ešte očakávame `o X vyšší` počet mín ako v susedstve B1, tak všetky neodhalené bunky okolo B2, ktoré nesusedia s B1 `obsahujú míny`.

Pre vyššiu prehľadnosť sú medzi pravidlami 3 a 4 zvýraznené ich rozdiely.

Ak Solver nájde pole, ktoré je riešiteľné len za pomoci týchto 4 pravidiel, výsledné pole je nakopírované do poľa, ktoré je zobrazované aktívnym oknom, na ktoré má užívateľ povolené kliknúť.

### Okno nastavenia

Nové okno, obsahujúce aktuálne nastavenia je možné otvoriť po kliknutí do spodného panelu pôvodného okna. Po otvorení má užívateľ možnosť zmeniť počet buniek na výšku a na šírku poľa a takisto aj počet mín. V prípade, že užívateľ zadá nepodporované hodnoty premenných, sa tieto hodnoty prepíšu. Hodnoty sa prepíšu do maximálnej hodnoty, ak užívateľ hodnotu prekročil zhora, alebo do minimálnej hodnoty, ak užívateľ hodnotu podliezol zdola. Zadané hodnoty je možné vymazať tlačidlom `Backspace`. Aktuálne novo zadané hodnoty môže užívateľ potvrdiť tlačidlom Confirm. Týmto sa pôvodné okno aj s celým poľom prekreslí a nanovo vygeneruje tak, aby vyhovovalo novým hodnotám premenných.
