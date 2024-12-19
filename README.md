# OP_papildoma_uzduotis

VU OP Papildoma užduotis. std::string konteinerio, o taip pat ir asociatyvių konteinerių naudojimas.

## Užduoties formuluotė

1. Suskaičiuokite, kiek kartų kiekvienas skirtingas žodis pasikartoja Jūsų tekste. Tekstą galite paimti iš bet kur, pvz.: Vikipediją straipsnį apie Vilnių (Geriau kad būtu kitoks tekstas, ne mažiau 1000 žodžių). Tuomet realizacijos output'e (išoriniame faile) išveskite skirtingus žodžius (kableliai, taškai ir kiti skyrybos ženklai nėra žodžiu dalis!), kurie pasikartojo dažniau negu 1 kartą, o taip pat ir count'erį, nurodantį kiek kartų konkretus žodis pasikartojo.

2. Sukurkite cross-reference tipo lentelę kurioje būtų nurodyta, kurioje teksto vietoje (kurioje(-iose) teksto eilutėse) kiekvienas iš daugiau negu vieną kartą pasikartojantis žodis buvo paminėtas.

3. URL adresų suradimas Jūsų tekste. Kaip ir pirmos užduoties atveju, tekstą galite paimti iš bet kur, tačiau būtina, kad tame tekste būtų bent keli URL'ai, pateikti "pilnu" https://www.vu.lt/, http://www.vu.lt ar "sutrumpintu" pavidalu: www.vu.lt, vu.lt, saitas.xyz, . Jūsų tikslas, iš to teksto išrinkti visus šiuos URL'us ir grąžiai atspausdinti ekrane (ar išvesti į failą)!

## Funkcionalumas

Ši programa:
- analizuoja pateiktą tekstą, kad suskaičiuotų žodžių dažnumą ir nustatytų žodžius, kurie pasitaiko daugiau nei vieną kartą.
- Sudaro kryžminių nuorodų lentelę, kurioje nurodomos eilutės, kuriose kartojasi žodžiai.
- Išveda visus tekste rastus URL adresus.

## Projekto Struktūra

- **src/**: kodo (.cpp) failai
- **include/**: header (.h) failai
- **data/**: tekstai ir kiti duomenys
- **output/**: rezultatai
- **README.md**: projekto aprašymas

## Instaliavimas
 - Atsisiųskite arba jau sukompiliuotą versiją, arba programos kodą.
 - Jeigu atsisiųsite kodą, tuomet paleiskite „compile_cmake.cmd“, kad susikompiliuotų kodas.

## Naudojimas

1. **Įvesties tekstas**: Pateikite bent teksto failą analizei atlikti.
2. **Paleiskite programą**: Paleiskite programą, kad būtų atlikta žodžių analizė, sukurta kryžminių nuorodų lentelė ir išskirtos nuorodos.
3. **Išvestis**: Rezultatai bus įrašyti į failus.

