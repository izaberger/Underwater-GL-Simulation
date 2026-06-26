# Underwater-GL-Simulation

## O projekcie
Interaktywna aplikacja 3D w C++/OpenGL przedstawiająca podwodną jaskinię w fioletowo-różowej estetyce. Projekt skupia się na zaawansowanym renderingu PBR, efektach post-processingu (Bloom, HDR) oraz dynamicznej manipulacji obiektami w czasie rzeczywistym.

<table align="center">
  <tr>
    <td><img src="https://github.com/user-attachments/assets/da46ceda-4361-40af-889b-c55faa1d25d8" alt="Screenshot 1"></td>
    <td><img src="https://github.com/user-attachments/assets/a75dc03b-0701-4a80-b1ec-d41b33623d34" alt="Screenshot 2"></td>
  </tr>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/c4715117-6e04-40cf-a948-ecdefb37d84c" alt="Screenshot 3"></td>
    <td><img src="https://github.com/user-attachments/assets/18469850-e3c9-4db8-aa50-43cef205bf0b" alt="Screenshot 4"></td>
  </tr>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/8bfe4642-6f0d-46e3-95eb-547c3f116041" alt="Screenshot 5"></td>
    <td><img src="https://github.com/user-attachments/assets/c0aa5036-adec-4505-8ac2-5fd0414032c5" alt="Screenshot 6"></td>
  </tr>
</table>

## Technologie i Biblioteki
* **Język:** C++ / GLSL
* **Biblioteki:** GLFW, GLEW, GLM, Assimp, SOIL, ImGui

## Sterowanie i Interakcje

### Nawigacja
* `Ctrl + W/A/S/D/Q/E` – Ruch kamerą (wspiera poruszanie się w górę/dół/boki).
* `C` – Przełączanie trybu sterowania myszką (Mouse Look).

### Zarządzanie sceną (Edytor)
* `C` – Przełączanie trybu sterowania (Rozglądanie się myszką / Swobodny kursor do obsługi UI).
* `Strzałki (← / → / ↑ / ↓)` – Obracanie kamery przy pomocy klawiatury (alternatywa dla myszki).
* `Ctrl + P` – Wypisanie aktualnej pozycji kamery w konsoli (przydatne do ustalania kadrów).

### Interakcje specjalne
* `Space` – Uruchomienie animacji bąbelka.
* `G` – Włączenie/wyłączenie pulsowania blasku kryształów.

## Zaimplementowane metody

### Metody obowiązkowe
* **Normal mapping** – detale powierzchni w przestrzeni.
* **PBR lighting** – parametry szorstkości i metaliczności materiałów.
* **Quaternion camera control** – nawigacja bez efektu gimbal lock.
* **Shadow mapping** – dynamiczne cienie z bufora głębi.
* **Parallel Transport Frames** – stabilna orientacja wzdłuż wyznaczonej krzywej.
* **Underwater skybox/cubemap** – sześcienna tekstura symulująca odległe otoczenie.

### Metody zaawansowane
* **[A13] Environment reflections/refractions** – Refrakcja i załamanie światła na bańkach powietrza z wykorzystaniem tekstury tła.
* **[B05] Vertex shader waving** – Proceduralnie falujące wodorosty w shaderze wierzchołkowym, symulujące prądy morskie.
* **[B09] OBJ model loading** – Wczytywanie zewnętrznych modeli (skały, wodorosty, kryształy) przy użyciu biblioteki Assimp.
* **[B10] Depth fog** – Symulacja gęstości wody i zanikania widoczności proporcjonalnie do odległości.
* **[B12] Transparency** – Renderowanie obiektów półprzezroczystych (blending).

### Post-Processing
* **Bloom (HDR)** – Wyodrębnianie najjaśniejszych elementów sceny (emisja kryształów) do oddzielnego bufora, nakładanie rozmycia Gaussa i łączenie ich ze sceną.





