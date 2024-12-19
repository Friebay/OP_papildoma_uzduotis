# OP_papildoma_uzduotis

VU OP Papildoma užduotis. std::string konteinerio, o taip pat ir asociatyvių konteinerių naudojimas.

## Motivation

So far, we have mostly used containers of type sequence, i.e. they keep the order in which we put the elements (e.g. using `insert` or `push_back()`) into the containers. However, for certain tasks, other types of containers are more appropriate. For example, to check whether a certain number (e.g. 100) exists in a container, using sequence containers we have basically two choices:
1. Either we check each element of the container against 100 until we find that the element does exist, or we check all the elements to make sure that the element does not exist;
2. Sort the container using a sorting algorithm and then use some more clever strategy to quickly check if such an item exists in the container. However, in this case, we have to re-sort the container after adding new elements.

Obviously, for both cases the implementation will be rather slow and inefficient.

This task is intended to investigate that for this type of tasks associative containers, i.e. containers that "sort" the elements during the insertion of the elements into the container, are more suitable and more efficient compared to sequential containers.

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


## Naudojimas

1. **Įvesties tekstas**: Pateikite bent 1000 žodžių teksto failą analizei atlikti.
2. **Paleiskite programą**: Paleiskite programą, kad būtų atlikta žodžių analizė, sukurta kryžminių nuorodų lentelė ir išskirtos nuorodos.
3. **Išvestis**: Rezultatai bus įrašyti į failus ir rodomi ekrane.

