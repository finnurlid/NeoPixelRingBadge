/*

  Kildekode til NeoPixel ring
  Kodeklubben Salangen
  Tilpasset fra Adafruits "strandtest.ino"
  BEBB 2019

  Kodeklubbens badge bruker:
  Arduino Pro Micro (ATmega32u4)
  Kompatibel med Leonardo board i IDE
  16 LED NeoPixel ring (WS2812)

*/

// Inkluder NeoPixel bibliotek for å kunne bruke
// ord som ringen forstår i programmet vårt:
#include <Adafruit_NeoPixel.h>

// Nummer på koblingspunkt (pin) på Arduino:
#define DATA_PIN 15

// Antall LED-pixler på ringen:
#define LED_COUNT 16

// Lag et NeoPixel objekt:
Adafruit_NeoPixel ring(LED_COUNT, DATA_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Antall pixler i NeoPixel ringen
// Argument 2 = Arduino pinnenummer
// Argument 3 = Pixel type flagg (det finnes flere typer enn den vi bruker):
//   NEO_KHZ800  800 KHz bitstrøm (800.000 bits per sekund, 0,00000125sek per bit)
//   NEO_GRB     Grønn Rød Blå

// Her har vi fortalt programmet vårt at vi har 16 NeoPixler
// koblet til vår Arduino på pinne nummer 15, og at dette er 
// en ting, eller et "objekt". Hver pixel får 24 biter med data,
// 8 for hver farge: Grønn, Rød og Blå. Vi har også gitt objektet
// vårt et navn: "ring".

// Ring-objektet har en slags notatblokk i RAM. Der kan vi fargelegge
// ringen, uten at det vises mens vi jobber med fargeleggingen. Det
// vises først når vi gjør ring.show()

// setup() kjører én gang under oppstart  --------------------------------

void setup() {
  ring.begin(); // Starter NeoPixel ring objektet
  ring.show(); // Slå av alle pixler først
  ring.setBrightness(40); // Sett lysstyrken til ca 1/6 (max = 255)
}


// loop() kjører hele tiden mens strømmen er på, etter at setup() er ferdig.

void loop() {
  // Fyll hele ringen med forskjellige farger, med en sveip-effekt:
  fyllFarge(ring.Color(255,   0,   0), 50); // Rød
  fyllFarge(ring.Color(  0, 255,   0), 50); // Grønn
  fyllFarge(ring.Color(  0,   0, 255), 50); // Blå

  // Gjør en snurre-effekt i forskjellige farger...
  snurrFarge(ring.Color(127, 127, 127), 50); // Hvit, halv lysstyrke (127 av 255)
  snurrFarge(ring.Color(127,   0,   0), 50); // Rød, halv lysstyrke (127 av 255)
  snurrFarge(ring.Color(  0,   0, 127), 50); // Blå, halv lysstyrke (127 av 255)

  regnbue(10); // Vri en regnbue rundt hele ringen...
  snurrGlitterRegnbue(50); // Glittre en regnbue rundt hele ringen...
}


// Her lager vi noen funksjoner for å animere farger -----------------

// Fyll pixlene i ringen med farge, en etter en. Ringen viskes ikke ut
// først; fargen som er der fra før blir byttet ut pixel for pixel.
// Vi sender farger som en hel 'pakke' med 32-bit lengde, som vi får
// ved å bruke ring.Color(rød, grønn, blå) som vist i loop().
// Vi har også en 'ventetid' (i millisekunder) mellom hver pixel.
void fyllFarge(uint32_t farge, int ventetid) {
  for(int i=0; i<ring.numPixels(); i++) { // For hver pixel i ringen...
    ring.setPixelColor(i, farge);         // Sett en pixels farge (i RAM).
    ring.show();                          // Oppdater ringen så det vises.
    delay(ventetid);                      // Vent litt, så en gang til.
  }
}

// Løpende prikker snurrer på ringen. Send inn en farge (32-bit lengde,
// lik ring.Color(r,g,b) slik som 'fyllFarge' over), og en ventetid (i ms)
// mellom 'bildene' i animasjonen.
void snurrFarge(uint32_t farge, int ventetid) {
  for(int a=0; a<10; a++) {     // Gjenta 10 ganger...
    for(int b=0; b<3; b++) {    // 'b' teller fra 0 til 2...
      ring.clear();             // Sett alle pixler i RAM til 0 (av)
      // 'c' teller opp fra 'b' til enden/starten av ringen i steg på 3...
      for(int c=b; c<ring.numPixels(); c += 3) {
        ring.setPixelColor(c, farge); // Sett pixel nummer 'c' til verdi 'farge', i RAM
      }
      ring.show(); // Oppdater ringen til de nye fargene slik det står i RAM
      delay(ventetid);  // Vent litt, og gjenta...
    }
  }
}

// Snurrer en regnbue på ringen. Gi den en ventetid mellom 'bildene'i animasjonen,
// som et Argument mellom parentesene: regnbue(ventetiden i millisekunder).
void regnbue(int ventetid) {
  // Fargen på første pixel kjører 5 fulle løkker/loops gjennom fargepaletten.
  // Paletten har 65536 farger, men det er ok å telle for langt, for 65537 er
  // det samme som neste farge i mikrokontrollerens øyne. Tenk på det som
  // klokka: vi skjønner at om 25 timer er klokka 1 time mer enn den er nå.
  // Bare tell fra 0 til 5*65536. Å legge 256 til startpixelFarge hver gang
  // betyr at vi går gjennom paletten med skritt som er 256 lange. Slik at vi
  // går 5*65536/256 = 1280 omganger gjennom denne yttre loopen:
  for(long startpixelFarge = 0; startpixelFarge < 5*65536; startpixelFarge += 256) {
    for(int i=0; i<ring.numPixels(); i++) { // For hver pixel i ringen...

      // Forskyv pixelFarge litt for å få én full rotering av paletten (65536 farger lang)
      // rundt lengden av ringen. Ringen svarer hvor lang den er hvis vi spør den
      // med å si ring.numPixels()
      // Vi vet at den er 16 pixler lang, men denne koden virker om vi bytter til
      // en større eller mindre ring.
      int pixelFarge = startpixelFarge + (i * 65536L / ring.numPixels());

      // HSV er en vanlig måte å beskrive en farge til en datamaskin. HSV er en
      // forkortelse for Hue, Saturation og Value: Farge, Mettning og Verdi.
      // Alle vet hva Farge er, men hva er Mettning og Verdi?
      // Mettning er hvor sterk fargen er, fra blek pastell til knall neon.
      // Value er hvor lyssterk fargen skal være. En slags dimmer.
      // ring.ColorHSV() kan ta 1 eller 3 argumenter: En Farge (Hue, 0 til 65535),
      // og hvis du vil, en Mettning og en Verdi (hver 0 til 255).
      // F.eks: ring.ColorHSV(65535) eller ring.ColorHSV(65535,255,255), velg egne tall.
      // Her bruker vi bare Farge-argumentet. Resultatet sendes gjennom ring.gamma32()
      // for å gi mer 'ekte' farger (det blir rett og slett finere), før vi gir fargen
      // videre til pixelen:
      ring.setPixelColor(i, ring.gamma32(ring.ColorHSV(pixelFarge)));
    }
    ring.show(); // Oppdater ringen med de nyeste fargene
    delay(ventetid);  // Vent litt
  }
}

// Snurrer en glitrende regnbue på ringen. Gi den en ventetid (i millisekunder, ms)
// mellom 'bildene' i animasjonen. Legg merke til at vi bruker både det engelske ordet 'hue'
// og det norske ordet 'farge' om to litt forskjellige ting. Det er fordi datamaskiner trenger
// tall for både Hue, Saturation og Luminosity for å beskrive det vi mennesker bare kaller
// farge. Er du spesielt interessert i dette kan du lese om Fargemodell og Fargerom.
void snurrGlitterRegnbue(int ventetid) {
  int startpixelFarge = 0; // Første pixel starter på rød (hue 0)
  for(int a=0; a<30; a++) { // Gjenta 30 ganger...
    for(int b=0; b<3; b++) { // 'b' teller fra 0 til 2...
      ring.clear(); // Sett alle pixlene i RAM til 0 (av)
      // 'c' teller opp fra 'b' til enden/starten av ringen i steg på 3...
      for(int c=b; c<ring.numPixels(); c += 3) {
        // Hue på pixel 'c' forskyves litt for å få hele regnbuen til å gå opp med
        // lengden av ringen:
        int      hue   = startpixelFarge + c * 65536L / ring.numPixels();
        uint32_t farge = ring.gamma32(ring.ColorHSV(hue)); // hue -> RGB
        ring.setPixelColor(c, farge); // Sett pixel 'c' til 'farge'
      }
      ring.show(); // Oppdater ringen med de nye fargene som ligger klar i RAM
      delay(ventetid); // Vent litt
      startpixelFarge += 65536 / 90; // Én runde av regnbuen rundt ringen over 90 bilder i animasjonen
    }
  }
}
