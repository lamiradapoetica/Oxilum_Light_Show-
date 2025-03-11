#include <Adafruit_NeoPixel.h>

#define LED_PIN 6    
#define NUM_LEDS 300 
#define BRILLO_GENERAL 170  // 🔧 Ajusta el brillo general (0-255)

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_RGB + NEO_KHZ800);  // ✅ Configurado correctamente para WS2815 RGB

// 🔹 Definir la estructura `Seccion`
struct Seccion {
    int inicio;
    int cantidad;
    String nombre;
};

// 🔹 Secciones
Seccion arriba[] = {
    {297, 33, "Arriba A"},
    {224, 33, "Arriba B"},
    {150, 33, "Arriba C"}
};

Seccion abajo[] = {
    {261, 35, "Abajo A"},
    {188, 34, "Abajo B"}
};

// 🔹 Centro (Siempre con luz roja tenue excepto en efectos)
Seccion centro = {96, 97, "Centro"};

// 🔹 Bordes (Siempre apagados)
int bordes[][2] = {
    {299, 2}, {264, 3}, {226, 2}, {191, 3}, {154, 4}, {117, 3}, {114, 18} 
};

void setup() {
    strip.begin();
    strip.setBrightness(BRILLO_GENERAL);
    strip.show();
    Serial.begin(9600);
    encenderCentroBase();  // 🔥 Activa la luz roja tenue en el centro
}

void loop() {


     efectoIntroduccion();       // 🔥 55 segundos
    encenderCentroBase();

    efectoTenebroso();          // 🔥 45 segundos
    encenderCentroBase();

    efectoFlashEstroboscopico(); // 🔥 30 segundos
    encenderCentroBase();

    efectoTransicionColores();  // 🔥 35 segundos
    encenderCentroBase();

    efectoPsicodelico();        // 🔥 35 segundos
    encenderCentroBase();
}

// ----------------------
// 🔥 LUZ BASE EN EL CENTRO 🔥
// ----------------------
void encenderCentroBase() {
    Serial.println("Centro: Luz Roja tenue activada");
    for (int j = 0; j < centro.cantidad; j++) {
        int pos = centro.inicio - j;
        strip.setPixelColor(pos, strip.Color(50, 0, 0));  // 🔧 Rojo al 20% de brillo
    }
    apagarBordes(); // 🔥 Asegura que los bordes se mantengan apagados
    strip.show();
}

// 🔹 Bordes siempre apagados
void apagarBordes() {
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < bordes[i][1]; j++) {
            strip.setPixelColor(bordes[i][0] - j, strip.Color(0, 0, 0));
        }
    }
}

// ----------------------
// 🔥 1️⃣ INTRODUCCIÓN (Solo ARRIBA) 🔥
// ----------------------
void efectoIntroduccion() {
    Serial.println("Efecto de Introducción");

    // 🔥 EDITABLE DURATIONS (in milliseconds)
    int duracionRojo = 20000;   // Time in solid Red
    int duracionRojoAzul = 20000;  // Transition time from Red → Blue
    int duracionAzulBlanco = 20000; // Transition time from Blue → White

    // 🔥 1️⃣ Solid RED phase
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < arriba[i].cantidad; j++) {
            strip.setPixelColor(arriba[i].inicio - j, strip.Color(255, 0, 0));  // Pure Red
        }
    }
    strip.show();
    delay(duracionRojo);  // Wait in red

    // 🔥 2️⃣ Transition from RED → BLUE
    for (int step = 0; step < 256; step += 5) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < arriba[i].cantidad; j++) {
                strip.setPixelColor(arriba[i].inicio - j, strip.Color(255 - step, 0, step)); // Interpolates from Red to Blue
            }
        }
        strip.show();
        delay(duracionRojoAzul / (256 / 5)); // Automatically adjusts transition timing
    }

    // 🔥 3️⃣ Transition from BLUE → WHITE
    for (int step = 0; step < 256; step += 5) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < arriba[i].cantidad; j++) {
                strip.setPixelColor(arriba[i].inicio - j, strip.Color(255, 255, 255)); // Interpolates from Blue to White
            }
        }
        strip.show();
        delay(duracionAzulBlanco / (256 / 5)); // Automatically adjusts transition timing
    }

    // 🔥 4️⃣ Turn everything off
    apagarTodo();
    delay(5000);
}

// ----------------------
// 🔥 2️⃣ EFECTO TENEBROSO 🔥
// ----------------------
void efectoTenebroso() {
    Serial.println("Efecto Tenebroso - Moving Blocks with Same Color");

    unsigned long inicio = millis();
    while (millis() - inicio < 50000) {  
        strip.clear(); // Clear previous frame

        for (int i = 0; i < 3; i++) {  // Loop through ARRIBA sections
            int startPos = arriba[i].inicio - random(arriba[i].cantidad - 5); // Pick a random position for the block
            
            // Pick a single random color for this block
            int r = random(255);
            int g = random(255);
            int b = 0; // Keep it in the red/yellow range
            
            for (int k = 0; k < 5; k++) {  // Light up a group of 5 LEDs together
                int pos = startPos - k;
                if (pos >= 0 && pos < NUM_LEDS) {
                    strip.setPixelColor(pos, strip.Color(r, g, b)); // Same color for all 5 LEDs
                }
            }
        }

        strip.show();
        delay(200);  // Adjust speed of movement
    }

        // 🔥 4️⃣ Turn everything off
    apagarTodo();
    delay(5000);
}



// ----------------------
// 🔥 3️⃣ FLASH ESTROBOSCÓPICO (30s) 🔥
// ----------------------
void efectoFlashEstroboscopico() {
    Serial.println("Efecto Flash Estroboscópico (30s)");

    int totalDuration = 30000; 
    int flashDuration = 100;   
    int iterations = totalDuration / (flashDuration * 2); 

    for (int i = 0; i < iterations; i++) {
        strip.fill(strip.Color(255, 255, 255)); 
        strip.show();
        delay(flashDuration);

        apagarTodo();
        delay(flashDuration);
    }
}

// ----------------------
// 🔥 4️⃣ TRANSICIÓN 🔥
// ----------------------
void efectoTransicionColores() {
    Serial.println("Efecto de Transición de Colores");

    for (int step = 0; step < 256; step += 5) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < arriba[i].cantidad; j++) {
                strip.setPixelColor(arriba[i].inicio - j, strip.Color(255, 255, step));
            }
        }
        strip.show();
        delay(100);
    }
}

// ----------------------
// 🔥 5️⃣ PSICODÉLICO 🔥
// ----------------------
void efectoPsicodelico() {
    Serial.println("Efecto Psicodélico");

    unsigned long inicio = millis();
    while (millis() - inicio < 30000) {
        for (int j = 0; j < NUM_LEDS; j += 5) {
            strip.setPixelColor(j, strip.Color(random(255), random(255), random(255)));
        }
        strip.show();
        delay(100);
    }
}

// ----------------------
// 🔥 Apagar Todo 🔥
// ----------------------
void apagarTodo() {
    Serial.println("Todo apagado.");
    strip.clear();
    strip.show();
}
