Paper Soccer
============

The game starts on an empty field of (usually) 8×10 boxes, with goals of two boxes wide marked in the centers of the two shorter sides. In the beginning, a ball is drawn in the center of the field, on the crossing of the paper lines.
Players take turns to move the ball into one of the eight paper line crossings around it (horizontally, vertically, or diagonally) and a segment from the original position to the new one is drawn to mark the move. The ball may move neither on the game field border nor on the segments marking the previous moves - instead, it "bounces" from them: a player who moves the ball into a position where there is already an end of a segment or a game field border gets another turn.
The first player to place the ball in the opponent's goal wins the game. The game may also end when a player does not have a valid move, in which case that player loses.

![Screenshot](https://raw.github.com/Pawel834/Paper-Soccer/master/screen1.png "Screenshot")

Code description (in polish)
============================

Projekt używa wzorca MVC:
    - katalog src/models/ zawiera logikę gry
    - katalog src/views/ zawiera widoki
    - katalog src/controllers/ zawiera kontrolery

Klasa Application składa to wszystko w całość. Zawiera ona w sobie instancje
wszystkich modeli, kontrolerów i widoków, poznwala zmnieniać aktywny widok i 
kontroler. Posiada też akcje takie jak "newGame", "loadGame".

Główne okno (i menu) tworzy klasa MainWindow. Jego centralWidget to kontener
z QStackedLayout'em trzymającym widoki.

Historia reprezentowana jest jako lista plansz. Takie rozwiązanie pozwala
wyświetlić miniaturki plansz w widoku historii. Rozmiar plansz jest na tyle mały
(max 30x30) że nie ma co się martwić zużyciem pamięci.

Kontrolery mogą zostać aktywowane i deaktywowane. Zawsze aktywny jest conajwyżej
jeden kontroler.

Format zapisu danych jest binarny, używający QDataStream. Modele implementują
operatory << i >> serializując poprostu swoją wewnętrzną reprezentacje.

AI to dość prosty alpha-beta. Działa całkiem nieźle (wygrywa ze mną, ale to nie
jest wielkie osiągnięcie), chociaż w walkach AI vs AI czasami widać problemy gdy
wykonuje głupi ruch, bo przez za małą głębokość przeszukiwania myśli że ruch 
jest wygrywający.
AI jest wykonywane w osobnym wątku, zwraca wynik przez sygnał (połączany za
pomocą queued connection z wątkiem GUI).

W katalogu src/ jest pare klas utility, jak Either, Maybe i Shape (nawiązujące
do kształtu tablicy, jak w NumPy - przydała się do 3-wymiarowej tablicy w klasie
Board).
